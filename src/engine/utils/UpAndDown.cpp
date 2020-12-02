#pragma once
#include <engine/utils/UpAndDown.hpp>
#include <GL/glew.h>

UpAndDown::UpAndDown() : GameState() {
	difference = glm::translate(glm::vec3(0.f, 4.f, -5.0f));
}

void UpAndDown::newState() {
	difference = glm::translate(glm::vec3(0.f, 4.f, -5.0f));
}

void UpAndDown::update(float _time, float _deltaTime) {
	if (_time > 5) {
		finished = true;
	}
	if ((int)_time % 2 == 0) {
		difference *= glm::translate(glm::vec3(0.f, -0.05f, 0.f));
	}
	else {
		difference *= glm::translate(glm::vec3(0.f, +0.05f, 0.f));
	}
}

void UpAndDown::draw(float _time, float _deltaTime) {
	renderer->clear();
	renderer->draw(*mesh, *texture, difference);
	renderer->present(*camera);
}

void UpAndDown::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->clear();
	renderer->draw(*mesh, *texture, difference);
	renderer->present(*camera);
}

void UpAndDown::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
