#include "BlacklistedPlayers.h"

#include <fstream>

#include "yaml-cpp/yaml.h"


BlacklistedPlayers::BlacklistedPlayers(const Utility::Logger& logger, const std::string& playersFilePath)
    :
    m_Logger(logger),
    m_PlayersFilePath(playersFilePath)
{
}

std::vector<Player>& BlacklistedPlayers::GetPlayers()
{
    return m_Players;
}

Player* BlacklistedPlayers::GetPlayer(uint64_t id)
{
    for (Player& player : m_Players)
    {
        if (player.ID == id)
        {
            return &player;
        }
    }

    return nullptr;
}

void BlacklistedPlayers::LoadPlayers()
{
    try
    {
        YAML::Node yaml;

        std::ifstream fin(m_PlayersFilePath);
        if (fin)
        {
            yaml = YAML::Load(fin);
        }
        fin.close();

        m_Players.clear();
        for (const YAML::Node& node : yaml)
        {
            const Player player =
            {
                .ID = node["ID"].as<uint64_t>(),
                .Name = node["Name"].as<std::string>(),
                .Autokick = node["Autokick"].as<bool>(),
                .Automute = node["Automute"].as<bool>()
            };

            m_Players.emplace_back(player);
        }
    }
    catch (const YAML::Exception& exception)
    {
        LOG_WARNING(m_Logger, exception.what());
    }
}

void BlacklistedPlayers::SavePlayers() const
{
    try
    {
        YAML::Node yaml;
        for (const Player& player : m_Players)
        {
            YAML::Node node;
            node["ID"] = player.ID;
            node["Name"] = player.Name;
            node["Autokick"] = player.Autokick;
            node["Automute"] = player.Automute;

            yaml.push_back(node);
        }

        std::ofstream fout(m_PlayersFilePath);
        fout << yaml;
        fout.close();
    }
    catch (const YAML::Exception& exception)
    {
        LOG_WARNING(m_Logger, exception.what());
    }
}

void BlacklistedPlayers::AddPlayer(const Player& player)
{
    m_Players.emplace_back(player);
}