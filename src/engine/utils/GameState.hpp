#pragma once
#include <engine/graphics/renderer/meshrenderer.hpp>
#include <engine/graphics/core/texture.hpp>
#include <engine/graphics/renderer/mesh.hpp>
#include <engine/graphics/camera.hpp>

class GameState {
public:
	GameState() : finished(false), difference(glm::mat4(1.f)) {
		using namespace graphics;
		Sampler sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR);
		const Texture2D& texturePlanet = *Texture2DManager::get("textures/planet1.png", sampler);
		texture = &texturePlanet;

		Mesh* meshPlanet = new Mesh(*utils::MeshLoader::get("models/sphere.obj"));
		mesh = meshPlanet;

		MeshRenderer* renderer = new MeshRenderer;
		this->renderer = renderer;

		Camera* camera = new Camera(45.f, 0.1f, 10000.f);
		this->camera = camera;
	}

	//TODO: Destruktor
	~GameState() {
		delete mesh;
		delete renderer;
		delete camera;
		utils::MeshLoader::clear();
	}


	//has pure virtual functions for frame updates
	virtual void update(float _time, float _deltaTime) = 0;
	virtual void draw(float _time, float _deltaTime) = 0;

	//has virtual interface for state transition events
	virtual void onResume() = 0;
	virtual void onPause() = 0;

	//interface to mark current state as finished and to provide a new state
	virtual void newState() = 0;
	bool isFinished() {
		return finished;
	}

	bool finished;
	const graphics::Texture2D* texture;
	graphics::Mesh* mesh;
	graphics::Camera* camera;
	graphics::MeshRenderer* renderer;
	glm::mat4 difference;
};
