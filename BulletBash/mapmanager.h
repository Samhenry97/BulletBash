#pragma once

#include "const.h"

namespace Maps {
	extern std::map<std::string, std::vector<std::vector<std::string>>> templates;

	void init();
	std::vector<std::string> &loadTemplate(std::string room);
};