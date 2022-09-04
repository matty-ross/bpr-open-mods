#include "ExceptionReporter.h"

#include "Pointer.h"

#include "ExceptionReport.h"


ExceptionReporter::ExceptionReporter(HINSTANCE dllInstance)
    :
    Mod(dllInstance),
    m_PreviousExceptionFilter(nullptr)
{
    s_Instance = this;
}

ExceptionReporter::~ExceptionReporter()
{
    ::SetUnhandledExceptionFilter(m_PreviousExceptionFilter);
}

bool ExceptionReporter::Load()
{
    const HANDLE mutexHandle = ::OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "BurnoutParadiseexe");
    if (mutexHandle != nullptr)
    {
        m_PreviousExceptionFilter = ::SetUnhandledExceptionFilter(TopLevelExceptionFilter);
        ::CloseHandle(mutexHandle);
        return true;
    }
    
    ::Sleep(100);
    return false;
}

LONG CALLBACK ExceptionReporter::TopLevelExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo)
{
    const ExceptionReport exceptionReport(ExceptionInfo);
    exceptionReport.Display(s_Instance->GetDllInstance());

    return s_Instance->m_PreviousExceptionFilter(ExceptionInfo);
}