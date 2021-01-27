#include "Shooter.hpp"


Shooter::Shooter() : GameState(), system(),
texturePlanet(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
textureCratetex(*graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
meshSphere(*utils::MeshLoader::get("models/sphere.obj")),
meshCrate(*utils::MeshLoader::get("models/crate.obj")),
shot(0)
{	
	for (int i = 0; i < 100; i++) {
		Entity entity;
		entities.push_back(system.createEntity(entity));

		system.addTexture(entities.back(), &textureCratetex);
		system.addMesh(entities.back(), &meshCrate);
		system.addTransform(entities.back(), glm::mat4(1.f));

		system.addPosition(entities.back(), glm::vec3( rand()%51+(-25), rand()%51+(-25), rand()%10+(-55) ));
		system.addOrientation(entities.back(), glm::quat(1.0f, 0.f, 0.f, 0.f));
		system.addScale(entities.back(), glm::vec3(1.f, 0.5f, 1.f));

		system.addVelocity(entities.back(), glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)));
		system.addAngularVelocity(entities.back(), glm::vec3(( ((double)rand()/(RAND_MAX))*2-1)*0.5, (((double)rand()/(RAND_MAX))*2-1)*0.5, (((double)rand()/(RAND_MAX))*2-1)*0.5));

		system.addAABB(entities.back(), false);
	}
}

void Shooter::newState() {
}

void Shooter::update(float _time, float _deltaTime) {
	if (_time > 100) finished = true;

	system.updateTransform(_deltaTime);
	system.updatePosition(_deltaTime);
	system.updateOrientation(_deltaTime);

	if (shot == 0 && input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		system.shootMeshWithTexure(&meshSphere, texturePlanet, entities, 50.f);
		shot = 30;
	}
	else if (shot > 0) {
		shot--;
	}
	system.updateAABB();
	int deletedCrates = system.removeIntersecting();

	for (int i = 0; i < deletedCrates; i++) {
		Entity entity;
		entities.push_back(system.createEntity(entity));
		system.addTexture(entities.back(), &textureCratetex);
		system.addMesh(entities.back(), &meshCrate);
		system.addTransform(entities.back(), glm::translate(glm::mat4(1.f), glm::vec3(-75 + ((rand() % 2) * 150), -50 + ((rand() % 2) * 100), rand() % 10 + (-55))));
		system.addPosition(entities.back(), glm::vec3(-75 + ((rand() % 2) * 150), -50 + ((rand() % 2) * 100), rand() % 10 + (-55)));
		system.addOrientation(entities.back(), glm::quat(1.0f, 0.f, 0.f, 0.f));
		system.addScale(entities.back(), glm::vec3(1.f, 0.5f, 1.f));
		system.addVelocity(entities.back(), glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)));
		system.addAngularVelocity(entities.back(), glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5));
		system.addAABB(entities.back(), false);
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

void Shooter::draw(float _time, float _deltaTime) {
	system.draw();
}

void Shooter::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Shooter::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Shooter::isFinished(){
	return false;
}

