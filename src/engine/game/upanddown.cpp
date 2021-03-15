#include "upanddown.hpp"
#include "gamehelper.hpp"

void toTheMenu3(Entity& _entity, System& _system) {
	choosenGameState = 1;
	gameStateSwitcht = true;
};

UpAndDown::UpAndDown() : GameState(), system(), guiToolkit(system), timePaused{ 0.0f },
darkBlueTexture(*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
lightBlueTexture(*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
whiteTexture(*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	menu = "Main Menu";
	system.createEntity(menuButton);
	guiToolkit.addButton(menuButton, glm::vec3(0.01f, 0.92f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, true, toTheMenu3, menu, glm::vec4(1.f));

	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 5.f));
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 4.f, -15.0f)), glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f));

	textDisplayText = "Up and Down";
	system.createEntity(textDisplay);
	guiToolkit.addTextDisplay(textDisplay, glm::vec3(0.35f, 0.92f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), whiteTexture, textDisplayText, glm::vec4(0.f));

}

void UpAndDown::newState() {
	finished = false;
	timePaused = 0.0f;
	system.eraseEntity(entity);
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 4.f, -15.0f)), glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f));
}

void UpAndDown::update(float _time, float _deltaTime) {
	_time += timePaused;
	if (_time > 60) {
		finished = true;
	}
	if ((int)_time % 2 == 0) {
		system.setVelocity(entity, glm::vec3(0.f, -0.1f, 0.f));
		system.move(entity, _deltaTime);
	}
	else {
		system.setVelocity(entity, glm::vec3(0.f, +0.1f, 0.f));
		system.move(entity, _deltaTime);
	}
	system.updateTransform(_deltaTime);
	guiToolkit.update(_deltaTime);
}

void UpAndDown::draw(float _time, float _deltaTime) {
	system.draw();
}

void UpAndDown::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void UpAndDown::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool UpAndDown::isFinished() {
	return false;
}
