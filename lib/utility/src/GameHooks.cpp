#include "pch.h"
#include "GameHooks.h"

#include "Exception.h"
#include "Pointer.h"
#include "Detour.h"


namespace Utility {
    
    void GameHooks::SetWindowProcHook(const std::function<std::pair<bool, LRESULT>(HWND, UINT, WPARAM, LPARAM)>& windowProc)
    {
        const HWND windowHandle = Pointer(0x0139815C).As<HWND>();
        s_WindowProc = windowProc;
        
        ::SetLastError(ERROR_SUCCESS);
        const LONG_PTR newValue = Pointer(WindowProcThunk).GetAddress<LONG_PTR>();
        const LONG_PTR previousValue = ::SetWindowLongPtrA(windowHandle, GWLP_WNDPROC, newValue);
        if (
            previousValue == 0 &&
            ::GetLastError() != ERROR_SUCCESS
        )
        {
            throw WINDOWS_EXCEPTION("Failed to hook WindowProc.");
        }

        s_PreviousWindowProc = Pointer(previousValue).GetAddress<WNDPROC>();
    }
    
    void GameHooks::SetPresentHook(const std::function<void()>& present)
    {
        s_Present = present;

        Detour(
            {
                .HookAddress = Pointer(0x0148600C).Field(0x0).Field(0x20).As<void*>(),
                .DetourFunction = PresentThunk,
                .PreserveFlags = true,
                .PreserveRegisters = true
            }
        ).Install();
    }
    
    void GameHooks::SetUpdateHook(const std::function<void()>& update)
    {
        s_Update = update;

        Detour(
            {
                .HookAddress = 0x0673A070,
                .DetourFunction = UpdateThunk,
                .PreserveFlags = true,
                .PreserveRegisters = true
            }
        ).Install();
    }
    
    LRESULT CALLBACK GameHooks::WindowProcThunk(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        auto [passMessage, result] = s_WindowProc(hWnd, Msg, wParam, lParam);
        if (passMessage)
        {
            result = ::CallWindowProcA(s_PreviousWindowProc, hWnd, Msg, wParam, lParam);
        }

        return result;
    }

    void GameHooks::PresentThunk()
    {
        s_Present();
    }

    void GameHooks::UpdateThunk()
    {
        s_Update();
    }

}