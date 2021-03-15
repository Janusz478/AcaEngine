#include "sliderdemo.hpp"
#include <iostream>



void nothing() {

};

SliderDemo::SliderDemo() :
	GameState(),
	system(),
	guiToolkit(system)
{
	finished = false;
	//guiToolkit.addButton(glm::vec3(0.35f, 0.1f, 0.f), glm::vec3(0.3f, 0.1f, 1.f), lightBlueTexture, darkBlueTexture, displayMessage, (char*)"click me!");
	Entity sliderEntity;
	system.createEntity(sliderEntity);
	guiToolkit.addSlider(sliderEntity, glm::vec3(0.55f, 0.1f, 0.f), glm::vec3(0.1f, 0.3f, 1.f), 10, 0, true);
	Entity sliderEntity2;
	system.createEntity(sliderEntity2);
	guiToolkit.addSlider(sliderEntity2, glm::vec3(0.1f, 0.1f, 0.f), glm::vec3(0.4, 0.1, 1.f), 25, 5, false);
}

void SliderDemo::newState() {
}

void SliderDemo::update(float _time, float _deltaTime) {

	if (_time > 50) {
		finished = true;
	}

	system.updateTransform(_deltaTime);
	guiToolkit.update(_deltaTime);
}

void SliderDemo::draw(float _time, float _deltaTime) {
	system.draw();
}

void SliderDemo::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SliderDemo::onPause(float _time) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool SliderDemo::isFinished() {
	return finished;
}