/**
 * We implement a hashtable align with Python. There are some difference between Python.
 * 1. This hashtable is C++ style and the key and value will never split.
 * 2. We use under score case since it's copied from another library.
 * 3. Python dict use two array to save elements. One for indices(arr1) and another for elements(arr2). 
 * When removing an element, set value of arr1 to -2 and arr2 to nullptr(each element in Python is a PyObject*) is OK. 
 * But the element type in C++ does not require for pointer type. In such way, this dict cannot keep ordered.
 * 
*/


#include <type_traits>
#include <utility>
#include <bit>
#include <memory>
#include <assert.h>

namespace python::collections
{
    // For set<int>, the key_type and value_type is int
    // For set<const int>, the key_type and value_type is const int
    struct identity
    {
        template <typename T> 
        using key_type = std::tuple_element_t<0, T>;

        template <typename T> 
        using value_type = key_type<T>;

        template <typename T>
        constexpr auto&& operator()(T&& x) const 
        { return (T&&)x; }
    };

    // For map<int, int>, the key_type is int and value_type is std::pair<const int, int>
    // For map<const int>, the key_type is const int and value_type is std::pair<const int, int>
    struct select1st
    {
        template <typename T> 
        using key_type = std::tuple_element_t<0, T>;

        template <typename T> 
        using value_type = std::pair<const std::tuple_element_t<0, T>, std::tuple_element_t<1, T>>;

        template <typename T>
        constexpr auto&& operator()(T&& x) const 
        { return std::get<0>((T&&)x); }
    };

    namespace detail
    {

        template <typename T> concept is_transparent = requires 
        { typename T::is_transparent; };

        template <bool IsTransparent>
        struct key_arg_helper 
        {
            template <typename K1, typename K2>
            using type = K1;
        };

        template <>
        struct key_arg_helper<false> 
        {
            template <typename K1, typename K2>
            using type = K2;
        };

        template <bool IsTransparent, class K1, class K2>
        using key_arg = typename key_arg_helper<IsTransparent>::template type<K1, K2>;
    
        template <typename T, typename... Args>
        struct emplace_helper
        {
        private:
            constexpr static auto is_same_as_key = [](){
                if constexpr (sizeof...(Args) != 1)
                    return false;
                else
                    return std::conjunction_v<std::is_same<T, std::remove_cvref_t<Args>>...>;
            }();
        public:
            constexpr static bool value = is_same_as_key;
        };

        template <typename T, typename Alloc>
        T* allocate(Alloc& alloc, std::size_t n)
        {
            using alloc_traits = typename std::allocator_traits<Alloc>::template rebind_traits<T>;
            using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;
            alloc_type a{ alloc };
            T* addr = alloc_traits::allocate(a, n); 
            return addr;
        }

        template <typename Alloc, typename T>
        void deallocate(Alloc& alloc, T* p, std::size_t n)
        {
            using alloc_traits = typename std::allocator_traits<Alloc>::template rebind_traits<T>;
            using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;
            alloc_type a{ alloc };
            alloc_traits::deallocate(a, p, n); 
        }

        template <typename T> 
        struct cache_hash_code : std::true_type { };

        template <typename T, bool Cache>
        struct storage_impl
        {
            T m_value;

            storage_impl(const storage_impl&) = delete;

            storage_impl(storage_impl&&) = delete;

            template <typename... Args>
            storage_impl(Args&&... args) : m_value((Args&&)args...) { }

            constexpr auto& value()  
            { return m_value; }

            constexpr auto& value() const 
            { return m_value; }

            constexpr T* value_ptr() 
            { return std::addressof(m_value); }

            constexpr const T* value_ptr() const
            { return std::addressof(m_value); }
        };

        template <typename T>
        struct storage_impl<T, true> : storage_impl<T, false>
        {
            std::size_t m_hash_code;

            template <typename... Args>
            constexpr storage_impl(std::size_t hash_code, Args&&... args)
                : storage_impl<T, false>{ (Args&&) args... }, m_hash_code{ hash_code } { }
        };

        template <std::size_t PerturbShift = 5>
        struct py_hash_generator
        {
            constexpr py_hash_generator(std::size_t hash_init, std::size_t table_size)  
            {
                assert(is_power_of_two(table_size));
                m_mask = table_size - 1;
                m_perturb_shift = hash_init;
                m_value = hash_init & m_mask;
            }

