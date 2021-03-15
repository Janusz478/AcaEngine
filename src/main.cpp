#pragma once
#include <engine/game/game.hpp>
#include <engine/game/spring.hpp>
#include <engine/game/upanddown.hpp>
#include <engine/game/freefall.hpp>
#include <engine/game/shooter.hpp>
#include <engine/game/lights.hpp>
#include <engine/game/mainmenu.hpp>
#include <engine/game/settings.hpp>


// CRT's memory leak detection
#ifndef NDEBUG 
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <iostream>

#endif

int main(int argc, char* argv[])
{
#ifndef NDEBUG 
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(769028);
#endif
#endif

	Game game;
	game.addState(std::make_shared<Settings>());
	game.addState(std::make_shared<UpAndDown>());
	game.addState(std::make_shared<FreeFall>());
	game.addState(std::make_shared<Spring>());
	game.addState(std::make_shared<Lights>());
	game.addState(std::make_shared<Shooter>());
	game.run(std::make_shared<MainMenu>());
	
	graphics::FontManager::clear();
	return EXIT_SUCCESS;
}
