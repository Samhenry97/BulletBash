#include "logger.h"
#include "const.h"
#include "game.h"
#include "images.h"
#include "sounds.h"
#include "util.h"
#include "xbox.h"
#include "mapmanager.h"

void pollEvents(sf::RenderWindow &window) {
	sf::Event e;
	int id, button, axis;
	float pos;

	while (window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			running = false;
			break;
		case sf::Event::JoystickButtonPressed:
			id = e.joystickButton.joystickId, button = e.joystickButton.button;
			Xbox::setButton(id, button, true);
			game->sendButtonPressed(id, button);
			break;
		case sf::Event::JoystickButtonReleased:
			id = e.joystickButton.joystickId, button = e.joystickButton.button;
			Xbox::setButton(id, button, false);
			game->sendButtonReleased(id, button);
			break;
		case sf::Event::JoystickMoved:
			id = e.joystickMove.joystickId, axis = e.joystickMove.axis, pos = e.joystickMove.position;
			Xbox::setAxis(id, axis, pos);
			game->sendAxisMoved(id, axis, pos);
			break;
		case sf::Event::JoystickConnected:
			id = e.joystickConnect.joystickId;
			Xbox::setConnected(id, true);
			break;
		case sf::Event::JoystickDisconnected:
			id = e.joystickConnect.joystickId;
			Xbox::setConnected(id, false);
			break;
		case sf::Event::LostFocus:
			Logger::info("Lost Focus!");
			if (game->state == Game::INGAME) {
				game->switchTo(Game::PAUSED);
			}
			break;
		case sf::Event::GainedFocus:
			Logger::info("Gained Focus!");
			break;
		case sf::Event::KeyPressed:
			Logger::info("Key pressed!");
			break;
		}
	}
}

int main() {
	sf::RenderWindow display(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), NAME, sf::Style::Fullscreen);
	display.setVerticalSyncEnabled(true);
	display.setFramerateLimit(60);
	display.setKeyRepeatEnabled(false);
	window = &display;

	sf::RenderTexture lightTexture;
	lightTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	lights = &lightTexture;
	light.setTexture(Images::get("light.png"));

	view = new sf::View(vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	minimap = new sf::View;
	minimap->setViewport(sf::FloatRect(MINIMAP_X, MINIMAP_Y, MINIMAP_WIDTH, MINIMAP_HEIGHT));
	minimap->setSize(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);

	sf::RectangleShape minimapBackground;
	minimapBackground.setSize(vec2(WINDOW_WIDTH * MINIMAP_WIDTH, WINDOW_HEIGHT * MINIMAP_HEIGHT));
	minimapBackground.setPosition(WINDOW_WIDTH * MINIMAP_X, WINDOW_HEIGHT * MINIMAP_Y);
	minimapBackground.setTexture(Images::get("gui.png"));
	minimapBackground.setFillColor(sf::Color(255, 255, 255, 200));

	sf::RectangleShape background;
	background.setSize(vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	background.setTexture(Images::get("background.png"));

	Maps::init();
	Sounds::init();
	Xbox::init();
	srand(time(0));
	game = new Game();

	sf::Clock timer;
	running = true;
	while (running) {
		pollEvents(display);
		game->update();
		frameTime = timer.restart().asSeconds();

		display.clear(sf::Color::Black);
		lightTexture.clear(sf::Color(40, 40, 40, 200));
		display.setView(window->getDefaultView());
		display.draw(background);
		display.setView(*view);
		game->render();
		display.setView(window->getDefaultView());
		lightTexture.display();
		sf::Sprite lightSprite(lightTexture.getTexture());
		display.draw(lightSprite, sf::BlendMultiply);
		game->renderStatic();
		if (game->state == Game::MINIMAP) {
			window->draw(minimapBackground);
			window->setView(*minimap);
			game->renderMinimap();
		}

		display.display();
	}

	delete game;

	return 0;
}
