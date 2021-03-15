#include "shooter.hpp"
#include "gamehelper.hpp"


void goToMenu(Entity& _entity, System& _system) {
	choosenGameState = 1;
	gameStateSwitcht = true;
};

void createTargets(Entity& _entity, System& _system) {
	for (int i = 0; i < 5; i++) {
		EntityCreationInterface::createRotatingCrate(_system, glm::mat4(1.f),
			glm::vec3(rand() % 51 + (-25), rand() % 51 + (-25), rand() % 10 + (-55)), glm::vec3(1.f, 0.5f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f),
			glm::vec3(_system.randomWithoutZero(9, -4), _system.randomWithoutZero(9, -4), _system.randomWithoutZero(9, -4)),
			glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5),
			false);
	}
}

void no(Entity& _entity, System& _system) {}

Shooter::Shooter() : GameState(), system(), shot(0), guiToolkit(system), timePaused{ 0.0f },
darkBlueTexture(*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
lightBlueTexture(*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
whiteTexture(*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	menuString = "Main Menu";
	system.createEntity(menuButton);
	guiToolkit.addButton(menuButton, glm::vec3(0.01f, 0.92f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, true, goToMenu, menuString, glm::vec4(1.f));

	createTargetsString = "Create Targets";
	system.createEntity(createTargetsButton);
	guiToolkit.addButton(createTargetsButton, glm::vec3(0.7f, 0.05f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, true, createTargets, createTargetsString, glm::vec4(1.f));

	checkBoxString = "Collision Detection";
	system.createEntity(checkBox);
	guiToolkit.addCheckBox(checkBox, glm::vec3(0.01f, 0.05f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, no, checkBoxString, glm::vec4(0.f, 0.f, 0.f, 1.f), true);

	textDisplayString = "Shooter";
	system.createEntity(textDisplay);
	guiToolkit.addTextDisplay(textDisplay, glm::vec3(0.35f, 0.92f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), whiteTexture, textDisplayString, glm::vec4(0.f));

	hitPointsString = "Points: 0";
	points = 0;
	system.createEntity(hitPoints);
	guiToolkit.addTextDisplay(hitPoints, glm::vec3(0.74f, 0.92f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), whiteTexture, hitPointsString, glm::vec4(0.f));

	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 10.f));
	for (int i = 0; i < 50; i++) {
		entities.push_back(EntityCreationInterface::createRotatingCrate(system, glm::mat4(1.f), 
			glm::vec3(rand() % 51 + (-25), rand() % 51 + (-25), rand() % 10 + (-55)), glm::vec3(1.f, 0.5f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f),
			glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)), 
			glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5),
			false));
	}

}

void Shooter::newState() {
	finished = false;
	timePaused = 0.0f;
	for (auto it = entities.begin(); it != entities.end(); it++) {
		system.eraseEntity(*it);
	}
	entities.clear();
	for (int i = 0; i < 50; i++) {
		entities.push_back(EntityCreationInterface::createRotatingCrate(system, glm::mat4(1.f),
			glm::vec3(rand() % 51 + (-25), rand() % 51 + (-25), rand() % 10 + (-55)), glm::vec3(1.f, 0.5f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f),
			glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)),
			glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5),
			false));
	}
	guiToolkit.deleteCheckBox(checkBox);
	system.createEntity(checkBox);
	guiToolkit.addCheckBox(checkBox, glm::vec3(0.01f, 0.05f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, no, checkBoxString, glm::vec4(0.f, 0.f, 0.f, 1.f), true);
	hitPointsString = "Points: 0";
	points = 0;
}

void Shooter::update(float _time, float _deltaTime) {
	_time += timePaused;
	
	if (_time > 200) finished = true;
	
	if (_time > 0.2) {
	
		if (shot == 0 && input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
			glm::vec3 curserPos = system.cameraPerspective.toWorldSpace(input::InputManager::getCursorPos());
			entities.push_back(EntityCreationInterface::createShootablePlanet(system, glm::mat4(1.f), glm::vec3(curserPos), 
				glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(curserPos * 150.f), true));
			shot = 30;
		}
		else if (shot > 0) {
			shot--;
		}
		int deletedCrates = 0;
	
		CheckBox& c = system.registry.getComponentUnsafe<CheckBox>(checkBox);
		if (c.status) {
			deletedCrates = system.removeIntersecting();
		}

		points += deletedCrates;
		std::string tempStr = "Points: ";
		tempStr.append(std::to_string(points));
		hitPointsString = tempStr;

		for (int i = 0; i < deletedCrates; i++) {
			entities.push_back(EntityCreationInterface::createRotatingCrate(system, glm::mat4(1.f),
				glm::vec3(rand() % 51 + (-25), rand() % 51 + (-25), rand() % 10 + (-55)), glm::vec3(1.f, 0.5f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f),
				glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)),
				glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5),
				false));
		}

		int entityId = system.whichEntityIsNotInView();
		if (entityId != -1) {
			Entity entity{ static_cast<uint32_t>(entityId) };
			if (system.hasComponent<Orientation>(entity)) {
				system.setPosition(entity, glm::vec3(-75 + ((rand() % 2) * 150), -50 + ((rand() % 2) * 100), rand() % 10 + (-55)));
				system.setVelocity(entity, glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)));
			}
			else {
				system.eraseEntity(entity);
				entities.remove_if([&entity](const Entity& _entity) { return _entity.id == entity.id; });
			}
		}	
	}

	guiToolkit.update(_deltaTime);
	system.updatePosition(_deltaTime);
	system.updateOrientation(_deltaTime);
	system.updateTransform(_deltaTime);
}

void Shooter::draw(float _time, float _deltaTime) {
	system.draw();
}

void Shooter::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Shooter::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Shooter::isFinished(){
	return false;
}

