#include "Logger.h"

namespace AudioMaster
{

	Logger* Logger::instance = nullptr;
	std::ofstream Logger::logFile;

    Logger::Logger()
    {
		// Open the file in output (writing) mode and with the append flag set
		Logger::logFile.open(LOG_FILE_PATH, std::ios::out | std::ofstream::trunc);
		Logger::logFile.imbue(std::locale::classic());
		
		if (Logger::logFile.fail() || !Logger::logFile)
		{
			Logger::logFile.close();
		}
    }
    Logger::~Logger()
    {
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
		Logger::logFile.close();
		delete Logger::instance;
		Logger::instance = nullptr;
	}

	void Logger::Log(std::string message)
	{
		if (Logger::logFile.fail() || !Logger::logFile)
			return;

		Logger::logFile << message << std::endl;
	}

	void Logger::Print(std::string messagePartial)
	{
		if (Logger::logFile.fail() || !Logger::logFile)
			return;

		Logger::logFile << messagePartial;
	}
}