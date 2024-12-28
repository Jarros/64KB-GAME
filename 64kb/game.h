#pragma once

#include "main.h"
#include "hud.h"
#include "input.h"

enum class GameModes : byte {
	menu,
	spectate,
	game,
	gamefly
};

class Game
{
private: 
	HUD *hud_ = nullptr;
	Input* input_ = nullptr;
	Terrain* terrain_ = nullptr;
public: 

	Game::Game(HUD* hud, Input* input, Terrain * terrain) {
		hud_ = hud;
		input_ = input;
		terrain_ = terrain;
	}

	GameModes mode = GameModes::menu;

	bool Debug = false;
	bool Cheat = false;
	bool Menu = true;
	bool Stop = true;

	bool GameOver = false;
};

