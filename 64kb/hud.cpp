
#include "hud.h"
#include "Input.h"
#include "render.h"
#include "textures.h"
#include "main.h"
#include "terrain.h"
#include "game.h"
#include "player.h"
#include "textures.h"
#include "sound.h"
#include "bots.h"
#include "projectile.h"

HUD::HUD()
{
	menuselected = 0;
	slot = Slots::HANDS;
}

void HUD::PrintConsole(std::string str, eClrs eClr) {
	//MessageBox(NULL, str.c_str(), "print", MB_OK);
	if (true && chat_i == 255) {
		for (std::string& str : chat) {
			str.clear();
		}
		return;
	}
	chat_i++;
	chat_i = chat_i % 256;
	chat[chat_i] = str;
	chatClrs[chat_i] = eClr;
	return;
}


void HUD::Line2D(coord x, coord y, coord x2, coord y2, coord width)
{
	float v[] = { x,y };
	float v2[] = { x2,y2 };

	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex2fv(v);
	glVertex2fv(v2);
	glEnd();
}


bool HUD::GetFontQuad(unsigned char st, coord x, coord y)
{
	switch (st) {
	case space:
		return false;
	case number:
		return font_number[x][y];
	case letter:
		return font_letter[x][y];
	case symbol1:
		return font_symbol1[x][y];
	case symbol2:
		return font_symbol2[x][y];
	default:
		return true;
	}
}

void HUD::Symbol(unsigned char chr, coord xpos, coord ypos, coord size, bool Italics)
{
	//float v[] = {x,y};
	char chrb;

	if (chr >= symbol1 && chr < number) // ! - /
		chrb = symbol1;
	else
		if (chr >= number && chr < symbol2) // 0 - 9
			chrb = number;
		else
			if (chr >= symbol2 && chr < letter) // : - @
				chrb = symbol2;
			else
				if (chr >= letter && chr < symbol3) // A - Z
					chrb = letter;
				else
					if (chr >= symbol3 && chr < number2) // [ - '
						chrb = symbol3;
					else
						if (chr >= number2 && chr <= 122) // a - z
						{
							chr = chr - number2 + letter; chrb = letter;
						}
						else
							switch (chr) {
							case 190:
								chr = '.';
								chrb = symbol1;
								break;
							case 191:
								chr = '/';
								chrb = symbol1;
								break;
							case 32:
								chr = ' ';
								chrb = space;
							default:
								return;
							}

	char chrc = chr - chrb;
	chrc *= 5;

	for (coord x = 0; x < 5; x++)
	{
		coord xs = x * size;
		coord x2 = xpos + xs;
		//if(Italics)x2+=y;

		for (coord y = 0; y < 6; y++)
		{
			coord ys = y * size;
			coord y2 = ypos + ys;
			if (Italics)y2 += x;

			if (GetFontQuad(chrb, y, chrc + x))
			{
				glVertex2f(x2, y2);
				glVertex2f(x2 - size, y2);
				glVertex2f(x2 - size, y2 + size);
				glVertex2f(x2, y2 + size);
			}
		}
	}

}


