#pragma once
#include <engine/utils/GameState.hpp>

class FreeFall : public GameState {
public:
	FreeFall();
	//has pure virtual functions for frame updates
	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	//has virtual interface for state transition events
	void onResume() override;
	void onPause() override;

	//interface to mark current state as finished and to provide a new state
	void newState() override;
	bool isFinished();

private:
	float a;
	float v;
};
