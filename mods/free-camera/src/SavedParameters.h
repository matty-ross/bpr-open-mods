#pragma once


#include <string>
#include <vector>

#include "Logger.h"


struct Parameters
{
    std::string Name;
    float PitchSpring;
    float YawSpring;
    float PivotY;
    float PivotZ;
    float FOV;
    float InFrontFOVMax;
    float FrontInAmount;
    float DriftYawSpring;
    float BoostFOVZoomCompensation;
    float DownAngle;
    float DropFactor;
};


class SavedParameters
{
public:
    SavedParameters(const Utility::Logger& logger, const std::string& parametersFilePath);

    const std::vector<Parameters>& GetParameters() const;
    void LoadParameters();
    void SaveParameters() const;
    void AddCurrentParameters(const std::string& name);
    void SetCurrentParameters(const Parameters& parameters) const;
    static void ResetDefaultParameters();
    static void SaveDefaultParameters();

private:
    const Utility::Logger& m_Logger;
    std::string m_ParametersFilePath;
    std::vector<Parameters> m_Parameters;
    inline static uint8_t s_DefaultParameters[0xAC];
};