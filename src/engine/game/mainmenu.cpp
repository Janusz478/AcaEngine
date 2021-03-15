#include "mainmenu.hpp"
#include "gamehelper.hpp"


void chooseGameState1(Entity& _entity, System& _system) {	
	choosenGameState = 1;
	gameStateSwitcht = true;
};
void chooseGameState2(Entity& _entity, System& _system) {
	choosenGameState = 2;
	gameStateSwitcht = true;
};
void chooseGameState3(Entity& _entity, System& _system) {
	choosenGameState = 3;
	gameStateSwitcht = true;
};
void chooseGameState4(Entity& _entity, System& _system) {
	choosenGameState = 4;
	gameStateSwitcht = true;
};
void chooseGameState5(Entity& _entity, System& _system) {
	choosenGameState = 5;
	gameStateSwitcht = true;
};
void chooseGameState6(Entity& _entity, System& _system) {
	choosenGameState = 6;
	gameStateSwitcht = true;
};
void chooseGameState7(Entity& _entity, System& _system) {
	choosenGameState = 7;
	gameStateSwitcht = true;
};
void exitGameFunction(Entity& _entity, System& _system) {
	choosenGameState = -1;
	gameStateSwitcht = true;
};


MainMenu::MainMenu() :
	GameState(),	
	system(), 
	guiToolkit(system),
	lightGrayTexture(*graphics::Texture2DManager::get("textures/lightGray.png", graphics::Sampler(
		graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	finished = false;

	stringMainMenu		= "Main Menu";	
	stringExitGame		= "Exit";
	stringGameState1	= "Main Menu";
	stringGameState2	= "Shooter";
	stringGameState3	= "Lights";
	stringGameState4	= "Spring";
	stringGameState5	= "FreeFall";
	stringGameState6	= "UpAndDown";
	stringGameState7	= "Settings";

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements1.back(), stringMainMenu);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), chooseGameState2, stringGameState2);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), chooseGameState3, stringGameState3);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), chooseGameState4, stringGameState4);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), chooseGameState5, stringGameState5);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), chooseGameState6, stringGameState6);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), chooseGameState7, stringGameState7);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), exitGameFunction, stringExitGame, glm::vec4(0.f));

	container1 = system.createEntity(container1);
	guiToolkit.addContainer(container1, glm::vec3(0.3f, 0.1f, 0.f), glm::vec3(0.4f, 0.8f, 1.f), 8, 1, 0.05f, lightGrayTexture, true, true, containerElements1);

}

void MainMenu::newState() {
	glClearColor(0.f, 0.f, 0.f, 0.f);
}

void MainMenu::update(float _time, float _deltaTime) {

	system.updateTransform(_deltaTime);
	guiToolkit.update(_deltaTime);
}

void MainMenu::draw(float _time, float _deltaTime) {
	system.draw();
}

void MainMenu::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainMenu::onPause(float _time) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool MainMenu::isFinished() {
	return finished;
}