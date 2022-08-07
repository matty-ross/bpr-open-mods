#include "pch.h"
#include "Logger.h"


namespace Utility {

    Logger::Logger(const std::string& filePath)
        :
        m_FilePath(filePath)
    {
    }

    void Logger::Log(LogType logType, const std::string& message) const
    {
        std::ofstream logFile(m_FilePath, std::ios_base::app);

        logFile << GetTime() << "  ";
        logFile << std::setw(12) << std::left << GetLogType(logType);
        logFile << message;
        logFile << '\n';

        logFile.close();
    }

    std::string Logger::GetLogType(LogType logType)
    {
        switch (logType)
        {
        case LogType::Info:
            return "[Info]";

        case LogType::Warning:
            return "[Warning]";

        case LogType::Error:
            return "[Error]";
        }

        return std::string();
    }

    std::string Logger::GetTime()
    {
        SYSTEMTIME time = {};
        ::GetLocalTime(&time);

        return std::format("{:02}.{:02}.{:04} {:02}:{:02}:{:02}:{:04}", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    }

}