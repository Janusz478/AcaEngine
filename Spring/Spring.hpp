#include <engine/utils/GameState.hpp>
#pragma once
class Spring : public GameState {
public:
	Spring();
	//has pure virtual functions for frame updates
	void update(float _time, float _deltaTime);
	void draw(float _time, float _deltaTime);

	//has virtual interface for state transition events
	void onResume();
	void onPause();

	//interface to mark current state as finished and to provide a new state
	void newState();
	bool isFinished();
};
