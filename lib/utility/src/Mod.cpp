#include "pch.h"
#include "Mod.h"

#include "Pointer.h"


namespace Utility {

    Mod::Mod(HINSTANCE dllInstance)
        :
        m_DllInstance(dllInstance),
        m_BackgroundWorkerThreadHandle(::CreateThread(nullptr, 0, BackgroundWorkerThreadProc, this, CREATE_SUSPENDED, nullptr)),
        m_TerminateBackgroundWorkerEventHandle(::CreateEventA(nullptr, TRUE, FALSE, nullptr))
    {
        ::DisableThreadLibraryCalls(m_DllInstance);;
    }

    Mod::~Mod()
    {
        ::SetEvent(m_TerminateBackgroundWorkerEventHandle);
        ::WaitForSingleObject(m_BackgroundWorkerThreadHandle, INFINITE);

        ::CloseHandle(m_BackgroundWorkerThreadHandle);
        ::CloseHandle(m_TerminateBackgroundWorkerEventHandle);
    }

    void Mod::Init() const
    {
        ::ResumeThread(m_BackgroundWorkerThreadHandle);
    }

    HINSTANCE Mod::GetDllInstance() const
    {
        return m_DllInstance;
    }

    DWORD CALLBACK Mod::BackgroundWorkerThreadProc(LPVOID lpParameter)
    {
        Mod& mod = Pointer(lpParameter).As<Mod>();
        
        while (!mod.Load())
        {
            if (::WaitForSingleObject(mod.m_TerminateBackgroundWorkerEventHandle, 0) != WAIT_TIMEOUT)
            {
                return 1;
            }
        }
        
        return 0;
    }

}