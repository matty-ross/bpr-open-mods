#pragma once


#include <string>
#include <vector>

#include "Logger.h"


struct Player
{
    uint64_t ID;
    std::string Name;
    bool Autokick;
    bool Automute;
};


class BlacklistedPlayers
{
public:
    BlacklistedPlayers(const Utility::Logger& logger, const std::string& playersFilePath);

    std::vector<Player>& GetPlayers();
    Player* GetPlayer(uint64_t id);
    void LoadPlayers();
    void SavePlayers() const;
    void AddPlayer(const Player& player);

private:
    const Utility::Logger& m_Logger;
    std::string m_PlayersFilePath;
    std::vector<Player> m_Players;
};