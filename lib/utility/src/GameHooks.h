#pragma once


#include <functional>

#include <Windows.h>


namespace Utility {

    class GameHooks
    {
    public:
        static void SetWindowProcHook(const std::function<std::pair<bool, LRESULT>(HWND, UINT, WPARAM, LPARAM)>& windowProc);
        static void SetPresentHook(const std::function<void()>& present);
        static void SetUpdateHook(const std::function<void()>& update);

    private:
        static LRESULT CALLBACK WindowProcThunk(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
        static void PresentThunk();
        static void UpdateThunk();

    private:
        inline static std::function<std::pair<bool, LRESULT>(HWND, UINT, WPARAM, LPARAM)> s_WindowProc;
        inline static std::function<void()> s_Present;
        inline static std::function<void()> s_Update;
        inline static WNDPROC s_PreviousWindowProc;
    };

}