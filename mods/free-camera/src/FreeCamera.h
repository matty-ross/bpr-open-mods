#pragma once


#include "Mod.h"
#include "Logger.h"
#include "ImGuiManager.h"

#include "SavedParameters.h"
#include "ManualController.h"
#include "Gui.h"


class FreeCamera : public Utility::Mod
{
public:
    FreeCamera(HINSTANCE dllInstance);

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

private:
    Utility::Logger m_Logger;
    Utility::ImGuiManager m_ImGuiManager;
    SavedParameters m_SavedParameters;
    ManualController m_ManualController;
    Gui m_Gui;
    LoadStage m_LoadStage;
};