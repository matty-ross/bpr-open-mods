#include "pch.h"
#include "Detour.h"

#include "detours.h"

#include "Exception.h"


namespace Utility {
    
    Detour::Detour(const DetourConfig& config)
        :
        m_Config(config)
    {
    }

    void Detour::Install()
    {
        LONG lastError = NO_ERROR;
        
        lastError = DetourTransactionBegin();
        if (lastError != NO_ERROR)
        {
            ::SetLastError(lastError);
            throw WINDOWS_EXCEPTION("Failed to begin a transaction for a detour at 0x{:08X}.", m_Config.HookAddress.GetAddress<uintptr_t>());
        }

        void* pointer = m_Config.HookAddress.GetAddress();
        const Pointer thunk = AllocateThunk();
        DETOUR_TRAMPOLINE* trampoline = nullptr;
        
        lastError = DetourAttachEx(&pointer, thunk.GetAddress(), &trampoline, nullptr, nullptr);
        if (lastError != NO_ERROR)
        {
            ::SetLastError(lastError);
            throw WINDOWS_EXCEPTION("Failed to attach a detour function to a detour at 0x{:08X}.", m_Config.HookAddress.GetAddress<uintptr_t>());
        }

        BuildThunk(thunk, trampoline);

        lastError = DetourTransactionCommit();
        if (lastError != NO_ERROR)
        {
            ::SetLastError(lastError);
            throw WINDOWS_EXCEPTION("Failed to commit a transaction for a detour at 0x{:08X}.", m_Config.HookAddress.GetAddress<uintptr_t>());
        }
    }

    Pointer Detour::AllocateThunk() const
    {
        void* const thunk = ::VirtualAlloc(nullptr, 0x10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (thunk == nullptr)
        {
            throw WINDOWS_EXCEPTION("Failed to allocate a detour thunk for a detour at 0x{:08X}.", m_Config.HookAddress.GetAddress<uintptr_t>());
        }

        return thunk;
    }

    void Detour::BuildThunk(Pointer thunk, Pointer trampolineAddress) const
    {
        // save flags
        if (m_Config.PreserveFlags)
        {
            // pushfd
            thunk.At(0x0).As<uint8_t>() = 0x9C;

            thunk.GetAddress<uint8_t*>() += 1;
        }

        // save registers
        if (m_Config.PreserveRegisters)
        {
            // pushad
            thunk.At(0x0).As<uint8_t>() = 0x60;

            thunk.GetAddress<uint8_t*>() += 1;
        }

        // call detour function
        {
            // call m_Config.DetourFunction
            thunk.At(0x0).As<uint8_t>() = 0xE8;
            thunk.At(0x1).As<uint32_t>() = GetRelativeDisplacement(thunk, m_Config.DetourFunction);

            thunk.GetAddress<uint8_t*>() += 5;
        }

        // restore registers
        if (m_Config.PreserveRegisters)
        {
            // popad
            thunk.At(0x0).As<uint8_t>() = 0x61;

            thunk.GetAddress<uint8_t*>() += 1;
        }

        // restore flags
        if (m_Config.PreserveFlags)
        {
            // popfd
            thunk.At(0x0).As<uint8_t>() = 0x9D;

            thunk.GetAddress<uint8_t*>() += 1;
        }

        // jump to the trampoline
        {
            // jmp trampolineAddress
            thunk.At(0x0).As<uint8_t>() = 0xE9;
            thunk.At(0x1).As<uint32_t>() = GetRelativeDisplacement(thunk, trampolineAddress);

            thunk.GetAddress<uint8_t*>() += 5;
        }
    }

    uint32_t Detour::GetRelativeDisplacement(Pointer instructionAddress, Pointer destinationAddress)
    {
        return destinationAddress.GetAddress<uintptr_t>() - instructionAddress.GetAddress<uintptr_t>() - 0x5;
    }

}