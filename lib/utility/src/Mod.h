#pragma once


#include <Windows.h>


namespace Utility {
    
    class Mod
    {
    public:
        Mod(HINSTANCE dllInstance);
        virtual ~Mod();

        void Init() const;
        HINSTANCE GetDllInstance() const;

    private:
        virtual bool Load() = 0;
        static DWORD CALLBACK BackgroundWorkerThreadProc(LPVOID lpParameter);

    private:
        HINSTANCE m_DllInstance;
        HANDLE m_BackgroundWorkerThreadHandle;
        HANDLE m_TerminateBackgroundWorkerEventHandle;
    };

}