            constexpr std::size_t operator()() 
            {
                // count++;
                m_perturb_shift >>= PerturbShift;
                m_value = (5 * m_value + 1 + m_perturb_shift) & m_mask;
                return m_value;
            }

            constexpr static bool is_power_of_two(std::size_t x) 
            { return std::popcount(x) == 1; }

            constexpr std::size_t operator*() const 
            { return m_value; }

            std::size_t m_value;
            std::size_t m_perturb_shift;
            std::size_t m_mask;
        };

    } // namespace detail

    template <typename TypePack, 
        typename HashFunction, 
        typename KeyEqual, 
        typename Allocator, 
        typename HashGenerator,
        typename KeyOfValue,
        bool IsUniqueKey>
    class hash_table 
    {
        static_assert(IsUniqueKey, "Only support unique-key now.");

    public:

        using hasher = HashFunction;
        using key_equal = KeyEqual;
        using allocator_type = Allocator;
        using key_type = typename KeyOfValue::template key_type<TypePack>;
        using value_type = typename KeyOfValue::template value_type<TypePack>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

    protected:

        constexpr static bool IsTransparent = detail::is_transparent<hasher> && detail::is_transparent<key_equal>;
        
        template <typename U>
        using key_arg_t = detail::key_arg<IsTransparent, U, key_type>;

        constexpr static bool CacheHashCode = true;
        using index_type = std::size_t;
        using slot_type = detail::storage_impl<value_type, CacheHashCode>;
        using hash_generator_type = HashGenerator;

        using slot_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<slot_type>;
        using slot_alloc_traits = std::allocator_traits<slot_allocator>;

        using indices_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<index_type>;
        using indices_alloc_traits = std::allocator_traits<indices_allocator>;

        constexpr static index_type SlotUnused = static_cast<index_type>(-1);
        constexpr static index_type SlotDeleted = static_cast<index_type>(-2);
        constexpr static std::size_t default_hash_size = 8;

        constexpr static bool IsNothrowMoveConstruct = 
                    std::is_nothrow_move_constructible_v<hasher> 
                 && std::is_nothrow_move_constructible_v<key_equal> 
                 && typename slot_alloc_traits::is_always_equal()
                 && typename indices_alloc_traits::is_always_equal();

        constexpr static bool IsNothrowMoveAssign = 
                    std::is_nothrow_move_assignable_v<hasher> 
                 && std::is_nothrow_move_assignable_v<key_equal> 
                 && typename slot_alloc_traits::is_always_equal()
                 && typename indices_alloc_traits::is_always_equal();

        constexpr static bool IsNothrowSwap = 
                    std::is_nothrow_swappable_v<hasher> 
                 && std::is_nothrow_swappable_v<key_equal> 
                 && typename slot_alloc_traits::is_always_equal()
                 && typename indices_alloc_traits::is_always_equal();

        template <bool Const>
        struct hash_iterator 
        {
            using link_type = std::conditional_t<Const, const hash_table*, hash_table*>;

            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = std::conditional_t<Const, const value_type, value_type>;
            using reference = value_type&;
            using difference_type = std::ptrdiff_t;


            link_type m_c;
            std::size_t m_idx;

            constexpr hash_iterator() = default;

            constexpr hash_iterator(const hash_iterator&) = default;

            constexpr hash_iterator(link_type c, std::size_t idx)  
                : m_c{ c }, m_idx{ idx } { }

            constexpr hash_iterator(const hash_iterator<!Const>& rhs) requires (Const)
                : m_c{ rhs.m_c }, m_idx{ rhs.m_idx } { }


            constexpr bool operator==(const hash_iterator&) const = default;

            constexpr reference operator*() const 
            {
                auto pos = m_c->m_indices[m_idx];
                return m_c->m_slots[pos].value();
            }

            constexpr auto operator->() const 
            { return std::addressof(**this); }

            constexpr hash_iterator& operator++()
            {
                m_idx++;
                for (; m_idx < m_c->m_capacity && m_c->m_indices[m_idx] >= SlotDeleted; m_idx++);
                return *this;
            }

            constexpr hash_iterator operator++(int) 
            {
                auto old = *this;
                ++*this;
                return old;
            }

