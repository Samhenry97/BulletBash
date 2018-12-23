#include <fstream>

#include "mapmanager.h"

namespace Maps {
	std::map<std::string, std::vector<std::vector<std::string>>> templates;
	std::vector<std::string> names = {
		"default",
		"chest",
		"enemy",
		"boss"
	};

	void init() {
		for (int i = 0; i < names.size(); i++) {
			std::ifstream file("data/" + names[i] + ".txt");
			std::string tmp;
			if (file.is_open()) {
				while (file >> tmp) {
					if (tmp == "template") {
						templates[names[i]].push_back(std::vector<std::string>());
					} else {
						templates[names[i]][templates[names[i]].size() - 1].push_back(tmp);
					}
				}
				file.close();
			} else {
				Logger::fatal("Could not read level data.");
			}
		}
	}

	std::vector<std::string> &loadTemplate(std::string room) {
		int idx = rand() % templates[room].size();
		return templates[room][idx];
	}
}