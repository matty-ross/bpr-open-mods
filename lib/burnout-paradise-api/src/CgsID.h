#pragma once


#include <cstdint>


namespace BP {

    class CgsID
    {
    public:
        CgsID(uint64_t id);
        CgsID(const char* string);

        uint64_t GetCompressed() const;
        const char* GetUncompressed() const;

    private:
        uint64_t m_Compressed;
        char m_Uncompressed[13];
    };

}