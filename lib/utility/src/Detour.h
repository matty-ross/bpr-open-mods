#pragma once


#include "Pointer.h"


namespace Utility {

    struct DetourConfig
    {
        Pointer HookAddress;
        void(*DetourFunction)();
        bool PreserveFlags;
        bool PreserveRegisters;
    };

    
    class Detour
    {
    public:
        Detour(const DetourConfig& config);
        
        void Install();

    private:
        Pointer AllocateThunk() const;
        void BuildThunk(Pointer thunk, Pointer trampolineAddress) const;
        static uint32_t GetRelativeDisplacement(Pointer instructionAddress, Pointer destinationAddress);

    private:
        DetourConfig m_Config;
    };

}