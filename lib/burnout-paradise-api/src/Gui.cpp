#include "Gui.h"

#include <cstring>


namespace BP {

    void DisplayHudMessage(const char* title, const char* text)
    {
        struct Params
        {
            char Title[64];
            char Text[64];
        };

        Params params = {};
        strcpy_s(params.Title, title);
        strcpy_s(params.Text, text);

        __asm
        {
            lea eax, [params]
            push eax

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x8216D0

            mov eax, 0x0091A4A0
            call eax
        }
    }

    const char* GetLocalizedTranslation(const char* stringID)
    {
        const char* localizedTranslation = nullptr;

        __asm
        {
            push dword ptr [stringID]

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x7A0E34

            mov eax, 0x0089C6D0
            call eax

            mov dword ptr [localizedTranslation], eax
        }

        return localizedTranslation;
    }

}