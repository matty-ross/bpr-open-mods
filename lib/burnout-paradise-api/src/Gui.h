#pragma once


#include <cstdint>


namespace BP {

    struct HudMessageParams
    {
        char Title[64];
        char Text[64];
    };

    
    void DisplayHudMessage(const HudMessageParams* params);
    
    const uint8_t* GetLocalizedTranslation(const char* stringID);

}