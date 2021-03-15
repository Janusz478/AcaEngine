#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"
#include "../gui_toolkit/entity_creation_interface.hpp"
#include "../gui_toolkit/guitoolkit.hpp"


class UpAndDown : public GameState {
public:
	UpAndDown();
	//has pure virtual functions for frame updates
	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime)override;

	//has virtual interface for state transition events
	void onResume() override;
	void onPause(float _time) override;

	//interface to mark current state as finished and to provide a new state
	void newState() override;
	bool isFinished();

private:
	System system;
	GuiToolkit guiToolkit;
	float timePaused;
	const graphics::Texture2D& darkBlueTexture;
	const graphics::Texture2D& lightBlueTexture;
	const graphics::Texture2D& whiteTexture;
	std::string menu;
	std::string textDisplayText;
	Entity menuButton;
	Entity entity;
	Entity textDisplay;
};
