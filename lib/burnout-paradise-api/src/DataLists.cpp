#include "DataLists.h"


namespace BP {

    int32_t GetVehicleIndex(uint64_t vehicleID)
    {
        int32_t index = -1;
        
        __asm
        {
            push dword ptr [vehicleID + 0x4]
            push dword ptr [vehicleID + 0x0]
            
            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x68C350

            mov eax, 0x004B70D0
            call eax

            mov dword ptr [index], eax
        }

        return index;
    }

    void* GetVehicleData(uint64_t vehicleID)
    {
        void* data = nullptr;

        __asm
        {
            push dword ptr [vehicleID + 0x4]
            push dword ptr [vehicleID + 0x0]

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x68C350

            mov eax, 0x004B7080
            call eax

            mov dword ptr [data], eax
        }
        
        return data;
    }

    void* GetVehicleDataByIndex(int32_t vehicleIndex)
    {
        void* data = nullptr;

        __asm
        {
            push dword ptr [vehicleIndex]

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x68C350

            mov eax, 0x004B7050
            call eax

            mov dword ptr [data], eax
        }

        return data;
    }

    int32_t GetWheelIndexByName(const char* wheelName)
    {
        int32_t index = -1;

        __asm
        {
            push wheelName

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x68FB68

            mov eax, 0x004E2A10
            call eax

            mov dword ptr [index], eax
        }
        
        return index;
    }

    int32_t GetChallengeIndex(uint64_t challengeID)
    {
        int32_t index = -1;

        __asm
        {
            push dword ptr [challengeID + 0x4]
            push dword ptr [challengeID + 0x0]

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x690B70

            mov eax, 0x004B6F20
            call eax

            mov dword ptr [index], eax
        }

        return index;
    }

    void* GetChallengeData(uint64_t challengeID)
    {
        void* data = nullptr;

        __asm
        {
            push dword ptr [challengeID + 0x4]
            push dword ptr [challengeID + 0x0]

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x690B70

            mov eax, 0x004B6ED0
            call eax

            mov dword ptr [data], eax
        }

        return data;
    }

    void* GetLandmarkData(uint64_t triggerID)
    {
        void* data = nullptr;

        __asm
        {
            push dword ptr [triggerID + 0x4]
            push dword ptr [triggerID + 0x0]

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x6A6880

            mov eax, 0x00BDA200
            call eax

            mov dword ptr [data], eax
        }

        return data;
    }

    void* GetGenericRegionData(uint64_t triggerID)
    {
        void* data = nullptr;

        __asm
        {
            push dword ptr [triggerID + 0x4]
            push dword ptr [triggerID + 0x0]

            mov ecx, dword ptr ds:[0x013FC8E0]
            add ecx, 0x6A6880

            mov eax, 0x00BDA180
            call eax

            mov dword ptr [data], eax
        }

        return data;
    }

}