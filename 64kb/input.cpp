#include "main.h"
#include "Input.h"
#include "render.h"
#include "textures.h"
#include "terrain.h"
#include "game.h"
#include "player.h"
#include "textures.h"
#include "hud.h"
#include "sound.h"
#include "bots.h"
#include "projectile.h"

void Input::ClearKeys()
{
	coord a;
	for (a = 0; a < 256; a++)
	{
		keys[a].Hit = false;
		keys[a].Rel = false;
	}
	LMB.Hit = false;
	LMB.Rel = false;

	keys[VK_SPACE].TICK++;

	LMB.TICK++;

	if(windowFocused)
	SetCursorPos(SCREEN_CENTER_X / 2, SCREEN_CENTER_Y / 2);
}

void Input::Chat() {
	enteringText = true;
}

void Input::ChatEnter(HUD &hud, Game& game) {
	//hud.chat_i++;
	//hud.chat[hud.chat_i] = enteredText;
	if(enteredText[0]=='/')
		game.procCommand(enteredText);
	else {
		network->sendText(enteredText.c_str());
	}
	enteredText.clear();
	enteringText = false;
}


bool Input::Hit(Terrain& terrain, const Player& player, BotManager &bots, float dist, float precision, int damage, bool destroyblocks, float pushforce, float maxdist, bool blood)
{
	float shit = dist * precision;
	float clr = 0.f;
	float x3 = 0.f, y3 = 0.f, z3 = 0.f, z2 = 0.f, x2 = 0.f, y2 = 0.f, z = 0.f, a = 0.f;
	GLint oldposx = 0, oldposz = 0, xi = 0, yi = 0, zi = 0;
	terrain.clr0rgb[oldposx][oldposz].r = clr;
	x3 = sin(player.yrot * PIover180) * cos(player.xrot * PIover180);
	y3 = -sin(player.xrot * PIover180);
	z3 = -cos(player.yrot * PIover180) * cos(player.xrot * PIover180);
	for (a = 0; a < shit; a++)
	{
		x2 = x3 * (a / precision) + player.xpos;
		y2 = y3 * (a / precision) + player.ypos;
		z2 = z3 * (a / precision) + player.zpos;
		xi = (int)(x2);
		yi = (int)(y2);
		zi = (int)(z2);

		coord xs = (int)(x2 / 2.0f + 0.25), ys = (int)(y2 / 2.0f + 0.25), zs = (int)(z2 / 2.0f + 0.25);
		if (destroyblocks && terrain.OnLevel(xs, ys, zs) && terrain.cubescene[xs][ys][zs].type != Objects::NONE)
		{

			terrain.cubescene[xs][ys][zs].type = Objects::NONE;
			return true;
		}
		for (coord b = 0; b < botsnum; b++)
		{
			if (Dist3D(bots.bot[b].x, bots.bot[b].y, bots.bot[b].z, x2, y2, z2) < maxdist)
			{
				bots.bot[b].HPb -= damage;
				bots.bot[b].x += x3 * pushforce;

				bots.bot[b].z += z3 * pushforce;

				if (blood)
				{
					terrain.clr0rgb[(int)bots.bot[b].x][(int)bots.bot[b].z].r = 140 + rnd(0, 15); terrain.clr0rgb[(int)bots.bot[b].x][(int)bots.bot[b].z].g = 80; terrain.clr0rgb[(int)bots.bot[b].x][(int)bots.bot[b].z].b = 30;
				}

				bots.bot[b].randyr += (rnd(-3, 3)) / 16.0f;
				return true;
			}
		}
	}
	return false;
}

