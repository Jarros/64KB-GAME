#include "bots.h"
#include "main.h"
#include "terrain.h"
#include "player.h"
#include "game.h"
#include "sound.h"

void Bot::Kill()
{
	HPb = 0;
	exists_ = false;
}

bool Bot::NewPosition(const Terrain& terrain)
{
	randyr = rand() % 360;
	for (int tries = 0; tries < 16; tries++)
	{
		x = rand() % terx;
		z = rand() % terz;
		if (terrain.scene[(int)x][(int)z].h > WATER_HEIGHT)
			return true;
	}
	return false;
}

bool Bot::Spawn(const Terrain& terrain)
{
	if (NewPosition(terrain))
	{
		exists_ = true;
		HPb = 100 + rnd(0, 155);
		return true;
	}
	return false;
}

void Bot::BreakWalls(Terrain& terrain)
{
	coord xc = (int)(x / 2) + rnd(-1, 1), yc = (int)(y / 2) + rnd(-1, 1), zc = (int)(z / 2) + rnd(-1, 1);
	if (terrain.cubescene[xc][yc][zc].type != Objects::NONE)
	{
		terrain.cubescene[xc][yc][zc].type = Objects::NONE;
		freezetime_ = 16;
	}
}

void Bot::Process(Terrain& terrain, const Game &game, Player &player, Sound &sound)
{
	bool Attack = false;
	if (exists_)
	{

		if (game.Stop)return;

		if (freezetime_ > 0)
		{
			freezetime_--;
			return;
		}
		if (x < 1)x = 2;
		if (z < 1)z = 2;
		if (x > terx)x = terx - 2;
		if (z > terz)z = terz - 2;


		y = terrain.scene[(int)x][(int)z].h + 3.0f;

		if (HPb < 0)
		{
			exists_ = false;
			Spawn(terrain);
		}

		float sped;

		float x3 = sin(yr);
		float z3 = cos(yr);

		if (Dist3D(x, y, z, player.xpos, player.ypos, player.zpos) < 64.0f && game.mode != GameModes::menu)
		{
			randyr += (rnd(-3, 3)) / 256.0f;
			if (!found_)
			{
				randyr = 0;
				found_ = true;
			}
			yr = atan((x - player.xpos) / (z - player.zpos)) + randyr;

			if (z > player.zpos) {
				yr += PI;
			}

			sped = 3.0f;
			sped += abs(terrain.scene[(int)x][(int)z].h - terrain.scene[(int)(x + x3)][(int)(z + z3)].h) * 8;

			if (Dist3D(player.xpos, player.ypos, player.zpos, x, y, z) < 4.0f)
				Attack = true;
			if (Attack)
			{
				sped += 128.0f;
				if (!game.Cheat && player.HP > 0)
				{
					player.HP -= 1;
					sound.Pickup();
				}
			}
		}
		else
		{
			randyr += rnd(-3, 3) / 64.0f;
			yr = randyr;

			if (y < WATER_HEIGHT)
			{
				x3 *= -4;
				z3 *= -4;
				randyr += PI;
			}
			else
			{
				x3 = sin(yr);
				z3 = cos(yr);
			}

			sped = 8.0f;
			sped += abs(terrain.scene[(int)x][(int)z].h - terrain.scene[(int)(x + x3)][(int)(z + z3)].h) * 8;

		}
		BreakWalls(terrain);
		x += x3 * T / sped;
		z += z3 * T / sped;
	}
}

void BotManager::Process(Terrain& terrain, const Game& game, Player& player, Sound& sound) {
	for (coord n = 0; n < botsnum; n++)
	{
		bot[n].Process(terrain, game, player, sound);
	}
}