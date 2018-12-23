#include "sounds.h"

namespace Sounds {
	std::vector<std::string> names = { 
		"bullet.wav", "damage.wav", "mommy.wav", "mommylove.wav",
		"empty.wav", "reload.wav", "shotgun.wav", "water.wav",
		"flamethrower.wav", "select.wav", "hover.wav", "ammo.wav", 
		"amazing.wav", "nextroom.wav",
	};
	std::unordered_map<std::string, sf::SoundBuffer> sounds;
	std::unordered_map<std::string, sf::Music*> musics;
	std::vector<sf::Sound*> playing;
	sf::Music* current = nullptr;

	void init() {
		for (std::string name : names) {
			sf::SoundBuffer buff;
			if (!buff.loadFromFile("sounds/" + name)) {
				Logger::fatal("Could not load sound: " + name);
			}
			sounds[name] = buff;
		}
	}

	void play(std::string name) {
		for (int i = 0; i < playing.size(); i++) {
			sf::Sound *cur = playing[i];
			if (cur->getStatus() == sf::Sound::Stopped) {
				playing.erase(playing.begin() + i);
				delete cur; i--;
			}
		}

		sf::Sound *sound = new sf::Sound(sounds[name]);
		sound->play();
		playing.push_back(sound);
	}

	void boss() {
		int choice = rand() % BOSS_MUSIC + 1;
		music("boss" + STR(choice) + ".wav");
	}

	void back() {
		int choice = rand() % BACK_MUSIC + 1;
		music("back" + STR(choice) + ".wav");
	}

	void stopmusic() {
		if(current) current->stop();
	}

	void music(std::string name) {
		stopmusic();

		if (musics.find(name) == musics.end()) {
			sf::Music *music = new sf::Music();
			music->openFromFile("sounds/music/" + name);
			music->setLoop(true);
			music->setVolume(50);
			musics[name] = music;
		}

		musics[name]->play();
		current = musics[name];
	}
}