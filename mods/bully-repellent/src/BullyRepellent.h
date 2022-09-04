#pragma once


#include "Mod.h"
#include "Logger.h"
#include "ImGuiManager.h"

#include "BlacklistedPlayers.h"
#include "Gui.h"


class BullyRepellent : public Utility::Mod
{
public:
    BullyRepellent(HINSTANCE dllInstance);

private:
    enum class LoadStage
    {
        Init,
        WaitForGameInit,
        InitMod,
        InitHooks,
        WaitForInGame,
        InitImGui,
        Done
    };

    bool Load() override;
    void OnInit() const;
    bool OnWaitForGameInit() const;
    void OnInitMod();
    void OnInitHooks();
    bool OnWaitForInGame() const;
    void OnInitImGui();
    void OnDone() const;
    void HandleException(const std::string& message) const;

private:
    Utility::Logger m_Logger;
    Utility::ImGuiManager m_ImGuiManager;
    BlacklistedPlayers m_BlacklistedPlayers;
    Gui m_Gui;
    LoadStage m_LoadStage;
};