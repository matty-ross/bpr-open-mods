#include "pch.h"
#include "ImGuiManager.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Pointer.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace Utility {

    ImGuiManager::ImGuiManager(const std::string& iniFilePath)
        :
        m_IniFilePath(iniFilePath)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsClassic();
        
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = m_IniFilePath.c_str();
    }

    ImGuiManager::~ImGuiManager()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiManager::InitBackends()
    {
        const HWND windowHandle = Pointer(0x0139815C).As<HWND>();
        ImGui_ImplWin32_Init(windowHandle);
        
        ID3D11Device* const d3d11Device = Pointer(0x01485BF8).As<ID3D11Device*>();
        ID3D11DeviceContext* const d3d11DeviceContext = Pointer(0x01485ECC).As<ID3D11DeviceContext*>();
        ImGui_ImplDX11_Init(d3d11Device, d3d11DeviceContext);
    }

    void ImGuiManager::AddItem(const ImGuiItem& item)
    {
        m_Mutex.lock();
        m_Items.emplace_back(item);
        m_Mutex.unlock();
    }

    void ImGuiManager::RenderImGui()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        io.MouseDrawCursor = false;

        m_Mutex.lock();
        for (const ImGuiItem& item : m_Items)
        {
            if (item.Visible)
            {
                io.MouseDrawCursor = true;
                item.RenderFunction();
            }
        }
        m_Mutex.unlock();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    std::pair<bool, LRESULT> ImGuiManager::ImGuiWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        const LRESULT result = ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
        if (result)
        {
            return { false, result };
        }

        const ImGuiIO& io = ImGui::GetIO();

        switch (Msg)
        {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MOUSEWHEEL:
            if (io.WantCaptureMouse)
            {
                Pointer(0x013FC2A8).As<float>() = -1.0f;
                Pointer(0x01398244).As<float>() = -1.0f;
                Pointer(0x01398243).As<bool>() = false;
                Pointer(0x01398248).As<float>() = 0.0f;
                return { false, 0 };
            }
            break;

        case WM_KEYDOWN:
            {
                const bool repeat = HIWORD(lParam) & KF_REPEAT;
                if (!repeat)
                {
                    m_Mutex.lock();
                    for (ImGuiItem& item : m_Items)
                    {
                        if (item.ToggleHotkey == wParam)
                        {
                            item.Visible = !item.Visible;
                        }
                    }
                    m_Mutex.unlock();
                }

                Pointer(0x013FC8E0).Field(0x71BF00).As<bool>() = !io.WantCaptureKeyboard;
            }
            break;
        }

        return { true, 0 };
    }

}