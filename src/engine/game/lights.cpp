#include "lights.hpp"
#include "gamehelper.hpp"

void toTheMenu(Entity& _entity, System& _system) {
	choosenGameState = 1;
	gameStateSwitcht = true;
};

void nothing(Entity& _entity, System& _system) {};

Lights::Lights() : GameState(), system(), guiToolkit(system), timePaused{ 0.0f },
darkBlueTexture(*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
lightBlueTexture(*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
whiteTexture(*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{

	menuString = "Main Menu";
	system.createEntity(menuButton);
	guiToolkit.addButton(menuButton, glm::vec3(0.01f, 0.92f, 0.f), glm::vec3(0.25f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, true, toTheMenu, menuString, glm::vec4(1.f));

	textDisplayString = "Point Lights";
	system.createEntity(textDisplay);
	guiToolkit.addTextDisplay(textDisplay, glm::vec3(0.35f, 0.92f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), whiteTexture, textDisplayString, glm::vec4(0.f));

	checkBoxString1 = "Light 1";
	checkBoxes.push_back(system.createEntity(entity));
	guiToolkit.addCheckBox(checkBoxes.back(), glm::vec3(0.f), glm::vec3(1.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString1, glm::vec4(0.f, 0.f, 0.f, 1.f), true);

	checkBoxString2 = "Light 2";
	checkBoxes.push_back(system.createEntity(entity));
	guiToolkit.addCheckBox(checkBoxes.back(), glm::vec3(0.f), glm::vec3(1.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString2, glm::vec4(0.f, 0.f, 0.f, 1.f), true);

	checkBoxString3 = "Light 3";
	checkBoxes.push_back(system.createEntity(entity));
	guiToolkit.addCheckBox(checkBoxes.back(), glm::vec3(0.f), glm::vec3(1.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString3, glm::vec4(0.f, 0.f, 0.f, 1.f), true);

	checkBoxString4 = "Light 4";
	checkBoxes.push_back(system.createEntity(entity));
	guiToolkit.addCheckBox(checkBoxes.back(), glm::vec3(0.f), glm::vec3(1.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString4, glm::vec4(0.f, 0.f, 0.f, 1.f), true);

	container = system.createEntity(container);
	guiToolkit.addContainer(container, glm::vec3(0.01f, 0.1f, 0.f), glm::vec3(0.25f, 0.35f, 1.f), 4, 1, 0.05, darkBlueTexture, true, false, checkBoxes);

	meshEntity = EntityCreationInterface::createRotatingPlanet(system, glm::translate(glm::vec3(-15.f, 1.f, -20.0f)), glm::vec3(-15.f, 1.f, -20.f),
		glm::vec3(1.f, 1.f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.05f, 0.01f, 0.02f), true);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<float> intensities;

	positions.push_back(glm::vec3(25.f, 1.f, -20.0f));
	colors.push_back(glm::vec3(10.f, 10.f, 10.f));
	intensities.push_back(2.f);

	positions.push_back(glm::vec3(0.f, 20.f, 0.f));
	colors.push_back(glm::vec3(10.f, 0.f, 0.f));
	intensities.push_back(2.f);
	
	positions.push_back(glm::vec3(-15.f, 1.f, -10.f));
	colors.push_back(glm::vec3(0.f, 3.f, 0.f));
	intensities.push_back(2.f);
	
	positions.push_back(glm::vec3(5.f, -10.f, -20.0f));
	colors.push_back(glm::vec3(0.f, 0.f, 7.f));
	intensities.push_back(2.f);

	lightEntities = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f, positions, colors, intensities);

}

void Lights::newState() {
	timePaused = 0.f;
	finished = false;
	guiToolkit.deleteCheckBox(checkBoxes[0]);
	system.createEntity(checkBoxes[0]);
	guiToolkit.addCheckBox(checkBoxes[0], glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString1, glm::vec4(0.f, 0.f, 0.f, 1.f), true);
	system.registry.getComponentUnsafe<PointLight>(lightEntities[0]).intensity = 2.f;

	guiToolkit.deleteCheckBox(checkBoxes[1]);
	system.createEntity(checkBoxes[1]);
	guiToolkit.addCheckBox(checkBoxes[1], glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString2, glm::vec4(0.f, 0.f, 0.f, 1.f), true);
	system.registry.getComponentUnsafe<PointLight>(lightEntities[1]).intensity = 2.f;

	guiToolkit.deleteCheckBox(checkBoxes[2]);
	system.createEntity(checkBoxes[2]);
	guiToolkit.addCheckBox(checkBoxes[2], glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString3, glm::vec4(0.f, 0.f, 0.f, 1.f), true);
	system.registry.getComponentUnsafe<PointLight>(lightEntities[2]).intensity = 2.f;

	guiToolkit.deleteCheckBox(checkBoxes[3]);
	system.createEntity(checkBoxes[3]);
	guiToolkit.addCheckBox(checkBoxes[3], glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), lightBlueTexture, darkBlueTexture, nothing, checkBoxString4, glm::vec4(0.f, 0.f, 0.f, 1.f), true);
	system.registry.getComponentUnsafe<PointLight>(lightEntities[3]).intensity = 2.f;
	
	system.eraseEntity(container);
	system.createEntity(container);
	guiToolkit.addContainer(container, glm::vec3(0.01f, 0.1f, 0.f), glm::vec3(0.25f, 0.35f, 1.f), 4, 1, 0.05, false, false, checkBoxes);


	system.eraseEntity(meshEntity);
	meshEntity = EntityCreationInterface::createRotatingPlanet(system, glm::translate(glm::vec3(-15.f, 1.f, -20.0f)), glm::vec3(-15.f, 1.f, -20.f),
		glm::vec3(1.f, 1.f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.05f, 0.01f, 0.02f), true);

}

void Lights::update(float _time, float _deltaTime) {
	_time += timePaused;
	guiToolkit.update(_deltaTime);
	if ((int)_time % 11 == 0) {
		system.eraseEntity(meshEntity);
		meshEntity = EntityCreationInterface::createRotatingPlanet(system, glm::translate(glm::vec3(-15.f, 1.f, -20.0f)), glm::vec3(-15.f, 1.f, -20.f),
			glm::vec3(1.f, 1.f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.05f, 0.01f, 0.02f), true);
	}
	for (int i = 0; i < 4; i++) {
		CheckBox& c = system.registry.getComponentUnsafe<CheckBox>(checkBoxes[i]);
		if (c.pressed && !c.status) {
			system.registry.getComponentUnsafe<PointLight>(lightEntities[i]).intensity = 0.f;
		}
		else if (c.pressed && c.status) {
			system.registry.getComponentUnsafe<PointLight>(lightEntities[i]).intensity = 2.f;
		}
	}
	if (_time > 60) finished = true;
	system.updateTransform(_deltaTime);
	system.updatePosition(_deltaTime);
	system.updateOrientation(_deltaTime);
}

void Lights::draw(float _time, float _deltaTime) {
	system.draw();
}

void Lights::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Lights::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Lights::isFinished(){
	return false;
}
