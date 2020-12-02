#pragma once
#include "Game.hpp"
#include <engine/graphics/core/device.cpp>
#include <engine/input/inputmanager.cpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <engine/input/inputmanager.hpp>


Game::Game() {
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

void Game::addState(std::unique_ptr<GameState> _state) {
	states.push_back(std::move(_state));
}

void Game::run(std::unique_ptr<GameState> _initialState) {
	//manages game states with a stack invoking the appropriate events
	//controls delta time to maintain a smooth frame rate without wasting to much CPU time
	//performs state update + rendering

	using clock = std::chrono::high_resolution_clock;
	using duration_t = std::chrono::duration<float>;
	states.push_back(std::move(_initialState));
	std::cout << "in run methode" << std::endl;
	bool b = true;

	while (!states.empty() && !glfwWindowShouldClose(window)) {
		GameState& current = *states.back();
		float t = 0;
		const float timeStep = 0.01;

		auto currentTime = clock::now();
		float dt = 0.f;

		while (!current.isFinished() && !glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			auto newTime = clock::now();
			duration_t frameTime = newTime - currentTime;
			currentTime = newTime;
			dt += frameTime.count();
			while (dt >= timeStep) {
				t += timeStep;
				dt -= timeStep;
				current.update(t, timeStep);
			}
			current.draw(t, dt/timeStep);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		states.pop_back();
	}
}
