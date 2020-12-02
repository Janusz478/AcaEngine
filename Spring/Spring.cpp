
#include "Spring.hpp"
#include <engine\graphics\core\texture.cpp>
#include <engine\graphics\renderer\mesh.cpp>
#include <engine\graphics\renderer\meshrenderer.hpp>
#include <iostream>



Spring::Spring() {

}

void Spring::newState() {
	difference = glm::mat4(1.f);
}

void Spring::update(float _time, float _deltaTime) {
	times = times + 1;
	
}

void Spring::draw(float _time, float _deltaTime) {
	float velo = 300 * sin((_time) * 3.14 / 180) * _deltaTime;
	if (times < 50) {
		difference = difference * glm::translate(glm::vec3(0.f, 0.f, -1.1f));
	}
	else {

		difference = difference * glm::translate(glm::vec3(velo, 0.f, 0.f));
	}


	renderer->clear();
	renderer->draw(*mesh, *texture, difference);
	renderer->present(*camera);
}

void Spring::onResume() {
	float t = 0;
}

void Spring::onPause() {
	float t = 0;
}

//auto test_start = std::chrono::steady_clock::now();
//float test_time = std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - test_start).count();