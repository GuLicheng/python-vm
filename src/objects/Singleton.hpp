#pragma once


namespace python
{

    template <typename T>
    concept order_superable = requires (T t)
    {
        t.order_supers();
    };

    template <typename TDerived>
    class Singleton
    {
    public:
        using single_type = TDerived;

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        static single_type* get_instance() noexcept
        {
            static single_type instance;
            return &instance;
        }

    protected:

        constexpr Singleton() noexcept = default;

    };

} // namespace leviathan