            constexpr hash_iterator& operator--()
            {
                m_idx--;
                for (; m_idx != static_cast<std::size_t>(-1) && m_c->m_indices[m_idx] >= SlotDeleted; m_idx--);
                return *this;   
            }

            constexpr hash_iterator operator--(int) 
            {
                auto old = *this;
                --*this;
                return old;
            }

        };

    public:

        using iterator = hash_iterator<false>;
        using const_iterator = hash_iterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    protected:

        template <typename K>
        bool check_equal(std::size_t hash_code, std::size_t pos, const K& x) const 
        {
            // for integer with hash(x) = x, compare hash_code is equivalent to x == value
            // for std::string or other string types, compare hash_code first may faster
            if constexpr (CacheHashCode)
            {
                return hash_code == m_slots[pos].m_hash_code 
                    && m_ke(x, KeyOfValue()(m_slots[pos].value()));
            }
            else
            {
                return m_ke(x, KeyOfValue()(m_slots[pos].value()));
            }
        }

        /**
         * @brief: Try insert element and return the position of index that the element inserted.
         * @return: (index, exits)
        */
        template <typename U>
        std::pair<std::size_t, bool> insert_with_hash_code(U&& u, std::size_t hash_code)
        {
            HashGenerator g { hash_code, m_capacity };
            auto offset = *g;

            slot_allocator alloc { m_alloc };

            while (1)
            {
                auto& state = m_indices[offset];

                if (state == SlotUnused)
                {
                    // Insert element here
                    if constexpr (CacheHashCode)
                    {
                        slot_alloc_traits::construct(alloc, m_slots + m_used, hash_code, (U&&) u); 
                    }
                    else
                    {
                        slot_alloc_traits::construct(alloc, m_slots + m_used, (U&&) u); 
                    }
                    state = m_used;
                    m_used++;
                    m_size++;
                    return { offset, false };
                }

                // If the slot is deleted, we just skip it. 
                // If the slot is active, we try to compare it with element

                if (state != SlotDeleted && check_equal(hash_code, m_indices[offset], KeyOfValue()(u)))
                {
                    // The element is exist.
                    return { offset, true };
                }

                offset = g();

            }
            std::unreachable();
        }

        template <typename U>
        void rehash_insert_with_hash_code(U&& u, std::size_t hash_code)
        {
            HashGenerator g { hash_code, m_capacity };
            auto offset = *g;

            slot_allocator alloc { m_alloc };

            while (1)
            {
                auto& state = m_indices[offset];

                // We move the elements from old table to new table.
                // Each slot in new table only has two state: active or unused.
                if (state == SlotUnused)
                {
                    if constexpr (CacheHashCode)
                    {
                        slot_alloc_traits::construct(alloc, m_slots + m_used, hash_code, (U&&) u); 
                    }
                    else
                    {
                        slot_alloc_traits::construct(alloc, m_slots + m_used, (U&&) u); 
                    }
                    state = m_used;
                    m_used++;
                    m_size++;
                    return;
                }

                offset = g();

            }
            std::unreachable();
        } 

        template <typename U>
        std::pair<std::size_t, bool> insert_impl(U&& x)
        {
            rehash_and_growth_if_necessary();
            const auto hash = m_hash(KeyOfValue()(x));
            return insert_with_hash_code((U&&) x, hash);
        }

        void initialize(std::size_t new_capacity)
        {
            auto new_indices = detail::allocate<index_type>(m_alloc, new_capacity);
            auto new_slot = detail::allocate<slot_type>(m_alloc, new_capacity);
            std::uninitialized_fill_n(new_indices, new_capacity, SlotUnused);
            m_indices = new_indices;
            m_slots = new_slot;
            m_capacity = new_capacity;
        }

        void rehash_and_growth_if_necessary()
        {
            if (m_capacity == 0)
            {
                resize(default_hash_size);
                return;
            }

            constexpr double threshold = 2.0 / 3;

            const double factor = (double) m_used / m_capacity;
            if (factor > threshold)
            {
                const auto new_capacity = m_capacity << 1;
                resize(new_capacity);
            }
        }

