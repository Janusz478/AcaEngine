#include "game.hpp"


Game::Game() {
	//acquires global resources
	graphics::Device::initialize(1000, 1000, false);
	window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	glClearColor(0.f, 1.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glfwSwapInterval(0); //VSync ON->(1) , OFF->(0)
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

	while (!states.empty() && !glfwWindowShouldClose(window)) {
		std::unique_ptr<GameState> current = std::move(states.back());
		float t = 0;
		const float timeStep = 0.01;

		auto currentTime = clock::now();
		float dt = 0.f;

		while (!current->isFinished() && !glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			auto newTime = clock::now();
			duration_t frameTime = newTime - currentTime;
			currentTime = newTime;
			dt += frameTime.count();
			while (dt >= timeStep) {
				t += timeStep;
				dt -= timeStep;
				current->update(t, timeStep);
			}
			current->draw(t, dt/timeStep);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		states.pop_back();
	}
}
