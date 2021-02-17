#pragma once
#include "GameState.hpp"
#include "../entity_component_system/System.hpp"


class UpAndDown : public GameState {
public:
	UpAndDown();
	//has pure virtual functions for frame updates
	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime)override;

	//has virtual interface for state transition events
	void onResume() override;
	void onPause() override;

	//interface to mark current state as finished and to provide a new state
	void newState() override;
	bool isFinished();

private:
	Entity entity;
	System system;
	const graphics::Texture2D& texture;
	const graphics::Mesh meshSphere;
};
