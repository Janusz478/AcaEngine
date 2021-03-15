#pragma once

#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "gamestate.hpp"
#include "../gui_toolkit/entity_creation_interface.hpp"
#include "../graphics/core/device.hpp"
#include "../input/inputmanager.hpp"


class Game {
public:
	//initialize global resources
	Game();

	//finalize global resources
	~Game();

	//Start the game loop with _initialState on the stack
	//Keys Left, Right and Numbers to switch between states, Space to restart State
	void run(std::shared_ptr<GameState> _initialState);

	void chooseState(float time, int number);

	void addState(std::shared_ptr<GameState> _state);
	
private:
	GLFWwindow* window;	
	std::vector<std::shared_ptr<GameState>> states;
	std::vector<std::shared_ptr<GameState>> pausedStates;
};
