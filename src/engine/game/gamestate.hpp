#pragma once

class GameState {
public:
	GameState() : 
		finished(false) {}

	virtual ~GameState() {}

	//has pure virtual functions for frame updates
	virtual void update(float _time, float _deltaTime) = 0;
	virtual void draw(float _time, float _deltaTime) = 0;

	//has virtual interface for state transition events
	virtual void onResume() = 0;
	virtual void onPause(float _time) = 0;

	//interface to mark current state as finished and to provide a new state
	virtual void newState() = 0;
	bool isFinished() {
		return finished;
	}

	bool finished;
};
