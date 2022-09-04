#pragma once


#include <string>

#include "BlacklistedPlayers.h"


class Gui
{
public:
    Gui(BlacklistedPlayers& blacklistedPlayers);

    void Render(const std::string& title);

private:
    void RenderMisc();
    void RenderPlayers();
    void RenderBlacklistedPlayers();

private:
    BlacklistedPlayers& m_BlacklistedPlayers;
};