bool Input::Raytrace(Terrain& terrain, const Player& player, int& xout, int& zout)
{

	float clr=0.f;
	float x3 = 0.f, y3 = 0.f, z3 = 0.f, z2 = 0.f, x2 = 0.f, y2 = 0.f, z = 0.f, a = 0.f;
	GLint oldposx = 0, oldposz = 0, xi = 0, yi = 0, zi = 0;
	terrain.clr0rgb[oldposx][oldposz].r = clr;
	x3 = sin(player.yrot * PIover180) * cos(player.xrot * PIover180);
	y3 = -sin(player.xrot * PIover180);
	z3 = -cos(player.yrot * PIover180) * cos(player.xrot * PIover180);
	for (a = 0; a < 512; a++)
	{
		x2 = x3 * (a / 4.0) + player.xpos;
		y2 = y3 * (a / 4.0) + player.ypos;
		z2 = z3 * (a / 4.0) + player.zpos;
		xi = (int)(x2);
		yi = (int)(y2);
		zi = (int)(z2);

		if (xi < terx && zi < terz && xi>0 && zi>0 && ((int)(terrain.scene[xi][zi].h) == yi || terrain.cubescene[xi / 2][(int)(yi / 2)][zi / 2].type != Objects::NONE))
		{
			xout = xi;
			zout = zi;
			return true;
		}
	}
	return false;
}

