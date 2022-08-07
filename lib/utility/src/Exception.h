#pragma once


#include <string>
#include <format>


namespace Utility {

    class Exception
    {
    public:
        Exception(const std::string& file, int line, const std::string& function, const std::string& description);

        template <typename... Args>
        Exception(const std::string& file, int line, const std::string& function, const std::string& format, Args&&... args)
            :
            Exception(file, line, function, std::vformat(format, std::make_format_args(args...)))
        {
        }

        std::string GetSummary() const;
        const std::string& GetFile() const;
        int GetLine() const;
        const std::string& GetFunction() const;
        const std::string& GetDescription() const;
        const std::string& GetDetails() const;

    protected:
        std::string m_File;
        int m_Line;
        std::string m_Function;
        std::string m_Description;
        std::string m_Details;
    };


    class WindowsException : public Exception
    {
    public:
        WindowsException(const std::string& file, int line, const std::string& function, uint32_t lastError, const std::string& description);

        template <typename... Args>
        WindowsException(const std::string& file, int line, const std::string& function, uint32_t lastError, const std::string& format, Args&&... args)
            :
            WindowsException(file, line, function, lastError, std::vformat(format, std::make_format_args(args...)))
        {
        }

    private:
        uint32_t GetErrorCode() const;
        std::string GetErrorMessage() const;

    private:
        uint32_t m_LastError;
    };

}


#define EXCEPTION(...)            Utility::Exception(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define WINDOWS_EXCEPTION(...)    Utility::WindowsException(__FILE__, __LINE__, __FUNCTION__, ::GetLastError(), __VA_ARGS__)