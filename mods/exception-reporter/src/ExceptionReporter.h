#pragma once


#include "Mod.h"


class ExceptionReporter : public Utility::Mod
{
public:
    ExceptionReporter(HINSTANCE dllInstance);
    ~ExceptionReporter() override;

private:
    bool Load() override;
    static LONG CALLBACK TopLevelExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo);

private:
    inline static const ExceptionReporter* s_Instance = nullptr;
    LPTOP_LEVEL_EXCEPTION_FILTER m_PreviousExceptionFilter;
};