void Input::Dig(Terrain& terrain, Player& player, const HUD& hud, Sound& sound, float level, bool addcube, bool debug, bool weak)
{
	int xo = 0, zo = 0;




	if (Raytrace(terrain, player, xo, zo))
	{
		if (!debug)
		{
			if (Dist3D(player.xpos, player.ypos, player.zpos, xo, terrain.scene[xo][zo].h, zo) < 10.0f)
			{
				if (!weak || terrain.scene[xo][zo].type == Objects::GRASS || terrain.scene[xo][zo].type == Objects::SAND)
				{
					terrain.scene[xo][zo].h += level;
					terrain.scene[xo + 1][zo].h += level;
					terrain.scene[xo][zo - 1].h += level;
					terrain.scene[xo][zo + 1].h += level;
					terrain.scene[xo - 1][zo].h += level;
				}

				if (terrain.IsGrass(terrain.scene[xo][zo]))		terrain.scene[xo][zo].type = Objects::GRASS;
				if (terrain.IsGrass(terrain.scene[xo + 1][zo]))	terrain.scene[xo + 1][zo].type = Objects::GRASS;
				if (terrain.IsGrass(terrain.scene[xo][zo - 1]))		terrain.scene[xo][zo - 1].type = Objects::GRASS;
				if (terrain.IsGrass(terrain.scene[xo][zo + 1]))	terrain.scene[xo][zo + 1].type = Objects::GRASS;
				if (terrain.IsGrass(terrain.scene[xo - 1][zo]))		terrain.scene[xo - 1][zo].type = Objects::GRASS;
			}
		}

		coord h = std::clamp((int)((terrain.scene[xo][zo].h + terrain.scene[xo][zo + 1].h + terrain.scene[xo + 1][zo + 1].h + terrain.scene[xo + 1][zo].h) / 8.0) + 1, 0, 128);
		if (player.ypos - CAM_HEIGHT - h * 2 > 4.0f)
		{
			h = (int)((player.ypos - CAM_HEIGHT) / 2.0f);
		}

		if (Dist3D(player.xpos, player.ypos, player.zpos, xo, h * 2.0f, zo) < 10.0f)
		{
			xo = (xo + 1) / 2;
			zo = (zo + 1) / 2;
			if (xdeb != 0 && addcube)
			{
				if (!terrain.AddCube(xo, h, zo, Objects::CONCRETE, true))
					if (!terrain.AddCube(xo, h + 1, zo, Objects::CONCRETE, true))
						if (!terrain.AddCube(xo, h + 2, zo, Objects::CONCRETE, true))
							terrain.AddCube(xo, h + 3, zo, Objects::CONCRETE, true);
			}
			if (debug)
			{

				for (coord h2 = 0; h2 < 4; h2++)
				{
					if (terrain.cubescene[xo][h + h2][zo].type == Objects::NONE)
					{
						xdeb = xo; ydeb = h + h2; zdeb = zo;
						break;
					}
				}
				if (terrain.cubescene[xo][ydeb - 1][zo].type == Objects::CRATE)
				{
					if (LMB.Hit)
					{
						terrain.cubescene[xo][ydeb - 1][zo].type = Objects::NONE;
						coord b = rnd(2, 5);
						player.AddAmmo(b, MaxAmmo[b] / 4);
						sound.Beep();
						xdeb = 0; ydeb = 0; zdeb = 0;
						return;
					}
					ydebspec = ydeb - 1;
				}
				else
					ydebspec = 0;
			}

		}
		if (hud.slot != Slots::BLOCKS)
		{
			xdeb = 0; ydeb = 0; zdeb = 0;
		}
	}
}
void Input::SMGShoot(Sound& sound, Terrain& terrain, Player& player, BotManager& bots, float damage)
{
	int xo=0, zo=0;
	smgtick_++;
	if (smgtick_ % 5 == 0)
	{
		player.GetWeapon(Slots::MACHINEGUN).Ammo -= 1;
		sound.Pickup();
		Hit(terrain, player, bots, 64.0f, 4.0f, 16, true, 1.0f, 2.0f, true);
	}

	if (Raytrace(terrain, player, xo, zo))
	{
		if (Dist3D(player.xpos, player.ypos, player.zpos, xo, terrain.scene[xo][zo].h, zo) < 128.0f)
		{
			terrain.scene[xo][zo].h += rndf() - 0.5f;
			if (terrain.clr0rgb[xo][zo].r > 32.0f)terrain.clr0rgb[xo][zo].r -= 2.0f + 2 * rndf();
			if (terrain.clr0rgb[xo][zo].g > 32.0f)terrain.clr0rgb[xo][zo].g -= 2.0f + 2 * rndf();
			if (terrain.clr0rgb[xo][zo].b > 32.0f)terrain.clr0rgb[xo][zo].b -= 2.0f + 2 * rndf();
			if (terrain.scene[xo][zo].type >= Objects::GRASS1 && terrain.scene[xo][zo].type <= Objects::GRASS3)
				terrain.scene[xo][zo].type = Objects::GRASS;
		}
		coord h = (int)((terrain.scene[xo][zo].h + terrain.scene[xo][zo + 1].h + terrain.scene[xo + 1][zo + 1].h + terrain.scene[xo + 1][zo].h) / 8.0) + 1;
		if (Dist3D(player.xpos, player.ypos, player.zpos, xo, terrain.scene[xo][zo].h, zo) < 10.0f)
		{
			xo /= 2;
			zo /= 2;
		}
	}
}
void Input::Jump(const Player &player)
{
	accel_ += 0.5 / (player.InWater() + 1);
	state_ = State::inair;
}
void Input::Gravity(Player& player, const Terrain &terrain)
{
	if (state_ == State::inair)
	{
		int xx2 = (int)(player.xpos / 2.0 + 0.5);
		int zz2 = (int)(player.zpos / 2.0 + 0.5);
		int yy2 = (int)((player.ypos - CAM_HEIGHT) / 2.0 + 0.5);

		accel_ -= GRAVITY / (player.InWater() + 1);

		if (accel_ < 0.0 )
		{
			if (((terrain.scene[(int)(player.xpos)][(int)(player.zpos)].h - player.ypos) >= -CAM_HEIGHT || 
				terrain.cubescene[int(player.xpos + 0.5) / 2][int(player.ypos) / 2 - 3][int(player.zpos + 0.5) / 2].type == Objects::CONCRETE) && accel_ < 0.0)
			{
				accel_ = 0; state_ = State::walking;
			}
		}

		player.ypos += accel_;
	}
}


