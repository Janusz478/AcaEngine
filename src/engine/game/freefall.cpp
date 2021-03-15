#include "freefall.hpp"
#include "gamehelper.hpp"


void toTheMenu2(Entity& _entity, System& _system) {
	choosenGameState = 1;
	gameStateSwitcht = true;
};

FreeFall::FreeFall() : GameState(), system(), guiToolkit(system), timePaused{ 0.0f },
darkBlueTexture(*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
lightBlueTexture(*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
whiteTexture(*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	menuString = "Main Menu";
	system.createEntity(menuButton);
	guiToolkit.addButton(menuButton, glm::vec3(0.01f, 0.92f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, true, toTheMenu2, menuString, glm::vec4(1.f));

	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 10.f));
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 10.f, -62.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));
	entity2 = EntityCreationInterface::createMovingCrate(system, glm::translate(glm::vec3(5.f, 10.f, -62.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));

	textDisplayString = "Free Fall";
	system.createEntity(textDisplay);
	guiToolkit.addTextDisplay(textDisplay, glm::vec3(0.35f, 0.92f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), whiteTexture, textDisplayString, glm::vec4(0.f));

}

void FreeFall::newState() {
	finished = false;
	timePaused = 0.0f;
	system.eraseEntity(entity);
	system.eraseEntity(entity2);
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 10.f, -62.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));
	entity2 = EntityCreationInterface::createMovingCrate(system, glm::translate(glm::vec3(5.f, 10.f, -62.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));
}

void FreeFall::update(float _time, float _deltaTime) {
	_time += timePaused;
	if (_time > 10) {
		finished = true;
	}
	system.move(entity, _deltaTime);
	system.move(entity2, _deltaTime);
	system.updateTransform(_deltaTime);
	guiToolkit.update(_deltaTime);
}

void FreeFall::draw(float _time, float _deltaTime) {
	system.draw();
}

void FreeFall::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void FreeFall::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool FreeFall::isFinished(){
	return false;
}
