#include "AutoActions.h"

#include "Pointer.h"

#include "BlacklistedPlayers.h"


struct PlayerOption
{
    int32_t Option;
    uint64_t PlayerID;
};


void PerformPlayerOption(const PlayerOption* playerOption)
{
    __asm
    {
        push playerOption

        mov ecx, dword ptr ds:[0x013FC8E0]
        add ecx, 0x7FABDC

        mov eax, 0x004FB4C0
        call eax
    }
}

void __stdcall AutoActions(void* playerData, BlacklistedPlayers* blacklistedPlayers)
{
    if (!g_BullyRepellentEnabled)
    {
        return;
    }
    
    // autokick
    const int32_t gameMode = Utility::Pointer(0x013FC8E0).Field(0x69D58C).As<int32_t>();
    if (
        gameMode == 15 ||
        gameMode == 16
    )
    {   
        if (
            !Utility::Pointer(0x013FC8E0).Field(0x8EFF36).As<bool>() && // isn't ranked match
            Utility::Pointer(0x013FC8E0).Field(0x8F6E89).As<bool>() && // is local player host
            !Utility::Pointer(playerData).At(0x12D).As<bool>() // isn't local player
        )
        {
            const uint64_t playerID = Utility::Pointer(playerData).At(0x110).As<uint64_t>();
            const Player* const player = blacklistedPlayers->GetPlayer(playerID);
            if (
                player != nullptr &&
                player->Autokick
            )
            {
                const PlayerOption playerOption =
                {
                    .Option = 2,
                    .PlayerID = playerID
                };
                PerformPlayerOption(&playerOption);
                return;
            }
        }
    }

    // automute
    if (
        Utility::Pointer(playerData).At(0x11C).As<int32_t>() != 0 && // has headset
        !Utility::Pointer(playerData).At(0x130).As<bool>() && // isn't muted
        !Utility::Pointer(playerData).At(0x12D).As<bool>() // isn't local player
    )
    {
        const uint64_t playerID = Utility::Pointer(playerData).At(0x110).As<uint64_t>();
        const Player* const player = blacklistedPlayers->GetPlayer(playerID);
        if (
            player != nullptr &&
            player->Automute
        )
        {
            const PlayerOption playerOption =
            {
                .Option = 3,
                .PlayerID = playerID
            };
            PerformPlayerOption(&playerOption);
            return;
        }
    }
}

__declspec(naked) void AutoActionsThunk(void* parameter)
{
    __asm
    {
        push dword ptr [esp + 0x4]
        push eax
        
        call AutoActions
        
        ret
    }
}