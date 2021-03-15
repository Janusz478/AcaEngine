#include "spring.hpp"
#include "gamehelper.hpp"

void getToMenu(Entity& _entity, System& _system) {
	choosenGameState = 1;
	gameStateSwitcht = true;
};


Spring::Spring() : GameState(), system(), guiToolkit(system), timePaused{ 0.0f }, 
darkBlueTexture(*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
lightBlueTexture(*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
whiteTexture(*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
 {
	lastFactor = 1;
	finished = false;
	
	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 5.f));
	
	entity = EntityCreationInterface::createSpringPlanet(system, glm::mat4(1.f), glm::vec3(0.f, 5.f, -15.0f), glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f), 1.f, glm::vec3(0.f, 1.f, -15.f));

	system.createEntity(sliderEntity);
	guiToolkit.addSlider(sliderEntity, glm::vec3(0.1f, 0.1f, 0.f), glm::vec3(0.1f, 0.3f, 1.f), 10, 0, true);

	menuString = "Main Menu";
	system.createEntity(menuButton);
	guiToolkit.addButton(menuButton, glm::vec3(0.01f, 0.92f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, true, getToMenu, menuString, glm::vec4(1.f));

	textDisplayString = "Spring";
	system.createEntity(textDisplay);
	guiToolkit.addTextDisplay(textDisplay, glm::vec3(0.35f, 0.92f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), whiteTexture, textDisplayString, glm::vec4(0.f));
}

void Spring::newState() {
	finished = false;
	timePaused = 0.0f;
	system.eraseEntity(entity);
	entity = EntityCreationInterface::createSpringPlanet(system, glm::mat4(1.f), glm::vec3(0.f, 5.f, -15.0f), glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f), 1.f, glm::vec3(0.f, 1.f, -15.f));
	guiToolkit.deleteSlider(sliderEntity);
	system.createEntity(sliderEntity);
	guiToolkit.addSlider(sliderEntity, glm::vec3(0.1f, 0.1f, 0.f), glm::vec3(0.1f, 0.3f, 1.f), 10, 0, true);
}

void Spring::update(float _time, float _deltaTime) {
	_time += timePaused;
	if (_time > 60) {
		finished = true;
	}
	int level = system.registry.getComponentUnsafe<Slider>(sliderEntity).currentLevel + 1;
	_deltaTime *= level;
	system.springY(entity, _deltaTime);
	system.updateVelocity(_deltaTime);
	system.updatePosition(_deltaTime);
	system.updateTransform(_deltaTime);
	guiToolkit.update(_deltaTime);
}

void Spring::draw(float _time, float _deltaTime) {
	system.draw();
}

void Spring::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Spring::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Spring::isFinished() {
	return finished;
}
