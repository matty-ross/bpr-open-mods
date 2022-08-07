#include <Windows.h>

#include "ExceptionReporter.h"


static ExceptionReporter* s_Mod = nullptr;


BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        s_Mod = new ExceptionReporter(hinstDLL);
        s_Mod->Init();
        break;
    
    case DLL_PROCESS_DETACH:
        delete s_Mod;
        break;
    }

    return TRUE;
}