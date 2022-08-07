#pragma once


#include <cstdint>


namespace Utility {

    class Pointer
    {
    public:
        template <typename T>
        Pointer(T address)
            :
            m_Address(reinterpret_cast<void*>(address))
        {
        }

        template <typename T = void*>
        inline T& GetAddress()
        {
            return reinterpret_cast<T&>(m_Address);
        }

        template <typename T = void*>
        inline T GetAddress() const
        {
            return reinterpret_cast<T>(m_Address);
        }
        
        template <typename T>
        inline T& As() const
        {
            return *reinterpret_cast<T*>(m_Address);
        }

        inline Pointer At(ptrdiff_t offset) const
        {
            return Pointer(GetAddress<uintptr_t>() + offset);
        }

        inline Pointer Field(ptrdiff_t offset) const
        {
            return Pointer(As<uintptr_t>() + offset);
        }
        
    private:
        void* m_Address;
    };

}