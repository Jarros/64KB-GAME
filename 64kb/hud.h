#pragma once
#include "main.h"

class Input;
class Textures;
class Player;
class Terrain;
class Game;
class Render;
class HUD;
class Sound;
class BotManager;
class Projectile;
enum class Slots : byte;

class HUD {
public:

	int menuselected;

	Slots slot;
	HUD::HUD();

	enum class eChatClr :unsigned char {
		default, red, green, blue
	};

	std::string chat[256]{""};
	eChatClr chatClrs[256];
	int chat_i = 0;

	void PrintConsole(std::string str, eChatClr eClr = eChatClr::default);

	void Line2D(coord x, coord y, coord x2, coord y2, coord width);
	//         ASCII !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! http://www.asciitable.com/index/asciifull.gif
	enum symboltype
	{
		space = 32,
		number = 48,  //48-57
		letter = 65,  //65-90
		symbol1 = 33, //33-47
		symbol2 = 58, //58-64
		symbol3 = 91, //91-96
		number2 = 97	//97-122
	};

	bool GetFontQuad(unsigned char st, coord x, coord y);

	void Symbol(unsigned char chr, coord xpos, coord ypos, coord size, bool Italics);

	void DrawChat(const Input& input);

	void PrintLine(std::string st, coord xpos, coord ypos, coord size, bool Italics);

	void SwitchMenu(Game& game);

	void StartGame(Game& game, Terrain& terrain, Player& player);

	void DrawInventory(Player& player);

	void DrawMenu(BotManager& bots, Game& game, Player& player, Terrain& terrain, const Input& input, Sound& sound);

	void DrawHUD(Player& player);
};



// A   B	C	D	E	F	G	H	I	J	K	L	M	N	O	P	Q	R	S	T	U	V	W	X	Y	Z


#define _ false
#define X true

//!"#$%^'()*+,-./
const bool font_symbol1[6][5 * 25] = {

	{ _,_,X,_,_,  X,X,X,_,_,  _,X,X,X,_,  X,X,X,_,_,  X,X,X,X,_,  X,X,X,X,_,  X,X,X,X,X,  X,_,_,X,_,  _,X,X,X,_,  _,X,X,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,X,  _,_,_,_,_,  _,_,_,_,_,  X,X,X,X,_,  X,X,X,X,_,  X,X,X,X,_,  _,X,X,X,_,  X,X,X,X,X,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  X,_,_,X,_ },
	{ _,_,X,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,X,X,X,X,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,X,_,_,  X,_,_,_,_,  X,X,_,X,X,  _,_,_,_,_,  _,_,_,_,X,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  X,_,_,X,_ },
	{ _,_,X,_,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,_,X,_,  X,X,X,_,_,  X,X,X,_,_,  X,X,X,X,X,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,X,_,X,  _,_,_,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,X,X,X,_,  X,X,X,_,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  _,X,X,_,_ },
	{ _,_,X,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,X,X,X,X,  X,X,X,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,X,  _,_,_,_,_,  _,_,X,_,_,  X,X,X,X,_,  X,_,_,X,_,  X,X,_,_,_,  _,_,_,X,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,X,_,X,  _,X,X,_,_ },
	{ _,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,X,X,X,X,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  _,_,X,X,_,  _,X,_,_,_,  X,_,_,_,_,  X,_,X,_,_,  X,_,X,_,_,  _,_,_,X,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,X,_,X,  X,_,_,X,_ },
	{ _,_,X,_,_,  X,X,X,X,_,  _,X,X,X,_,  X,X,X,_,_,  X,X,X,X,_,  X,_,_,_,_,  X,X,X,X,X,  X,_,_,X,_,  _,X,X,X,_,  X,X,X,_,_,  X,_,_,X,_,  X,X,X,X,_,  X,_,_,_,X,  _,_,X,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,X,_,X,_,  X,_,_,X,_,  X,X,X,_,_,  _,_,X,_,_,  _,X,X,_,_,  _,X,X,_,_,  _,X,_,X,_,  X,_,_,X,_ }
};

//!"#$%^'()*+,-./
const bool font_symbol2[6][5 * 25] = {

	{ _,X,X,_,_,  X,X,X,_,_,  _,X,X,X,_,  X,X,X,_,_,  X,X,X,X,_,  X,X,X,X,_,  _,X,X,X,_,  X,_,_,X,_,  _,X,X,X,_,  _,X,X,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,X,  _,_,_,_,_,  _,_,_,_,_,  X,X,X,X,_,  X,X,X,X,_,  X,X,X,X,_,  _,X,X,X,_,  X,X,X,X,X,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  X,_,_,X,_ },
	{ _,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,X,_,_,  X,_,_,_,_,  X,X,_,X,X,  _,_,_,_,_,  _,_,_,_,X,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  X,_,_,X,_ },
	{ _,_,_,_,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,_,X,_,  X,X,X,_,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,X,_,X,  _,_,_,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,X,X,X,_,  X,X,X,_,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  _,X,X,_,_ },
	{ _,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,X,X,_,  X,X,X,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,X,  _,_,_,_,_,  _,_,X,_,_,  X,X,X,X,_,  X,_,_,X,_,  X,X,_,_,_,  _,_,_,X,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,X,_,X,  _,X,X,_,_ },
	{ _,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  _,_,X,X,_,  _,X,_,_,_,  X,_,_,_,_,  X,_,X,_,_,  X,_,X,_,_,  _,_,_,X,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,X,_,X,  X,_,_,X,_ },
	{ _,X,X,_,_,  X,X,X,X,_,  _,X,X,X,_,  X,X,X,_,_,  X,X,X,X,_,  X,_,_,_,_,  _,X,X,X,_,  X,_,_,X,_,  _,X,X,X,_,  X,X,X,_,_,  X,_,_,X,_,  X,X,X,X,_,  X,_,_,_,X,  _,_,X,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,X,_,X,_,  X,_,_,X,_,  X,X,X,_,_,  _,_,X,_,_,  _,X,X,_,_,  _,X,X,_,_,  _,X,_,X,_,  X,_,_,X,_ }
};


