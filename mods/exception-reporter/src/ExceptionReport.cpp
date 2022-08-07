#include "ExceptionReport.h"

#include <string>
#include <format>

#include <Windows.h>

#include "Pointer.h"

#include "../rsc/resource.h"


ExceptionReport::ExceptionReport(const EXCEPTION_POINTERS* exceptionPointers)
    :
    m_ExceptionRecord(*(exceptionPointers->ExceptionRecord)),
    m_ContextRecord(*(exceptionPointers->ContextRecord))
{
}

void ExceptionReport::Display(HINSTANCE dllInstance) const
{
    const LPARAM parameter = reinterpret_cast<LPARAM>(this);
    
    DialogBoxParamA(dllInstance, MAKEINTRESOURCEA(IDD_EXCEPTION_REPORT_DIALOG), nullptr, DialogProc, parameter);
}

std::string ExceptionReport::GetCode() const
{
    return std::format("0x{:08X}", m_ExceptionRecord.ExceptionCode);
}

std::string ExceptionReport::GetDescription() const
{
    switch (m_ExceptionRecord.ExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION:
        return "The thread tried to read from or write to a virtual address for which it does not have the appropriate access.";

    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        return "The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.";

    case EXCEPTION_BREAKPOINT:
        return "A breakpoint was encountered.";

    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return "The thread tried to read or write data that is misaligned on hardware that does not provide alignment.";

    case EXCEPTION_FLT_DENORMAL_OPERAND:
        return "One of the operands in a floating-point operation is denormal.";

    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return "The thread tried to divide a floating-point value by a floating-point divisor of zero.";

    case EXCEPTION_FLT_INEXACT_RESULT:
        return "The result of a floating-point operation cannot be represented exactly as a decimal fraction.";

    case EXCEPTION_FLT_INVALID_OPERATION:
        return "An invalid floating-point operation.";

    case EXCEPTION_FLT_OVERFLOW:
        return "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.";

    case EXCEPTION_FLT_STACK_CHECK:
        return "The stack overflowed or underflowed as the result of a floating-point operation.";

    case EXCEPTION_FLT_UNDERFLOW:
        return "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.";

    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return "The thread tried to execute an invalid instruction.";

    case EXCEPTION_IN_PAGE_ERROR:
        return "The thread tried to access a page that was not present, and the system was unable to load the page.";

    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return "The thread tried to divide an integer value by an integer divisor of zero.";

    case EXCEPTION_INT_OVERFLOW:
        return "The result of an integer operation caused a carry out of the most significant bit of the result.";

    case EXCEPTION_INVALID_DISPOSITION:
        return "An exception handler returned an invalid disposition to the exception dispatcher.";

    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        return "The thread tried to continue execution after a noncontinuable exception occurred.";

    case EXCEPTION_PRIV_INSTRUCTION:
        return "The thread tried to execute an instruction whose operation is not allowed in the current machine mode.";

    case EXCEPTION_SINGLE_STEP:
        return "A trace trap or other single-instruction mechanism signaled that one instruction has been executed.";

    case EXCEPTION_STACK_OVERFLOW:
        return "The thread used up its stack.";
    }
    
    return std::string();
}

std::string ExceptionReport::GetAddress() const
{
    const HMODULE module = GetModule();
    if (module == nullptr)
    {
        return std::string();
    }

    const Utility::Pointer address = m_ExceptionRecord.ExceptionAddress;
    const Utility::Pointer baseAddress = module;
    const ptrdiff_t rva = address.GetAddress<uintptr_t>() - baseAddress.GetAddress<uintptr_t>();

    return std::format("0x{:08X} (RVA: 0x{:08X})", address.GetAddress<uintptr_t>(), rva);
}

std::string ExceptionReport::GetModuleName() const
{
    const HMODULE module = GetModule();
    if (module == nullptr)
    {
        return std::string();
    }

    char filePath[MAX_PATH] = {};
    ::GetModuleFileNameA(module, filePath, sizeof(filePath));

    const std::string moduleName = filePath;
    return moduleName.substr(moduleName.find_last_of("/\\") + 1);
}

