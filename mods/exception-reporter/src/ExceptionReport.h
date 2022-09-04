#pragma once


#include <string>

#include <Windows.h>


enum class RegisterName
{
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    EBP,
    ESP,
    EIP
};


class ExceptionReport
{
public:
    ExceptionReport(const EXCEPTION_POINTERS* exceptionPointers);

    void Display(HINSTANCE dllInstance) const;
    std::string GetCode() const;
    std::string GetDescription() const;
    std::string GetAddress() const;
    std::string GetModuleName() const;
    std::string GetRegisterValue(RegisterName registerName) const;
    std::string GetAdditionalInformation() const;
    std::string GetStackTrace() const;

private:
    HMODULE GetModule() const;
    static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

private:
    const EXCEPTION_RECORD& m_ExceptionRecord;
    const CONTEXT& m_ContextRecord;
};