        void resize(std::size_t new_capacity)
        {
            auto old_indices = m_indices;
            auto old_slots = m_slots;
            auto old_capacity = m_capacity;

            initialize(new_capacity); 

            // If we don't cache hash code, we should use m_hash to get hash code again for each active value.
            // If std::is_nothrow_move_construable_v<T>, the rehash should be  and we can call std::move for
            // each element otherwise we must copy each element so that if an exception is thrown, the state of 
            // container is not changed. 
            m_size = 0;
            m_used = 0;
            slot_allocator alloc { m_alloc };
            for (std::size_t i = 0; i < old_capacity; ++i)
            {
                auto state = old_indices[i];
                if (state == SlotUnused)
                {
                    continue;
                }
                else if (state == SlotDeleted)
                {
                    slot_alloc_traits::destroy(alloc, old_slots + state);
                }
                else
                {
                    std::size_t hash_code;
                    auto pos = state;
                    if constexpr (CacheHashCode)
                    {
                        hash_code = old_slots[pos].m_hash_code;
                    }
                    else
                    {
                        hash_code = m_hash(KeyOfValue()(old_slots[pos].value()));
                    }

                    rehash_insert_with_hash_code(std::move_if_noexcept(old_slots[pos].value()), hash_code);
                    slot_alloc_traits::destroy(alloc, old_slots + pos);
                }
            }

            if (old_capacity)
            {
                assert(old_indices && old_slots && "these pointers should never be nullptr");
                detail::deallocate(m_alloc, old_indices, old_capacity);
                detail::deallocate(m_alloc, old_slots, old_capacity);
            }   
        }

        template <typename K>
        std::size_t find_slot_by_key(const K& x) const 
        {
            
            auto hash_code = m_hash(x);

            auto offset = find_slot_by_key_aux(x, hash_code);
            
            if (offset == m_capacity)
                return m_capacity;

            
            auto pos = m_indices[offset];
            if (pos == SlotUnused)
                return m_capacity;

            return offset;
        }

        template <typename K>
        size_type remove_by_key(const K& x)
        {
            auto idx = find_slot_by_key(x);
            if (idx == m_capacity)
                return 0;
            m_indices[idx] = SlotDeleted;
            m_size--;
            return 1;
        }

        template <typename I>
        iterator remove_by_iterator(I iter)
        {
            auto idx = iter.m_idx;
            m_indices[idx] = SlotDeleted;
            m_size--;
            return std::next(iterator(this, idx));
        }

        /**
         * @brief: Find the location of x.
         * @return: The offset of x in indices.
        */
        template <typename K>
        std::size_t find_slot_by_key_aux(const K& x, std::size_t hash_code) const 
        {
            if (m_capacity == 0)
            {
                return m_capacity;
            }
            
            HashGenerator g { hash_code, m_capacity };

            auto offset = *g;

            while (1)
            {
                auto pos = m_indices[offset];

                if (pos != SlotDeleted)
                {
                    if (pos == SlotUnused || check_equal(hash_code, pos, x))
                        return offset;
                }

                offset = g();
            }
            std::unreachable();
        }


    public:

        hash_table() 
            : m_hash{ }, m_ke{ }, m_alloc{ }, m_indices{ }, m_slots{ }, m_size{ }, m_capacity{ }, m_used{ }
        {
        }

        hash_table(hasher hash, key_equal ke, allocator_type alloc)
            : m_hash{ hash }, m_ke{ ke }, m_alloc{ alloc }, m_indices{ }, m_slots{ }, m_size{ }, m_capacity{ }, m_used{ }
        {
        }

        // TODO: 
        hash_table(const hash_table&) = delete;
        hash_table(hash_table&&) = delete;
        void swap(hash_table& rhs) = delete;
        hash_table& operator=(hash_table&&) = delete;
        hash_table& operator=(const hash_table&) = delete;


        std::size_t size() const 
        { return m_size; }

        bool empty() const 
        { return size() == 0; }

        std::size_t capacity() const 
        { return m_capacity; }

        hasher hash_function() const 
        { return m_hash; }

        key_equal key_eq() const 
        { return m_ke; }

        allocator_type get_allocator() const 
        { return m_alloc; }

        const index_type* indices() const
        { return m_indices; }

        const slot_type* slots() const
        { return m_slots; }

        // float load_factor() const 
        // { return empty() ? 0.0 : (float)m_size / m_capacity; }

        // float max_load_factor() const 
        // { return (float)HashPolicy::factor; }

        // void max_load_factor(float) 
        // { /* Does nothing. */ }

        size_t max_size() const 
        { return static_cast<std::size_t>(-1) - 2; }