void HUD::DrawChat(const Input& input)
{

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);

	glColor4f(0.2f, 0.2f, 0.2f, 0.1f);
	glBegin(GL_QUADS);
	//Line2D(96, SCREEN_HEIGHT - 288 + 64, 256 + 95 + 128, SCREEN_HEIGHT - 288 + 64, 16.0f);
	glVertex2i(96, SCREEN_HEIGHT - 88 + 64);
	glVertex2i(256 + 95 + 128, SCREEN_HEIGHT - 88 + 64);
	glVertex2i(256 + 95 + 128, SCREEN_HEIGHT - 288 + 64);
	glVertex2i(96, SCREEN_HEIGHT - 288 + 64);

	glEnd();

	glColor4f(1.0f, 1.0f, 0.5f, 1.0f);
	glBegin(GL_QUADS);

	//PrintLine(gamename, HSCREEN_WIDTH - 192, HSCREEN_HEIGHT - 64, 8, true);

	glColor4f(1.0f, 1.0f, 0.5f, 0.5f);

	if (menuselected < 0)menuselected = 0;
	if (menuselected > 3)menuselected = 3;

	float menuclr[] = { 1.0f,1.0f,1.0f };
	const int size = 2;

	for (int i = chat_i; i >= 0; i--)
	{
		switch (chatClrs[chat_i - i]) {
		case eClrs::default:
			menuclr[0] = 1.0f;
			menuclr[1] = 1.0f;
			menuclr[2] = 1.0f;
			break;
		case eClrs::red:
			menuclr[0] = 1.0f;
			menuclr[1] = 0.5f;
			menuclr[2] = 0.5f;
			break;
		case eClrs::green:
			menuclr[0] = 0.5f;
			menuclr[1] = 1.0f;
			menuclr[2] = 0.5f;
			break;
		case eClrs::blue:
			menuclr[0] = 0.5f;
			menuclr[1] = 0.5f;
			menuclr[2] = 1.0f;

			break;

		}
		bool sel = false;// (slot == menuselected);
		glColor4f(menuclr[0], menuclr[1], menuclr[2], sel / 2.0f + 0.5f);
		PrintLine(chat[chat_i-i], 96+16, SCREEN_HEIGHT - 288 + 64 + 7 * size * i + 16, size, sel);
	}
	if (input.enteringText)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		PrintLine("'", 96 - 8, SCREEN_HEIGHT - 288 + 64 + 7 * size * 0 - 16, 4, true);
		PrintLine(input.enteredText, 96 + 16, SCREEN_HEIGHT - 288 + 64 + 7 * size * 0 - 16, 4, true);
	}
	glEnd();
	glDisable(GL_BLEND);
}

void HUD::PrintLine(std::string st, coord xpos, coord ypos, coord size, bool Italics)
{
	coord xx = 0, space;
	space = size * (4 + 2);
	for (coord a = 0; a < st.length(); a++)
	{
		//if()
		if (st[a] == ' ') {

		}
		else {
		Symbol(st[a], xpos + xx, ypos, size, Italics);
		}
		xx += space;
	}
}

void HUD::SwitchMenu(Game& game)
{
	if (game.mode == GameModes::menu)
		return;
	//Menu=true;
	game.Menu = !game.Menu;
	game.Stop = game.Menu;
	//else
	//	Stop=false;
}


void HUD::StartGame(Game& game, Terrain& terrain, Player& player)
{
	game.Menu = 0;
	game.Stop = 0;
	game.mode = GameModes::game;
}


void HUD::DrawInventory(Player &player)
{
	static ubyte r[7], g[7], b[7];
	r[0] = 0;
	g[0] = 0;
	b[0] = 0;
	r[static_cast<int>(Slots::HANDS)] = 160;
	g[static_cast<int>(Slots::HANDS)] = 130;
	b[static_cast<int>(Slots::HANDS)] = 110;
	r[static_cast<int>(Slots::BLOCKS)] = 110;
	g[static_cast<int>(Slots::BLOCKS)] = 100;
	b[static_cast<int>(Slots::BLOCKS)] = 80;
	r[static_cast<int>(Slots::SPADE)] = 80;
	g[static_cast<int>(Slots::SPADE)] = 60;
	b[static_cast<int>(Slots::SPADE)] = 20;
	r[static_cast<int>(Slots::MACHINEGUN)] = 140;
	g[static_cast<int>(Slots::MACHINEGUN)] = 120;
	b[static_cast<int>(Slots::MACHINEGUN)] = 170;
	r[static_cast<int>(Slots::BAZOOKA)] = 90;
	g[static_cast<int>(Slots::BAZOOKA)] = 150;
	b[static_cast<int>(Slots::BAZOOKA)] = 70;
	r[6] = 0;
	g[6] = 0;
	b[6] = 0;

	for (coord a = 1; a < 6; a++)
	{
		bool HasIt = (static_cast<int>(slot) == a && player.Weapon[a].Own);

		glColor3ub(r[a] * player.Weapon[a].Own, g[a] * player.Weapon[a].Own, b[a] * player.Weapon[a].Own);
		glBegin(GL_QUADS);

		Symbol('0' + a, HSCREEN_WIDTH - 224 + a * 64, SCREEN_HEIGHT - 192, 4, HasIt);

		glEnd();
	}
}


