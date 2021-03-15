#pragma once
#include "gamestate.hpp"
#include "../gui_toolkit/guitoolkit.hpp"

class MainMenu : public GameState {
public:
	MainMenu();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:

	const graphics::Texture2D& lightGrayTexture;
	System system;
	GuiToolkit guiToolkit;
	std::vector<Entity> containerElements1;
	Entity entity;
	Entity container1;
	std::string stringMainMenu;
	std::string stringExitGame;
	std::string stringGameState1;
	std::string stringGameState2;
	std::string stringGameState3;
	std::string stringGameState4;
	std::string stringGameState5;
	std::string stringGameState6;
	std::string stringGameState7;
};
