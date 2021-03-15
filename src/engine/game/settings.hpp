#pragma once
#include "gamestate.hpp"
#include "../gui_toolkit/guitoolkit.hpp"

class Settings : public GameState {
public:
	Settings();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	const graphics::Texture2D& darkBlueTexture;
	const graphics::Texture2D& lightBlueTexture;
	const graphics::Texture2D& lightGrayTexture;
	const graphics::Texture2D& greenTexture;
	const graphics::Texture2D& lightGreenTexture;
	const graphics::Texture2D& redTexture;
	const graphics::Texture2D& lightRedTexture;
	System system;
	GuiToolkit guiToolkit;
	std::string stringSettings;
	std::string stringBackground;
	std::string stringColor;
	std::string stringHelp;
	std::string stringName;
	std::string stringBack;	
	std::string stringRed;
	std::string stringGreen;
	std::string stringBlue;
	std::string stringCurrentName;
	std::string stringChangeName;
	std::string stringHelpText1;
	std::string stringHelpText2;
	std::string stringHelpText3;
	std::string stringHelpText4;
	std::string stringVSync;
	Entity entity;
	Entity buttonBackEntity;
	std::vector<Entity> containerElements0;
	Entity container0;
	std::vector<Entity> containerElements1;
	Entity container1;
	std::vector<Entity> containerElements2;
	Entity container2;
	std::vector<Entity> containerElements3;
	Entity container3;
	std::vector<Entity> containerElements4;
	Entity container4;
	std::vector<Entity> containerElements5;
	Entity container5;
	std::vector<Entity> containerElements6;
	Entity container6;
	std::vector<Entity> containerElements7;
	Entity container7;
	std::vector<Entity> containerElements8;
	Entity container8;
};
