#pragma once
#include "gamestate.hpp"
#include "../gui_toolkit/guitoolkit.hpp"

class SliderDemo : public GameState {
public:
	SliderDemo();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	System system;
	GuiToolkit guiToolkit;
};
