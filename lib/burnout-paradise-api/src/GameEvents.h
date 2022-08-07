#pragma once


#include <cstdint>


namespace BP {

    struct GameEvent
    {
    };


    bool AddGameEvent(const GameEvent* eventData, int32_t eventType, uint32_t eventDataSize);


    struct SetupVehicleEvent : public GameEvent
    {
        float Position[4];
        float Direction[4];
        uint64_t VehicleID;
        uint64_t WheelID;

        bool Add()
        {
            return AddGameEvent(this, 0, sizeof(*this));
        }
    };
    
    
    struct TeleportVehicleEvent : public GameEvent
    {
        float Position[4];
        float Direction[4];

        bool Add()
        {
            return AddGameEvent(this, 1, sizeof(*this));
        }
    };
    
    
    struct ChangeVehicleEvent : public GameEvent
    {
        uint64_t VehicleID;
        uint64_t WheelID;
        bool ResetCamera;
        bool KeepResetSection;

        bool Add()
        {
            return AddGameEvent(this, 2, sizeof(*this));
        }
    };

}