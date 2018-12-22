#include <iostream>

#include "logger.h"
#include "const.h"

namespace Logger {
	Level level = Level::INFO;

	void debug(std::string msg) {
		if (level <= Level::DEBUG) {
			std::cout << "DEBUG: " << msg << std::endl;
		}
	}

	void info(std::string msg) {
		if (level <= Level::INFO) {
			std::cout << "INFO: " << msg << std::endl;
		}
	}

	void warn(std::string msg) {
		if (level <= Level::WARN) {
			std::cerr << "WARN: " << msg << std::endl;
		}
	}

	void fatal(std::string msg, bool quit) {
		if (level <= Level::FATAL) {
			std::cerr << "FATAL: " << msg << std::endl;
		}

		if (quit) {
			std::getchar();
			exit(1);
		}
	}
}