void HUD::DrawMenu(BotManager& bots, Game& game, Player& player, Terrain& terrain, const Input& input, Sound& sound)
{

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);

	glColor4f(0.2f, 0.2f, 0.2f, 0.4f);
	Line2D(HSCREEN_WIDTH - 208, HSCREEN_HEIGHT - 16, HSCREEN_WIDTH + 304, HSCREEN_HEIGHT - 16, 16.0f);

	glColor4f(1.0f, 1.0f, 0.5f, 1.0f);
	glBegin(GL_QUADS);

	PrintLine(gamename, HSCREEN_WIDTH - 192, HSCREEN_HEIGHT - 64, 8, true);

	glColor4f(1.0f, 1.0f, 0.5f, 0.5f);

	if (menuselected < 0)menuselected = 0;
	if (menuselected > 3)menuselected = 3;

	if (input.keys[VK_DOWN].Hit) { menuselected++; sound.Beep(); }
	if (input.keys[VK_UP].Hit) { menuselected--; sound.Beep(); }

	if (input.keys[VK_RETURN].Hit) {

		switch (menuselected) {
		case 0://START
			StartGame(game, terrain, player);
			break;
		case 1://SETTINGS
			break;
		case 2://NEW MAP
			terrain.BuildScene(true, true);
			break;
		case 3://EXIT
			done = true;
			break;
		}

		sound.Explode();
	}

	const std::string Menus[4] = {
		"START",
		"SETTINGS",
		"NEW MAP",
		"EXIT" };

	const float menuclr[] = { 0.5f,1.0f,0.5f };
	const int size = 5;

	for (coord slot = 0; slot < 4; slot++)
	{
		bool sel = (slot == menuselected);
		glColor4f(menuclr[0], menuclr[1], menuclr[2], sel / 2.0f + 0.5f);
		PrintLine(Menus[slot], HSCREEN_WIDTH - 128, HSCREEN_HEIGHT + 7 * size * slot, size, sel);
	}
	glEnd();
	glDisable(GL_BLEND);
}

void HUD::DrawHUD(Player& player)
{
	if (player.GetWeapon(slot).Ammo<0 || player.GetWeapon(slot).Ammo>MaxAmmo[static_cast<int>(slot)] + 1)
		player.GetWeapon(slot).Own = false;
	if (player.GetWeapon(slot).Own == false)
		slot = static_cast<Slots>(static_cast<int>(slot) - 1);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_TEXTURE_2D);

	//glColor4f(1.0f,1.0f,1.0f,1.0f);

	glEnable(GL_BLEND);

	//HEALTH

	glColor4f(0.7f, 0.0f, 0.0f, 0.5f);
	Line2D(96 + player.HP, SCREEN_HEIGHT - 288, 256 + 95, SCREEN_HEIGHT - 288, 16.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 0.3f);
	Line2D(96, SCREEN_HEIGHT - 288, player.HP + 96, SCREEN_HEIGHT - 288, 16.0f);

	//AMMO

	glColor4f(0.6f, 0.4f, 0.0f, 0.4f);
	Line2D(96, SCREEN_HEIGHT - 236, 96 + player.GetWeapon(slot).Ammo * 256 / (MaxAmmo[static_cast<int>(slot)] + 1), SCREEN_HEIGHT - 236, 16.0f);

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	PrintLine("HEALTH", 104, SCREEN_HEIGHT - 313, 2.0f, false);
	if (player.GetWeapon(slot).Ammo > 0)PrintLine("AMMO", 104, SCREEN_HEIGHT - 261, 2.0f, false);
	glEnd();

	glColor4f(0.2f, 0.2f, 0.2f, 0.2f);

	glBegin(GL_QUADS);
	glVertex2f(HSCREEN_WIDTH - 188, SCREEN_HEIGHT - 164); // top left
	glVertex2f(HSCREEN_WIDTH + 148, SCREEN_HEIGHT - 164); // bottom left
	glVertex2f(HSCREEN_WIDTH + 148, SCREEN_HEIGHT - 196); // bottom right
	glVertex2f(HSCREEN_WIDTH - 188, SCREEN_HEIGHT - 196); // top right
	glEnd();

	glDisable(GL_BLEND);

	glColor3f(1.0f, 1.0f, 0.5f);
	glBegin(GL_QUADS);

	glEnd();

	DrawInventory(player);

	glEnable(GL_DEPTH_TEST);
}