void Input::Collision(Player& player, const Terrain& terrain)
{
	float finalX = 0.0;
	float finalZ = 0.0;
	//int x=int(xpos);

	switch (state_)
	{
	//case State::walking:
	default:

		if (true) {
			if (terrain.cubescene[int(player.xpos+0.5) / 2][int(player.ypos) / 2][int(player.zpos+0.5 - 0.5) / 2].type == Objects::CONCRETE)
			{
				if(finalmove_.GetY()<0.0f)
					finalmove_.MakeFromXY(finalmove_.GetX(), 0);
			}
			if (terrain.cubescene[int(player.xpos + 0.5) / 2][int(player.ypos) / 2][int(player.zpos + 0.5 + 1.0) / 2].type == Objects::CONCRETE)
			{
				if (finalmove_.GetY() > 0.0f)
					finalmove_.MakeFromXY(finalmove_.GetX(), 0);
			}
		}
		if(true)
		if (terrain.cubescene[int(player.xpos + 0.5 + 1.0) / 2][int(player.ypos) / 2][int(player.zpos + 0.5) / 2].type == Objects::CONCRETE)
		{
			if (finalmove_.GetX() > 0.0f)
				finalmove_.MakeFromXY(0.000001, finalmove_.GetY()); // TODO
		}
		if (terrain.cubescene[int(player.xpos + 0.5 - 0.5) / 2][int(player.ypos) / 2][int(player.zpos + 0.5) / 2].type == Objects::CONCRETE)
		{
			if (finalmove_.GetX() < 0.0f)
				finalmove_.MakeFromXY(-0.000001, finalmove_.GetY()); // TODO
		}

		break;
	}


}

/*

void Input::UpCollision(Player& player, const Terrain& terrain)
{
	float finalX = 0.0;
	float finalZ = 0.0;
	//int x=int(xpos);

	switch (state_)
	{
	case State::walking:

		if (true) {
			if (terrain.cubescene[int(player.xpos + 0.5) / 2][int(player.ypos) / 2][int(player.zpos + 0.5 - 0.5) / 2].type == Objects::CONCRETE)
			{
				if (finalmove_.GetY() < 0.0f)
					finalmove_.MakeFromXY(finalmove_.GetX(), 0);
			}
		}
		break;
	}


}
*/


void Input::SmoothCamera(Terrain& terrain, Player& player, Game& game)
{
	static float yrotating;
	realcamera_[1] = terrain.SafeScene((int)(player.xpos), (int)(player.zpos)).h + CAM_HEIGHT;
	fincamera_[1] = (fincamera_[1] * 5+ realcamera_[1]) / 6.0f;
	int cubeY;
		switch (state_)
		{
		case State::walking:
			cubeY = int(player.ypos) / 2 - 3;
			if (terrain.cubescene[int(player.xpos + 0.5) / 2][cubeY][int(player.zpos + 0.5) / 2].type == Objects::CONCRETE)
			{
				player.ypos = cubeY*2+6;
			}else
				player.ypos = fincamera_[1];
			player.movespeed = 0.2;
			break;

		case State::inair:
			break;
		}
	
	//Collision(player, terrain);
}

void Input::ProcChr(HUD& hud, Game& game) {
	for (unsigned char chr = 0; chr < 255; chr++) {
		if (keys[chr].Hit) {
			unsigned char newChr = chr;
			switch (chr) {
			case 13:
				ChatEnter(hud, game);
				return;
			case 190:
				newChr = '.';
				break;
			case 191:
				newChr = '/';
				break;
			case 32:
				newChr = ' ';
				break;
			case 186:
				newChr = ':';
				break;
			default:
				break;
			}
			std::string str{ (char)newChr };
			enteredText.append(str);
		}
	}

}

