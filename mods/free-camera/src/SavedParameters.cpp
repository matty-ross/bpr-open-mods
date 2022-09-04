#include "SavedParameters.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Pointer.h"


SavedParameters::SavedParameters(const Utility::Logger& logger, const std::string& parametersFilePath)
    :
    m_Logger(logger),
    m_ParametersFilePath(parametersFilePath)
{
    memset(m_DefaultParameters, 0, sizeof(m_DefaultParameters));
}

const std::vector<Parameters>& SavedParameters::GetParameters() const
{
    return m_Parameters;
}

void SavedParameters::LoadParameters()
{
    try
    {
        YAML::Node yaml;
        
        std::ifstream fin(m_ParametersFilePath);
        if (fin)
        {
            yaml = YAML::Load(fin);
        }
        fin.close();
        
        m_Parameters.clear();
        for (const YAML::Node& node : yaml)
        {
            const Parameters parameters =
            {
                .Name = node["Name"].as<std::string>(),
                .PitchSpring = node["PitchSpring"].as<float>(),
                .YawSpring = node["YawSpring"].as<float>(),
                .PivotY = node["PivotY"].as<float>(),
                .PivotZ = node["PivotZ"].as<float>(),
                .FOV = node["FOV"].as<float>(),
                .InFrontFOVMax = node["InFrontFOVMax"].as<float>(),
                .FrontInAmount = node["FrontInAmount"].as<float>(),
                .DriftYawSpring = node["DriftYawSpring"].as<float>(),
                .BoostFOVZoomCompensation = node["BoostFOVZoomCompensation"].as<float>(),
                .DownAngle = node["DownAngle"].as<float>(),
                .DropFactor = node["DropFactor"].as<float>()
            };

            m_Parameters.emplace_back(parameters);
        }
    }
    catch (const YAML::Exception& exception)
    {
        LOG_WARNING(m_Logger, exception.what());
    }
}

void SavedParameters::SaveParameters() const
{
    try
    {
        YAML::Node yaml;
        for (const Parameters& parameters : m_Parameters)
        {
            YAML::Node node;
            node["Name"] = parameters.Name;
            node["PitchSpring"] = parameters.PitchSpring;
            node["YawSpring"] = parameters.YawSpring;
            node["PivotY"] = parameters.PivotY;
            node["PivotZ"] = parameters.PivotZ;
            node["FOV"] = parameters.FOV;
            node["InFrontFOVMax"] = parameters.InFrontFOVMax;
            node["FrontInAmount"] = parameters.FrontInAmount;
            node["DriftYawSpring"] = parameters.DriftYawSpring;
            node["BoostFOVZoomCompensation"] = parameters.BoostFOVZoomCompensation;
            node["DownAngle"] = parameters.DownAngle;
            node["DropFactor"] = parameters.DropFactor;

            yaml.push_back(node);
        }

        std::ofstream fout(m_ParametersFilePath);
        fout << yaml;
        fout.close();
    }
    catch (const YAML::Exception& exception)
    {
        LOG_WARNING(m_Logger, exception.what());
    }
}

void SavedParameters::AddCurrentParameters(const std::string& name)
{
    const Parameters parameters =
    {
        .Name = name,
        .PitchSpring = Utility::Pointer(0x013FC8E0).Field(0x716604).As<float>(),
        .YawSpring = Utility::Pointer(0x013FC8E0).Field(0x716608).As<float>(),
        .PivotY = Utility::Pointer(0x013FC8E0).Field(0x716614).As<float>(),
        .PivotZ = Utility::Pointer(0x013FC8E0).Field(0x716618).As<float>(),
        .FOV = Utility::Pointer(0x013FC8E0).Field(0x716634).As<float>(),
        .InFrontFOVMax = Utility::Pointer(0x013FC8E0).Field(0x716638).As<float>(),
        .FrontInAmount = Utility::Pointer(0x013FC8E0).Field(0x71663C).As<float>(),
        .DriftYawSpring = Utility::Pointer(0x013FC8E0).Field(0x716654).As<float>(),
        .BoostFOVZoomCompensation = Utility::Pointer(0x013FC8E0).Field(0x716658).As<float>(),
        .DownAngle = Utility::Pointer(0x013FC8E0).Field(0x71665C).As<float>(),
        .DropFactor = Utility::Pointer(0x013FC8E0).Field(0x716670).As<float>()
    };

    m_Parameters.emplace_back(parameters);
}

void SavedParameters::SetCurrentParameters(const Parameters& parameters) const
{
    Utility::Pointer(0x013FC8E0).Field(0x716604).As<float>() = parameters.PitchSpring;
    Utility::Pointer(0x013FC8E0).Field(0x716608).As<float>() = parameters.YawSpring;
    Utility::Pointer(0x013FC8E0).Field(0x716614).As<float>() = parameters.PivotY;
    Utility::Pointer(0x013FC8E0).Field(0x716618).As<float>() = parameters.PivotZ;
    Utility::Pointer(0x013FC8E0).Field(0x716634).As<float>() = parameters.FOV;
    Utility::Pointer(0x013FC8E0).Field(0x716638).As<float>() = parameters.InFrontFOVMax;
    Utility::Pointer(0x013FC8E0).Field(0x71663C).As<float>() = parameters.FrontInAmount;
    Utility::Pointer(0x013FC8E0).Field(0x716654).As<float>() = parameters.DriftYawSpring;
    Utility::Pointer(0x013FC8E0).Field(0x716658).As<float>() = parameters.BoostFOVZoomCompensation;
    Utility::Pointer(0x013FC8E0).Field(0x71665C).As<float>() = parameters.DownAngle;
    Utility::Pointer(0x013FC8E0).Field(0x716670).As<float>() = parameters.DropFactor;
}

void SavedParameters::ResetDefaultParameters()
{
    memcpy_s(Utility::Pointer(0x013FC8E0).Field(0x7165C8).GetAddress(), 0xAC, m_DefaultParameters, sizeof(m_DefaultParameters));
}

void SavedParameters::SaveDefaultParameters(void* parameter)
{
    SavedParameters& savedParameters = Utility::Pointer(parameter).As<SavedParameters>();
    memcpy_s(savedParameters.m_DefaultParameters, sizeof(savedParameters.m_DefaultParameters), Utility::Pointer(0x013FC8E0).Field(0x7165C8).GetAddress(), 0xAC);
}