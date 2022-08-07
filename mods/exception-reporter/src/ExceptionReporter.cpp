#include "ExceptionReporter.h"

#include "Pointer.h"

#include "ExceptionReport.h"


ExceptionReporter::ExceptionReporter(HINSTANCE dllInstance)
    :
    Mod(dllInstance)
{
    s_Instance = this;
}

bool ExceptionReporter::Load()
{
    const LPTOP_LEVEL_EXCEPTION_FILTER previousExceptionFilter = ::SetUnhandledExceptionFilter(TopLevelExceptionFilter);
    if (previousExceptionFilter == Utility::Pointer(0x0085CCEC).GetAddress<LPTOP_LEVEL_EXCEPTION_FILTER>())
    {
        return true;
    }

    ::Sleep(1000);
    return false;
}

LONG CALLBACK ExceptionReporter::TopLevelExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo)
{
    const ExceptionReport exceptionReport(ExceptionInfo);
    exceptionReport.Display(s_Instance->GetDllInstance());

    return EXCEPTION_CONTINUE_SEARCH;
}