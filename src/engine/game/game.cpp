#include "game.hpp"
#include "gamehelper.hpp"


int choosenGameState;
bool gameStateSwitcht;


Game::Game() {
	graphics::Device::initialize(1600, 1000, false); // fullScreen -> false or true -> has no effect -> always no fullScreen -> solution -> glfwGetPrimaryMonitor()
	window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glfwSwapInterval(0); //VSync ON->(1) , OFF->(0)
	srand(static_cast<unsigned int>(time(NULL)));
	EntityCreationInterface::initialize();
}

Game::~Game() {
	graphics::Device::close(); 
	utils::MeshLoader::clear();
	graphics::ShaderManager::clear();
	graphics::Texture2DManager::clear();
}

void Game::addState(std::shared_ptr<GameState> _state) {
	states.push_back(std::move(_state));
}

void Game::run(std::shared_ptr<GameState> _initialState) {

	using clock = std::chrono::high_resolution_clock;
	using duration_t = std::chrono::duration<float>;
	states.push_back(std::move(_initialState));
	bool rightPressed = false, leftPressed = false, f1Pressed = false;
	

	while (!states.empty() && !glfwWindowShouldClose(window)) {
		std::shared_ptr<GameState> current = states.back();
		
		current->onResume();
		float t = 0;
		const float timeStep = 0.01;
		auto currentTime = clock::now();
		float dt = 0.f;
		rightPressed = leftPressed = f1Pressed = false;

		choosenGameState = 0;

		while (!current->isFinished() && !glfwWindowShouldClose(window) && !leftPressed && !rightPressed && !f1Pressed && !choosenGameState) {
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
			current->draw(t, dt / timeStep);
			glfwPollEvents();
			glfwSwapBuffers(window);

			if (input::InputManager::isKeyPressed(input::Key::RIGHT)) {
				rightPressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::LEFT)) {
				leftPressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::F1)) {
				f1Pressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
		}

		if (rightPressed) {
			current->onPause(t);
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}
		else if (leftPressed) {
			if (pausedStates.size() == 0) {
				current->newState();
			}
			else {
				current->onPause(t);
				std::move(pausedStates.end() - 1, pausedStates.end(), std::back_inserter(states));
				pausedStates.pop_back();
			}
		}
		else if (f1Pressed) {
			current->newState();
		}
		else if (choosenGameState) {
			chooseState(t, choosenGameState);
		}
		else {
			current->newState();
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}
		if (glfwWindowShouldClose(window) || states.empty()) {
			for (auto it = states.begin(); it != states.end(); it++) {
				it->reset();
			}
			for (auto it = pausedStates.begin(); it != pausedStates.end(); it++) {
				it->reset();
			}
		}
	}
}

void Game::chooseState(float time,  int number) {
	states.back()->onPause(time);
	int numPausedStates = pausedStates.size();
	int numRemainingStates = states.size();
	int totalStates = numPausedStates + numRemainingStates;
	if (0 < number && number <= numPausedStates) {		//backwards
		int difference = numPausedStates - number + 1;
		for (int i = 0; i < difference; i++) {
			std::move(pausedStates.end() - 1, pausedStates.end(), std::back_inserter(states));
			pausedStates.pop_back();
		}
	}
	else if (number == numPausedStates + 1) {	//current
		return;
	}
	else if (0 < number && number <= totalStates) {		//forewards
		int difference = number - numPausedStates - 1;
		for (int i = 0; i < difference; i++) {
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}
	}
	else {
		for (auto it = states.begin(); it != states.end(); it++) {
			it->reset();
		}
		for (auto it = pausedStates.begin(); it != pausedStates.end(); it++) {
			it->reset();
		}
		states.clear();
	}

}