const bool font_letter[6][5 * 27] = {
	{ _,X,X,_,_,  X,X,X,_,_,  _,X,X,X,_,  X,X,X,_,_,  X,X,X,X,_,  X,X,X,X,_,  _,X,X,X,_,  X,_,_,X,_,  _,X,X,X,_,  _,X,X,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,X,  X,_,_,X,_,  X,X,X,X,_,  X,X,X,X,_,  X,X,X,X,_,  X,X,X,X,_,  _,X,X,X,_,  X,X,X,X,X,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  X,_,_,X,_,  X,_,_,X,_,  X,X,X,X,_ },
	{ X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,X,_,_,  X,_,_,_,_,  X,X,_,X,X,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  X,_,_,X,_,  X,_,_,X,_,  _,_,_,X,_ },
	{ X,_,_,X,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,_,X,_,  X,X,X,_,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,X,X,_,_,  X,_,_,_,_,  X,_,X,_,X,  X,X,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,X,X,X,_,  X,X,X,_,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  _,X,X,_,_,  X,X,X,X,_,  _,_,X,_,_ },
	{ X,X,X,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,X,X,_,  X,X,X,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,X,  X,_,X,X,_,  X,_,_,X,_,  X,X,X,X,_,  X,_,_,X,_,  X,X,_,_,_,  _,_,_,X,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,X,_,X,  _,X,X,_,_,  _,_,_,X,_,  _,X,_,_,_ },
	{ X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,_,_,_,  X,_,_,X,_,  X,_,_,X,_,  _,_,X,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,X,  X,_,_,X,_,  X,_,_,X,_,  X,_,_,_,_,  X,_,X,_,_,  X,_,X,_,_,  _,_,_,X,_,  _,_,X,_,_,  X,_,_,X,_,  X,_,_,X,_,  X,_,X,_,X,  X,_,_,X,_,  _,_,X,_,_,  X,_,_,_,_ },
	{ X,_,_,X,_,  X,X,X,X,_,  _,X,X,X,_,  X,X,X,_,_,  X,X,X,X,_,  X,_,_,_,_,  _,X,X,X,_,  X,_,_,X,_,  _,X,X,X,_,  X,X,X,_,_,  X,_,_,X,_,  X,X,X,X,_,  X,_,_,_,X,  X,_,_,X,_,  X,X,X,X,_,  X,_,_,_,_,  X,X,_,X,_,  X,_,_,X,_,  X,X,X,_,_,  _,_,X,_,_,  _,X,X,_,_,  _,X,X,_,_,  _,X,_,X,_,  X,_,_,X,_,  X,X,_,_,_,  X,X,X,X,_ }
};


const bool font_number[6][5 * 10] = {
	{ _,X,X,_,_,  _,_,X,_,_,  X,X,X,X,_,  X,X,X,X,_,  _,_,X,X,_,  X,X,X,X,_,  _,_,X,X,_,  X,X,X,X,_,  _,X,X,_,_,  _,X,X,_,_   },
	{ X,_,_,X,_,  _,X,X,_,_,  _,_,_,X,_,  _,_,_,X,_,  _,X,_,X,_,  X,_,_,_,_,  _,X,_,_,_,  _,_,_,X,_,  X,_,_,X,_,  X,_,_,X,_   },
	{ X,_,_,X,_,  _,_,X,_,_,  _,_,X,X,_,  _,X,X,_,_,  X,_,_,X,_,  X,X,X,X,_,  X,_,_,_,_,  _,_,X,_,_,  _,X,X,_,_,  X,X,X,X,_   },
	{ X,_,_,X,_,  _,_,X,_,_,  X,X,_,_,_,  _,_,_,X,_,  X,X,X,X,_,  _,_,_,X,_,  X,X,X,X,_,  _,X,_,_,_,  X,_,_,X,_,  _,_,_,X,_   },
	{ X,_,_,X,_,  _,_,X,_,_,  X,_,_,_,_,  _,_,_,X,_,  _,_,_,X,_,  _,_,_,X,_,  X,_,_,X,_,  _,X,_,_,_,  X,_,_,X,_,  _,_,X,_,_   },
	{ _,X,X,_,_,  _,X,X,X,_,  X,X,X,X,_,  X,X,X,X,_,  _,_,_,X,_,  X,X,X,X,_,  X,X,X,X,_,  _,X,_,_,_,  _,X,X,_,_,  X,X,_,_,_   }
};


#undef _
#undef X