void Input::Check(BotManager& bots, Sound& sound, Terrain& terrain, Player& player, HUD& hud, Game& game)
{

	if (game.GameOver && !deadonce_)sound.Dead();

	if (player.HP<20 || player.HP>255)
	{
		game.GameOver = true; sound.Dead();
	}
	if (!game.GameOver)
	{
		float x3, y3, z3, z3h, y3h, x3h;
		if (game.mode == GameModes::game)
		{
			if (enteringText) {
				ProcChr(hud, game);
				//ClearKeys();
				//return;
			}
			else {


			Dig(terrain, player, hud, sound, 0.0f, false, true);

			if (keys[VK_UP].Hit && game.Cheat) {
				terrain.BuildScene(true, true);
			}
			if (keys[VK_DOWN].Hit && game.Cheat) {
				big_--;
				terrain.BuildScene(true, true);
			}
			if (keys[VK_SPACE].Hit && keys[VK_SPACE].TICK > 20) {
				keys[VK_SPACE].TICK = 0;
				if (game.mode == GameModes::game && state_ == State::walking)
				{
					Jump(player);
				}
			}
			if (keys[VK_TAB].press && game.Cheat) {
				terrain.Smooth();
				terrain.BuildScene(false, true);
			}

			//if (keys['0'].Hit)
			//	hud.slot = Slots::NONE;
			if (keys['1'].Hit)
			{
				hud.slot = Slots::HANDS; sound.Beep();
			}
			if (keys['2'].Hit)
			{
				hud.slot = Slots::BLOCKS; sound.Beep();
			}
			if (keys['3'].Hit && player.Weapon[3].Own)
			{
				hud.slot = Slots::SPADE; sound.Beep();
			}
			if (keys['4'].Hit && player.Weapon[4].Own)
			{
				hud.slot = Slots::MACHINEGUN; sound.Beep();
			}
			if (keys['5'].Hit && player.Weapon[5].Own)
			{
				hud.slot = Slots::BAZOOKA; sound.Beep();
			}
			/*
			if (keys['6'].Hit)
				hud.slot = 6;
			if (keys['7'].Hit)
				hud.slot = 7;
			if (keys['8'].Hit)
				hud.slot = 8;
			if (keys['9'].Hit)
				hud.slot = 9;
			*/
			//if(keys['R'].press && Cheat){
			//	Terrain.JustMakeItBigger();
			//	BuildScene(false,true);}
			//if(keys['deltaTick'].press && Cheat){
			//	Terrain.JustMakeItSmaller();
			//	BuildScene(false,true);}
			if (LMB.press)
			{
				switch (hud.slot)
				{
				case Slots::HANDS:
					break;
				case Slots::SPADE:
					Dig(terrain, player, hud, sound, 0.1f, false, false, true);
					break;
				case Slots::MACHINEGUN:
					SMGShoot(sound, terrain, player, bots, 1.0f);
					break;
				case Slots::BLOCKS:
					break;
				}
			}
			if (RMB.press)
			{
				switch (hud.slot)
				{
				case Slots::HANDS:
					break;
				case Slots::SPADE:
					Dig(terrain, player, hud, sound, -0.1f, false, false, true);
					break;
				case Slots::BLOCKS:
					break;
				}
			}
			if (LMB.Hit)
			{
				//Hand = rand() % 2;
				LMB.Hit = false;
				switch (hud.slot)
				{
				case Slots::HANDS:
					Dig(terrain, player, hud, sound, 0.0f, false, false);

					x3 = sin(player.yrot * PIover180) * cos(player.xrot * PIover180);
					y3 = -sin(player.xrot * PIover180);
					z3 = cos(player.yrot * PIover180) * cos(player.xrot * PIover180);
					if (LMB.TICK > 16)
					{
						LMB.TICK = 0;
						if (Hit(terrain, player, bots, 2.0f, 16.0f, 32, false, 2.0f, 4.0f, false))
							sound.Foot();
					}
					break;
				case Slots::SPADE:

					if (LMB.TICK > 16)
					{
						LMB.TICK = 0;
						if (Hit(terrain, player, bots, 4.0f, 16.0f, 16, true, 4.0f, 4.0f, false))
							sound.Foot();
					}
					break;
				case Slots::BAZOOKA:
					x3 = sin(player.yrot * PIover180) * cos(player.xrot * PIover180);
					y3 = -sin(player.xrot * PIover180);
					z3 = -cos(player.yrot * PIover180) * cos(player.xrot * PIover180);
					x3h = sin((player.yrot + 90.0f) * PIover180) * cos((player.xrot)*PIover180);
					y3h = -sin((player.xrot)*PIover180);
					z3h = cos((player.yrot + 90.0f) * PIover180) * cos((player.xrot)*PIover180);

					projectile.MakeRocket(0, player.xpos + x3 * 4, player.ypos + y3 * 4, player.zpos + z3 * 4, x3, y3, z3, 16.0f, 8.0f);
					player.GetWeapon(Slots::BAZOOKA).Ammo -= 1;
					break;
				case Slots::BLOCKS:
					Dig(terrain, player, hud, sound, 0.0f, true, false);
					break;
				}
			}
			if (RMB.Hit)
			{
				RMB.Hit = false;
				switch (hud.slot)
				{
				case Slots::HANDS:

					if (LMB.TICK > 16)
					{
						LMB.TICK = 0;
						if (Hit(terrain, player, bots, 2.0f, 16.0f, 32, false, 2.0f, 4.0f, false))
							sound.Foot();

					}
					break;
				case Slots::SPADE:
					if (LMB.TICK > 16)
					{
						if (Hit(terrain, player, bots, 4.0f, 16.0f, 16, true, 4.0f, 4.0f, false))
							sound.Foot();
					}
					break;
				case Slots::BLOCKS:
					break;
				}
			}

			if (keys['G'].Hit && game.Cheat) {
				game.Debug = !game.Debug;
				terrain.BuildScene(false, true);
			}
			bool pressedy = false;
			
			if (keys['F'].press && game.Cheat)
			{
				physup_ = true;
			}
			else physup_ = false;
			if (keys['H'].press)
			{
				int b = 0;
				for (int a = 0; a < 1024 * 1024 * 128; a++)
				{
					b++;
				}
			};
			}
			if (keys['T'].Hit) {
				Chat();
			}
		}
		else if (game.mode == GameModes::menu)
		{
			player.yrot += 0.3f * deltaTick;
		}
		move_.angle = player.yrot * PIover180;
		move_.scalar = 1.0f * deltaTick;



		float forwardx = move_.GetY(),
			forwardy = -player.movespeed,
			forwardz = -move_.GetX(),
			leftx = -move_.Rotate(PI / 2).GetY(),
			leftz = move_.Rotate(PI / 2).GetX();

		float finxmove = 0, finymove = 0, finzmove = 0;

		if (!enteringText) {
			if (keys['W'].press)
			{
				slock_ = false;
				finymove += forwardy;

				if (!wlock_)
				{
					finxmove += forwardx;
					finzmove += forwardz;
				}
			}

			if (keys['S'].press)
			{
				wlock_ = false;
				finymove -= forwardy;

				if (!slock_)
				{
					finxmove -= forwardx;
					finzmove -= forwardz;
				}
			}
			if (keys['A'].press)
			{
				dlock_ = false;
				if (!alock_)
				{
					finxmove += leftx;
					finzmove += leftz;
				}
			}
			if (keys['D'].press)
			{
				alock_ = false;
				if (!dlock_)
				{
					finxmove -= leftx;
					finzmove -= leftz;
				}
			}
		}


		finalmove_.MakeFromXY(finxmove, finzmove);


		finalmove_.Normalize();




		int localx, localy;
		POINT ptCursor;
		GetCursorPos(&ptCursor);

		localx = ptCursor.x;
		localy = ptCursor.y;
		int deltax = (localx - (signed int)(SCREEN_CENTER_X / 2));
		int deltay = (localy - (signed int)(SCREEN_CENTER_Y / 2));

		if (!windowFocused) {
			deltax = 0;
			deltay = 0;
		}

		if (game.mode != GameModes::menu)
		{
			Collision(player,terrain);
			player.yrot += deltax / 32.0;
			player.xrot += deltay / 32.0;
			player.xpos += finalmove_.GetX() * player.movespeed * deltaTick;
			player.zpos += finalmove_.GetY() * player.movespeed * deltaTick;
			if (player.xrot > 90.0f)player.xrot = 90;
			if (player.xrot < -90.0f)player.xrot = -90;
			SmoothCamera(terrain, player, game);
			Gravity(player, terrain);
		}
		else
		{
			float h = terrain.scene[(int)(player.xpos)][(int)(player.zpos)].h + 1.0f;
			if (h - player.ypos > 0.0f) { player.ypos = h; }
		}

		ClearKeys();
	}
}
