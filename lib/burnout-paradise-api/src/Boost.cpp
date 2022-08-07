#include "Boost.h"


namespace BP {
    
    bool IsBoosting()
    {
        bool boosting = false;

        __asm
        {
            mov ecx, dword ptr ds:[0x013FC8E0]
            mov ecx, [ecx + 0x40750]

            mov eax, [ecx]
            call dword ptr [eax + 0x50]

            mov byte ptr [boosting], al
        }

        return boosting;
    }
    
    float GetBoostAmount()
    {
        float boostAmount = 0.0f;
        
        __asm
        {
            mov ecx, dword ptr ds:[0x013FC8E0]
            mov ecx, [ecx + 0x40750]

            mov eax, [ecx]
            call dword ptr [eax + 0x78]

            fstp dword ptr [boostAmount]
        }

        return boostAmount;
    }

    float GetMaxBoost()
    {
        float maxBoost = 0.0f;

        __asm
        {
            mov ecx, dword ptr ds:[0x013FC8E0]
            mov ecx, [ecx + 0x40750]

            mov eax, [ecx]
            call dword ptr [eax + 0x7C]

            fstp dword ptr [maxBoost]
        }

        return maxBoost;
    }

    void SetInfiniteBoost(bool infiniteBoost)
    {
        __asm
        {
            movzx eax, byte ptr [infiniteBoost]
            push eax

            mov ecx, dword ptr ds:[0x013FC8E0]
            mov ecx, [ecx + 0x40750]

            mov eax, [ecx]
            call dword ptr [eax + 0x88]
        }
    }

    void SetBoostAmount(float boostAmount)
    {
        __asm
        {
            push dword ptr [boostAmount]
            
            mov ecx, dword ptr ds:[0x013FC8E0]
            mov ecx, [ecx + 0x40750]

            mov eax, [ecx]
            call dword ptr [eax + 0xAC]
        }
    }

    void SetBoostEarningEnabled(bool boostEarningEnabled)
    {
        __asm
        {
            movzx eax, byte ptr [boostEarningEnabled]
            push eax
            
            mov ecx, dword ptr ds:[0x013FC8E0]
            mov ecx, [ecx + 0x40750]

            mov eax, [ecx]
            call dword ptr [eax + 0xC0]
        }
    }

    void RefillBoost()
    {
        __asm
        {
            mov ecx, dword ptr ds:[0x013FC8E0]
            mov ecx, [ecx + 0x40750]

            mov eax, [ecx]
            call dword ptr [eax + 0xD4]
        }
    }

}