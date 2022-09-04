#include "GameEvents.h"


namespace BP {

    bool AddGameEvent(const GameEvent* eventData, int32_t eventType, uint32_t eventDataSize)
    {
        bool added = false;
        
        __asm
        {
            push dword ptr [eventDataSize]
            push dword ptr [eventType]
            push eventData

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x6E0E70

            mov eax, 0x004E3F70
            call eax

            mov byte ptr [added], al
        }

        return added;
    }

}