std::string ExceptionReport::GetRegisterValue(RegisterName registerName) const
{
    switch (registerName)
    {
    case RegisterName::EAX:
        return std::format("0x{:08X}", m_ContextRecord.Eax);

    case RegisterName::EBX:
        return std::format("0x{:08X}", m_ContextRecord.Ebx);

    case RegisterName::ECX:
        return std::format("0x{:08X}", m_ContextRecord.Ecx);

    case RegisterName::EDX:
        return std::format("0x{:08X}", m_ContextRecord.Edx);

    case RegisterName::ESI:
        return std::format("0x{:08X}", m_ContextRecord.Esi);

    case RegisterName::EDI:
        return std::format("0x{:08X}", m_ContextRecord.Edi);

    case RegisterName::EBP:
        return std::format("0x{:08X}", m_ContextRecord.Ebp);

    case RegisterName::ESP:
        return std::format("0x{:08X}", m_ContextRecord.Esp);

    case RegisterName::EIP:
        return std::format("0x{:08X}", m_ContextRecord.Eip);
    }

    return std::string();
}

std::string ExceptionReport::GetAdditionalInformation() const
{
    std::string additionalInformation = "";
    for (size_t i = 0; i < m_ExceptionRecord.NumberParameters; ++i)
    {
        additionalInformation += std::format("0x{:08X}\n", m_ExceptionRecord.ExceptionInformation[i]);
    }

    return additionalInformation;
}

std::string ExceptionReport::GetStackTrace() const
{
    std::string stackTrace = "";

    struct StackFrame
    {
        StackFrame* PreviousStackFrame;
        void* ReturnAddress;
    };

    const StackFrame* stackFrame = Utility::Pointer(m_ContextRecord.Ebp).GetAddress<StackFrame*>();
    bool done = false;
    
    auto getStackFrame = [&]()
    {
        __try
        {
            char buffer[16] = {};
            sprintf_s(buffer, "0x%p\n", stackFrame->ReturnAddress);
            
            stackTrace += buffer;
            stackFrame = stackFrame->PreviousStackFrame;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            done = true;
        }
    };
    
    while (!done)
    {
        getStackFrame();
    }

    return stackTrace;
}

HMODULE ExceptionReport::GetModule() const
{
    HMODULE module = nullptr;
    const LPCSTR address = static_cast<LPCSTR>(m_ExceptionRecord.ExceptionAddress);
    ::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, address, &module);

    return module;
}

INT_PTR CALLBACK ExceptionReport::DialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_INITDIALOG:
        {
            const ExceptionReport& instance = Utility::Pointer(lParam).As<ExceptionReport>();
            ::SetDlgItemTextA(hWnd, IDC_CODE_VALUE, instance.GetCode().c_str());
            ::SetDlgItemTextA(hWnd, IDC_DESCRIPTION_VALUE, instance.GetDescription().c_str());
            ::SetDlgItemTextA(hWnd, IDC_ADDRESS_VALUE, instance.GetAddress().c_str());
            ::SetDlgItemTextA(hWnd, IDC_MODULE_NAME_VALUE, instance.GetModuleName().c_str());
            ::SetDlgItemTextA(hWnd, IDC_EAX_VALUE, instance.GetRegisterValue(RegisterName::EAX).c_str());
            ::SetDlgItemTextA(hWnd, IDC_EBX_VALUE, instance.GetRegisterValue(RegisterName::EBX).c_str());
            ::SetDlgItemTextA(hWnd, IDC_ECX_VALUE, instance.GetRegisterValue(RegisterName::ECX).c_str());
            ::SetDlgItemTextA(hWnd, IDC_EDX_VALUE, instance.GetRegisterValue(RegisterName::EDX).c_str());
            ::SetDlgItemTextA(hWnd, IDC_ESI_VALUE, instance.GetRegisterValue(RegisterName::ESI).c_str());
            ::SetDlgItemTextA(hWnd, IDC_EDI_VALUE, instance.GetRegisterValue(RegisterName::EDI).c_str());
            ::SetDlgItemTextA(hWnd, IDC_EBP_VALUE, instance.GetRegisterValue(RegisterName::EBP).c_str());
            ::SetDlgItemTextA(hWnd, IDC_ESP_VALUE, instance.GetRegisterValue(RegisterName::ESP).c_str());
            ::SetDlgItemTextA(hWnd, IDC_EIP_VALUE, instance.GetRegisterValue(RegisterName::EIP).c_str());
            ::SetDlgItemTextA(hWnd, IDC_ADDITIONAL_INFORMATION_VALUE, instance.GetAdditionalInformation().c_str());
            ::SetDlgItemTextA(hWnd, IDC_STACK_TRACE_VALUE, instance.GetStackTrace().c_str());
        }
        return TRUE;

    case WM_CLOSE:
        ::EndDialog(hWnd, 0);
        return TRUE;
    }

    return FALSE;
}