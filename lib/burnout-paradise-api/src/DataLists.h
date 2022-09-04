#pragma once


#include <cstdint>


namespace BP {

    int32_t GetVehicleIndex(uint64_t vehicleID);
    void* GetVehicleData(uint64_t vehicleID);
    void* GetVehicleDataByIndex(int32_t vehicleIndex);

    int32_t GetWheelIndexByName(const char* wheelName);
    
    int32_t GetChallengeIndex(uint64_t challengeID);
    void* GetChallengeData(uint64_t challengeID);

    void* GetLandmarkData(uint64_t triggerID);
    void* GetGenericRegionData(uint64_t triggerID);

}