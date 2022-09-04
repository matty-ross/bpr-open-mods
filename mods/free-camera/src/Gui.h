#pragma once


#include <string>

#include "SavedParameters.h"
#include "ManualController.h"


class Gui
{
public:
    Gui(SavedParameters& savedParameters, ManualController& manualController);

    void Render(const std::string& title);

private:
    void RenderMisc();
    void RenderBehavior();
    void RenderParameters();
    void RenderManualController();

private:
    SavedParameters& m_SavedParameters;
    ManualController& m_ManualController;
};