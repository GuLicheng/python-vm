#pragma once


namespace python
{
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

    template <typename T>
    class Factory
    {
    public:

        static T* instance()
        {
            static T m_instance;
            return &m_instance;
        }

    };
} 

