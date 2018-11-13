#ifndef INC_LOGGER_H
#define INC_LOGGER_H

#include <string>
#include <fstream>

namespace AudioMaster
{
	constexpr const char* LOG_FILE_PATH = "data\\log.txt";

	class Logger
	{
	private:
		static Logger* instance;

		std::ofstream logFile;

	public:
		Logger();
		~Logger();

		// Specify no copy constructor or assignment operator to stop cloning the Logger singleton
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static Logger* GetInstance();
		static void ResetInstance();

		void Log(std::string message);
	};
	
}

#endif