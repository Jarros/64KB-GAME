#pragma once

#include "main.h"
#include "hud.h"
#include "input.h"
#include "network.h"

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

	std::string sConsoleCommands[256] =
	{
		"null",
		"/HOST",
		"/CONNECT",
	};

	enum eConsoleCommands: int{
		$null,
		$host,
		$connect,
	};

	int hashit(std::string s) {
		for (int i = 0; i < 256; i++)
		{
			if ((s) == sConsoleCommands[i]) 
				return i;
		}
		return 0;
	}

	void procCommand(std::string str) {
		std::string firstWord = str;
		std::string arg = "";
		if (str.find(" ")!=-1) {
			firstWord = str.substr(0, str.find(" "));
			arg=str.substr(str.find(" ")+1, str.size());
		}
		
		const char* c_str = firstWord.c_str();
		switch (hashit(c_str)){
		case $host:
			hud_->PrintConsole("trying to host...");
			network->networkMain();
				break;
		case $connect:
			hud_->PrintConsole("trying to connect...");
			network->networkMainClient(arg);
			//NetworkMainClient(arg, hud_);
			//MessageBox(0,"test","testset", 0);;
				break;
		default:
			break;
		}
	}

	GameModes mode = GameModes::menu;

	bool Debug = false;
	bool Cheat = false;
	bool Menu = true;
	bool Stop = true;

	bool GameOver = false;
};