        iterator begin()
        {
            std::size_t idx = 0;
            for (; idx < m_capacity && m_indices[idx] >= SlotDeleted; idx++);
            return { this, idx };
        }

        iterator end()
        { return { this, m_capacity }; }

        const_iterator begin() const
        { return const_cast<hash_table&>(*this).begin(); }

        const_iterator end() const
        { return const_cast<hash_table&>(*this).end(); }

        const_iterator cbegin() const 
        { return begin(); }

        const_iterator cend() const 
        { return end(); }

        reverse_iterator rbegin()
        { return std::make_reverse_iterator(end()); }
        
        reverse_iterator rend()
        { return std::make_reverse_iterator(begin()); }
        
        const_reverse_iterator rbegin() const
        { return std::make_reverse_iterator(end()); }

        const_reverse_iterator rend() const
        { return std::make_reverse_iterator(begin()); }

        const_reverse_iterator rcbegin() const
        { return std::make_reverse_iterator(end()); }

        const_reverse_iterator rcend() const
        { return std::make_reverse_iterator(begin()); }

        std::pair<iterator, bool> insert(const value_type& x)
        { return emplace(x); }

        std::pair<iterator, bool> insert(value_type&& x)
        { return emplace(std::move(x)); }

        iterator insert(const_iterator, const value_type& x) 
        { return insert(x).first; }

        iterator insert(const_iterator, value_type&& x) 
        { return insert(std::move(x)).first; }

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args)
        {
            if constexpr (detail::emplace_helper<value_type, Args...>::value)
            {
                // If there is only one arg and remove_cvref_t<decltype(arg)> is value_type
                // we just call insert_impl
                auto [idx, exist] = insert_impl((Args&&) args...);
                return { iterator(this, idx), !exist };
            }
            else
            {
                // We construct value on stack and try to move it.
                alignas(value_type) unsigned char raw[sizeof(value_type)];
                value_type* slot = reinterpret_cast<value_type*>(&raw);
                slot_allocator alloc { m_alloc };

                // Whether the value is move successfully, the destructor should be invoked. 
                // ~unique_ptr will destroy the object constructed.
                auto deleter = [&](value_type* p) {
                    slot_alloc_traits::destroy(alloc, p);
                };
                std::unique_ptr<value_type, decltype(deleter)> _ { slot, deleter };
                slot_alloc_traits::construct(alloc, slot, (Args&&) args...);
                auto [idx, exist] = insert_impl(std::move(*slot));
                return { iterator(this, idx), !exist };
            }
        }

        template <typename... Args>
        iterator emplace_hint(const_iterator, Args&&... args)
        { return emplace((Args&&) args...).first; }

        template <typename K = key_type>
        bool contains(const key_arg_t<K>& x) const
        { return find(x) != end(); }

        template <typename K = key_type>
        iterator find(const key_arg_t<K>& x) 
        { return { this, find_slot_by_key(x) }; }

        template <typename K = key_type>
        const_iterator find(const key_arg_t<K>& x) const
        { return const_cast<hash_table&>(*this).find(x); }

        iterator erase(iterator pos)
        { return remove_by_iterator(pos); }

        iterator erase(const_iterator pos)
        { return remove_by_iterator(pos); }

        // iterator erase( const_iterator first, const_iterator last );

        size_type erase(const key_type& x)
        { return remove_by_key(x); }

        template< class K >
            requires (IsTransparent && 
                !std::is_convertible_v<iterator, K> && 
                !std::is_convertible_v<const_iterator, K>)
        size_type erase(K&& x)
        { return remove_by_key(x); }

    protected:
    
        [[no_unique_address]] hasher m_hash;
        [[no_unique_address]] key_equal m_ke;
        [[no_unique_address]] allocator_type m_alloc;
    
