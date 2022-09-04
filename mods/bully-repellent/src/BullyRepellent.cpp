#include "BullyRepellent.h"

#include "Exception.h"
#include "Pointer.h"
#include "Detour.h"
#include "GameHooks.h"

#include "AutoActions.h"


static const std::string k_ModTitle = "Bully Repellent";
static const std::string k_ModDirectory = ".\\mods\\bully-repellent\\";


BullyRepellent::BullyRepellent(HINSTANCE dllInstance)
    :
    Mod(dllInstance),
    m_Logger(k_ModDirectory + "log.txt"),
    m_ImGuiManager(k_ModDirectory + "imgui.ini"),
    m_BlacklistedPlayers(m_Logger, k_ModDirectory + "blacklisted-players.yaml"),
    m_Gui(m_BlacklistedPlayers),
    m_LoadStage(LoadStage::Init)
{
}

bool BullyRepellent::Load()
{
    try
    {
        switch (m_LoadStage)
        {
        case LoadStage::Init:
            OnInit();
            m_LoadStage = LoadStage::WaitForGameInit;
            break;

        case LoadStage::WaitForGameInit:
            if (OnWaitForGameInit())
            {
                m_LoadStage = LoadStage::InitMod;
            }
            break;

        case LoadStage::InitMod:
            OnInitMod();
            m_LoadStage = LoadStage::InitHooks;
            break;

        case LoadStage::InitHooks:
            OnInitHooks();
            m_LoadStage = LoadStage::WaitForInGame;
            break;

        case LoadStage::WaitForInGame:
            if (OnWaitForInGame())
            {
                m_LoadStage = LoadStage::InitImGui;
            }
            break;

        case LoadStage::InitImGui:
            OnInitImGui();
            m_LoadStage = LoadStage::Done;
            break;

        case LoadStage::Done:
            OnDone();
            return true;
        }
    }
    catch (const Utility::Exception& exception)
    {
        HandleException(exception.GetSummary());
        return true;
    }
    catch (const std::exception& exception)
    {
        HandleException(exception.what());
        return true;
    }
    catch (...)
    {
        HandleException("Unknown exception.");
        return true;
    }

    return false;
}

void BullyRepellent::OnInit() const
{
    if (
        ::CreateDirectoryA(k_ModDirectory.c_str(), nullptr) == FALSE &&
        ::GetLastError() != ERROR_ALREADY_EXISTS
    )
    {
        throw WINDOWS_EXCEPTION("Failed to create a mod directory '{}'.", k_ModDirectory);
    }
}

bool BullyRepellent::OnWaitForGameInit() const
{
    if (Utility::Pointer(0x013FC8E0).As<void*>() != nullptr)
    {
        LOG_INFO(m_Logger, "Game initialized.");
        return true;
    }

    ::Sleep(100);
    return false;
}

void BullyRepellent::OnInitMod()
{
    LOG_INFO(m_Logger, "Initializing mod...");

    Utility::ImGuiManager::InitBackends();
    m_BlacklistedPlayers.LoadPlayers();

    LOG_INFO(m_Logger, "Mod initialized.");
}

void BullyRepellent::OnInitHooks()
{
    LOG_INFO(m_Logger, "Initializing hooks...");

    Utility::GameHooks::SetWindowProcHook(
        [this](HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
        {
            return m_ImGuiManager.ImGuiWindowProc(hWnd, Msg, wParam, lParam);
        }
    );

    Utility::GameHooks::SetPresentHook(
        [this]()
        {
            m_ImGuiManager.RenderImGui();
        }
    );

    Utility::Detour(
        {
            .HookAddress = 0x0092BF0C,
            .DetourFunction = AutoActionsThunk,
            .Parameter = &m_BlacklistedPlayers,
            .PreserveFlags = true,
            .PreserveRegisters = true
        }
    ).Install();

    LOG_INFO(m_Logger, "Hooks initialized.");
}

bool BullyRepellent::OnWaitForInGame() const
{
    if (Utility::Pointer(0x013FC8E0).Field(0xB6D4CE).As<bool>())
    {
        LOG_INFO(m_Logger, "In Game.");
        return true;
    }

    ::Sleep(1000);
    return false;
}

void BullyRepellent::OnInitImGui()
{
    LOG_INFO(m_Logger, "Initializing ImGui...");

    m_ImGuiManager.AddItem(
        {
            .RenderFunction = [this]() { m_Gui.Render(k_ModTitle); },
            .ToggleHotkey = VK_F8,
            .Visible = false
        }
    );

    LOG_INFO(m_Logger, "ImGui initialized.");
}

void BullyRepellent::OnDone() const
{
    LOG_INFO(m_Logger, "Mod done loading.");
}

void BullyRepellent::HandleException(const std::string& message) const
{
    LOG_ERROR(m_Logger, message);
    ::MessageBoxA(nullptr, message.c_str(), k_ModTitle.c_str(), MB_ICONERROR);
}