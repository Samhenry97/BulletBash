#pragma once

#include "const.h"

namespace Sounds {
	extern std::unordered_map<std::string, sf::SoundBuffer> sounds;
	extern std::unordered_map<std::string, sf::Music*> musics;
	extern std::vector<sf::Sound*> playing;
	extern sf::Music* current;
	void init();
	void play(std::string name);
	void music(std::string name);
	void stopmusic();
	void boss();
	void back();
}