        index_type* m_indices;      // store indices or state
        slot_type* m_slots;         // store entries
        std::size_t m_size;         // number of elements
        std::size_t m_capacity;     // table capacity
        std::size_t m_used;         // used slots, always point the end of m_slots
    };


    template <typename T>
    using hash_set = hash_table<
        std::tuple<T>, 
        std::hash<T>, 
        std::equal_to<T>, 
        std::allocator<T>, 
        detail::py_hash_generator<>, 
        identity, 
        true
    >;

    template <typename K, typename V, 
        typename HashFunction = std::hash<K>, 
        typename KeyEqual = std::equal_to<K>, 
        typename Allocator = std::allocator<std::pair<const K, V>>>
    class hash_map : public hash_table<
        std::tuple<K, V>,
        HashFunction,
        KeyEqual,
        Allocator,
        detail::py_hash_generator<>,
        select1st,
        true>
    {

        using base_hash_type = hash_table<
            std::tuple<K, V>,
            HashFunction,
            KeyEqual,
            Allocator,
            detail::py_hash_generator<>,
            select1st,
            true>;

        using base_hash_type::CacheHashCode;
        using base_hash_type::IsTransparent;
        using base_hash_type::SlotUnused;
        using base_hash_type::key_arg_t;

    public:

        using mapped_type = V;
        using typename base_hash_type::key_type;
        using typename base_hash_type::value_type;
        using typename base_hash_type::iterator;
        using typename base_hash_type::const_iterator;
        using typename base_hash_type::index_type;
        using typename base_hash_type::slot_allocator;
        using typename base_hash_type::slot_alloc_traits;


        V& operator[](const K& key)
        { return try_emplace(key).first->second; }

        V& operator[](K&& key)
        { return try_emplace(std::move(key)).first->second; }

        template <typename... Args>
        std::pair<iterator, bool> try_emplace(const K& k, Args&&... args)
        { return try_emplace_impl(k, (Args&&) args...); }

        template <typename... Args>
        std::pair<iterator, bool> try_emplace(K&& k, Args&&... args)
        { return try_emplace_impl(std::move(k), (Args&&) args...); }

        template <typename... Args>
        std::pair<iterator, bool> try_emplace(const_iterator, const K& k, Args&&... args)
        { return try_emplace_impl(k, (Args&&) args...); }

        template <typename... Args>
        std::pair<iterator, bool> try_emplace(const_iterator, K&& k, Args&&... args)
        { return try_emplace_impl(std::move(k), (Args&&) args...); }

        template <typename M>
        std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj)
        { return insert_or_assign_impl(k, (M&&)obj); }

        template <typename M>
        std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj)
        { return insert_or_assign_impl(std::move(k), (M&&)obj); }

    private:

        template <typename KK, typename M>
        std::pair<iterator, bool> insert_or_assign_impl(KK&& k, M&& obj)
        {
            this->rehash_and_growth_if_necessary();

            auto hash_code = this->m_hash(k);
            auto offset = this->find_slot_by_key_aux(k, hash_code);

            auto& state = this->m_indices[offset];
            if (state == SlotUnused)
            {
                slot_allocator alloc { this->m_alloc };
                if constexpr (CacheHashCode)
                {
                    slot_alloc_traits::construct(alloc, this->m_slots + this->m_used, hash_code, (KK&) k, (M&&) obj); 
                }
                else
                {
                    slot_alloc_traits::construct(alloc, this->m_slots + this->m_used, (KK&) k, (M&&) obj); 
                }
                state = this->m_used;
                this->m_used++;
                this->m_size++;
                return { iterator(this, state), true };
            }
            else
            {
                this->m_slots[state].value().second = (M&&)obj;
                return { iterator(this, state), false };
            }
        }

        template <typename KK, typename... Args>
        std::pair<iterator, bool> try_emplace_impl(KK&& k, Args&&... args)
        {
            this->rehash_and_growth_if_necessary();

            auto hash_code = this->m_hash(k);
            auto offset = this->find_slot_by_key_aux(k, hash_code);

            auto& state = this->m_indices[offset];
            if (state == SlotUnused)
            {
                slot_allocator alloc { this->m_alloc };
                if constexpr (CacheHashCode)
                {
                    slot_alloc_traits::construct(
                        alloc, 
                        this->m_slots + this->m_used, 
                        hash_code, 
                        std::piecewise_construct, 
                        std::forward_as_tuple((KK&) k),
                        std::forward_as_tuple((Args&&) args...)); 
                }
                else
                {
                    slot_alloc_traits::construct(
                        alloc, 
                        this->m_slots + this->m_used, 
                        std::piecewise_construct, 
                        std::forward_as_tuple((KK&) k),
                        std::forward_as_tuple((Args&&) args...)); 
                }
                state = this->m_used;
                this->m_used++;
                this->m_size++;
                return { iterator(this, state), true };
            }
            else
            {
                return { iterator(this, state), false };
            }
        }

    };


}
