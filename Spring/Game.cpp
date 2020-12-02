#include "Game.hpp"
#include <engine\graphics\core\device.cpp>
#include <engine\input\inputmanager.cpp>
#include <chrono>
#include <iostream>

Game::Game() : maxDt(1.f / 60.f) {
	//acquires global resources
	graphics::Device::initialize(1366, 768, false);
	window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	glClearColor(0.f, 1.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

Game::~Game() {
	graphics::Device::close();
}

void Game::run(std::unique_ptr<Spring> _initialState) {
	//manages game states with a stack invoking the appropriate events
	//controls delta time to maintain a smooth frame rate without wasting to much CPU time
	//performs state update + rendering

	using clock = std::chrono::high_resolution_clock;
	using duration_t = std::chrono::duration<float>;
	states.push_back(*_initialState);
	std::cout << "in run methode" << std::endl;
	
	typedef std::chrono::milliseconds TimeT;
	while (!states.empty()) {	//TODO: Mehrere States und Wechsel zwischen ihnen.
		GameState& current = states.back();
		float t = 0;
		auto currentTime = clock::now();
		float dt = 0.f;
		auto test_start = std::chrono::steady_clock::now();
		while (!current.isFinished()) {
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			float test_time = std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - test_start).count();
			auto newTime = clock::now();
			duration_t frameTime = newTime - currentTime;
			currentTime = newTime;
			do {
				dt = std::min(frameTime.count(), maxDt);
				current.update(test_time, dt);
				frameTime -= duration_t(dt);
				t += dt;
			} while (frameTime.count() > 0.0f);
			current.draw(test_time, dt);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		states.pop_back();
	}
}
