#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"
#include <stdlib.h>
#include <time.h> 
#include "../gui_toolkit/entity_creation_interface.hpp"
#include "../gui_toolkit/guitoolkit.hpp"


class Shooter : public GameState {
public:
	Shooter();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	const graphics::Texture2D& darkBlueTexture;
	const graphics::Texture2D& lightBlueTexture;
	const graphics::Texture2D& whiteTexture;
	System system;
	GuiToolkit guiToolkit;
	std::list<Entity> entities;
	Entity menuButton;
	Entity createTargetsButton;
	Entity checkBox;
	Entity textDisplay;
	Entity hitPoints;
	std::string menuString;
	std::string checkBoxString;
	std::string createTargetsString;
	std::string textDisplayString;
	std::string hitPointsString;
	int points;
	int shot;
	float timePaused;
};
