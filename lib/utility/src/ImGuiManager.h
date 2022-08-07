#pragma once


#include <string>
#include <vector>
#include <mutex>
#include <functional>

#include <Windows.h>


namespace Utility {

    struct ImGuiItem
    {
        std::function<void()> RenderFunction;
        int ToggleHotkey;
        bool Visible;
    };

    
    class ImGuiManager
    {
    public:
        ImGuiManager(const std::string& iniFilePath);
        ~ImGuiManager();

        static void InitBackends();
        void AddItem(const ImGuiItem& item);
        void RenderImGui();
        std::pair<bool, LRESULT> ImGuiWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    private:
        std::string m_IniFilePath;
        std::vector<ImGuiItem> m_Items;
        std::mutex m_Mutex;
    };

}