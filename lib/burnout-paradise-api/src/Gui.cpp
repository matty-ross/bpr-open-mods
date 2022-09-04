#include "Gui.h"


namespace BP {

    void DisplayHudMessage(const HudMessageParams* params)
    {
        __asm
        {
            push params

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x8216D0

            mov eax, 0x0091A4A0
            call eax
        }
    }

    const uint8_t* GetLocalizedTranslation(const char* stringID)
    {
        const uint8_t* localizedTranslation = nullptr;

        __asm
        {
            push stringID

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x7A0E34

            mov eax, 0x0089C6D0
            call eax

            mov dword ptr [localizedTranslation], eax
        }

        return localizedTranslation;
    }

}