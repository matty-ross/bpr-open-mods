#pragma once


namespace BP {

    bool IsBoosting();
    float GetBoostAmount();
    float GetMaxBoost();
    void SetInfiniteBoost(bool infiniteBoost);
    void SetBoostAmount(float boostAmount);
    void SetBoostEarningEnabled(bool boostEarningEnabled);
    void RefillBoost();

}