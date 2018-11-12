#include "Logger.h"

namespace AudioMaster
{

	Logger* Logger::instance = nullptr;

    Logger::Logger()
    {
		Logger::logFile.open(LOG_FILE_PATH, std::ios::out);
		Logger::logFile.imbue(std::locale::classic());
		
		if (Logger::logFile.fail() || !Logger::logFile)
		{
			Logger::logFile.close();
		}
    }
    Logger::~Logger()
    {
        Logger::logFile.close();
    }

	Logger* Logger::GetInstance()
	{
		if (Logger::instance == nullptr)
		{
			Logger::instance = new Logger();
		}

		return Logger::instance;
	}
	void Logger::ResetInstance()
	{
		delete Logger::instance;
		Logger::instance = nullptr;
	}

	void Logger::Log(std::string message)
	{
		if (Logger::logFile.fail() || !Logger::logFile)
			return;

		Logger::logFile << message << std::endl;
	}
}