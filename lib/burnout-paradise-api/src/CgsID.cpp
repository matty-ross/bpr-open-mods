#include "CgsID.h"

#include <cstring>


namespace BP {

    CgsID::CgsID(uint64_t id)
    {
        char string[13] = {};
        
        __asm
        {
            push dword ptr [id + 0x4]
            push dword ptr [id + 0x0]
            lea ecx, [string]
            
            mov eax, 0x0086CE00
            call eax
            add esp, 0x8
        }

        m_Compressed = id;
        strcpy_s(m_Uncompressed, string);
    }

    CgsID::CgsID(const char* string)
    {
        uint64_t id = 0;
        
        __asm
        {
            mov ecx, string
            
            mov eax, 0x0086CED0
            call eax

            mov dword ptr [id + 0x0], eax
            mov dword ptr [id + 0x4], edx
        }

        m_Compressed = id;
        strcpy_s(m_Uncompressed, string);
    }

    uint64_t CgsID::GetCompressed() const
    {
        return m_Compressed;
    }

    const char* CgsID::GetUncompressed() const
    {
        return m_Uncompressed;
    }

}