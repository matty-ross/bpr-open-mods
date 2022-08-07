#include "pch.h"
#include "Exception.h"


namespace Utility {
    
    Exception::Exception(const std::string& file, int line, const std::string& function, const std::string& description)
        :
        m_File(file.substr(file.find_last_of("/\\") + 1)),
        m_Line(line),
        m_Function(function),
        m_Description(description),
        m_Details("No additional details.")
    {
    }

    std::string Exception::GetSummary() const
    {
        std::stringstream ss;
        
        ss << "[File] " << GetFile() << '\n';
        ss << "[Line] " << GetLine() << '\n';
        ss << "[Function] " << GetFunction() << '\n';
        ss << '\n';
        ss << "[Description] " << GetDescription() << '\n';
        ss << '\n';
        ss << "[Details] " << '\n';
        ss << GetDetails();

        return ss.str();
    }

    const std::string& Exception::GetFile() const
    {
        return m_File;
    }

    int Exception::GetLine() const
    {
        return m_Line;
    }

    const std::string& Exception::GetFunction() const
    {
        return m_Function;
    }

    const std::string& Exception::GetDescription() const
    {
        return m_Description;
    }

    const std::string& Exception::GetDetails() const
    {
        return m_Details;
    }

    
    WindowsException::WindowsException(const std::string& file, int line, const std::string& function, uint32_t lastError, const std::string& description)
        :
        Exception(file, line, function, description),
        m_LastError(lastError)
    {
        std::stringstream ss;
        
        ss << "[Error Code] " << GetErrorCode() << '\n';
        ss << "[Error Message] " << GetErrorMessage();

        m_Details = ss.str();
    }

    uint32_t WindowsException::GetErrorCode() const
    {
        return m_LastError;
    }

    std::string WindowsException::GetErrorMessage() const
    {
        char buffer[1024] = {};
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, m_LastError, 0, buffer, sizeof(buffer), nullptr);

        return buffer;
    }

}