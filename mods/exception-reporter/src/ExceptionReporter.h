#pragma once


#include <Windows.h>

#include "Mod.h"


class ExceptionReporter : public Utility::Mod
{
public:
    ExceptionReporter(HINSTANCE dllInstance);

private:
    bool Load() override;
    static LONG CALLBACK TopLevelExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo);

private:
    inline static ExceptionReporter* s_Instance = nullptr;
};