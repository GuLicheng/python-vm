#pragma once


namespace python
{

    template <typename Derived>
    class Singleton
    {
    public:
        using single_type = Derived;

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

