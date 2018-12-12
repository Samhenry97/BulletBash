#pragma once

#include <string>

namespace Logger {
	enum Level { OFF, DEBUG, INFO, WARN, FATAL };
	extern Level level;
	void debug(std::string msg);
	void info(std::string msg);
	void warn(std::string msg);
	void fatal(std::string msg, bool quit = true);
}