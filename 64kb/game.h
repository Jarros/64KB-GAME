#pragma once

#include "main.h"

enum class GameModes : byte {
	menu,
	spectate,
	game,
	gamefly
};

class Game
{
public: 

	GameModes mode;

	bool Debug = false;
	bool Cheat = true;
	bool Menu;
	bool Stop;

	bool GameOver = false;
};

