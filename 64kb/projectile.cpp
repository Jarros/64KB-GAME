#include "Projectile.h"
#include "main.h"
#include "bots.h"
#include "terrain.h"
#include "sound.h"

void Projectile::MakeProj(float x, float y, float z, float vecx, float vecy, float vecz, float dmg, float dmgrad2)
{
	ystart = y;
	//if(!IsExists)
	//{
	mx = vecx; my = vecy; mz = vecz;
	px = x; py = y; pz = z;
	IsExists = true;
	dmgrad = dmgrad2;
	accel = 0.0f;
	TICK = 0;

}
void Projectile::Explode(BotManager &bots, Terrain& terrain, Sound& sound)
{
	int x2 = (int)px, y2 = (int)py, z2 = (int)pz;
	int x4, y4, z4;

	for (coord a = 0; a < 16; a++)
	{
		float dist = Dist3D(bots.bot[a].x, bots.bot[a].y, bots.bot[a].z, px, py, pz);
		if (dist < 32.0f)
		{
			bots.bot[a].HPb -= 256;
			for (int ran = -4; ran < 4; ran++)
			{
				int xra = +rnd(-2, 2), zra = +rnd(-2, 2);
				terrain.clr0rgb[(int)bots.bot[a].x + xra][(int)bots.bot[a].z + zra].r = 140 + rnd(0, 15);
				terrain.clr0rgb[(int)bots.bot[a].x + xra][(int)bots.bot[a].z + zra].g = 80;
				terrain.clr0rgb[(int)bots.bot[a].x + xra][(int)bots.bot[a].z + zra].b = 30;
			}
		}
	}

	if (terrain.OnLevel(x2, y2, z2))
	{
		sound.Foot();
		for (int x = -dmgrad; x < dmgrad; x++)
		{
			for (int z = -dmgrad; z < dmgrad; z++)
			{
				coord x3 = x2 + x, y3 = y2 + y, z3 = z2 + z;
				float damage2 = dmgrad / (abs(Dist3D(0, y2, 0, x, terrain.scene[x3][z3].h, z)) + 0.5f);
				terrain.scene[x3][z3].h -= damage2;
				if (terrain.clr0rgb[x3][z3].r > 48)
					terrain.clr0rgb[x3][z3].r -= 4 * damage2;
				if (terrain.clr0rgb[x3][z3].g > 48)
					terrain.clr0rgb[x3][z3].g -= 4 * damage2;
				if (damage2 > 2.0 && terrain.IsGrass(terrain.scene[x3][z3]))
					terrain.scene[x3][z3].type = Objects::GRASS;
			}
		}
		dmgrad /= 2;
		for (int x = -dmgrad; x < dmgrad; x++)
		{
			for (int y = -dmgrad; y < dmgrad; y++)
			{
				for (int z = -dmgrad; z < dmgrad; z++)
				{
					coord x3 = (px + x) / 2, y3 = (py + y) / 2, z3 = (pz + z) / 2;
					terrain.cubescene[x3][y3][z3].type = Objects::NONE;
				}
			}
		}
	}
	IsExists = false;
}
void Projectile::Move(BotManager& bots, Terrain& terrain, Sound& sound)
{
	TICK += T;

	if (TICK > 200)
		Explode(bots, terrain, sound);
	accel += 0.01f;
	px += mx * T;
	py = -GRAVITY * TICK * TICK / 2 + my * TICK + ystart;    //(my-accel)*T;
	pz += mz * T;
	int x2 = (int)px, y2 = (int)py, z2 = (int)pz;

	float rounding = 2.0f;

	for (int x3 = -1; x3 < 1; x3++)
		for (int z3 = -1; z3 < 1; z3++)
		{
			if ((int)(terrain.scene[x2 + x3][z2 + z3].h / rounding) == (int)(y2 / rounding))
			{
				Explode(bots, terrain, sound); return;
			}
		}
	if (terrain.cubescene[x2 / 2][y2 / 2][z2 / 2].type != Objects::NONE)
	{
		Explode(bots, terrain, sound); return;
	}
}

bool Projectile::MakeRocket(int a, float x, float y, float z, float vecx, float vecy, float vecz, float dmg, float dmgrad)
{
	for (coord b = 0; b < 10; b++)
	{
		if (!/*proj[b].*/IsExists)
		{
			/*proj[b].*/MakeProj(x, y, z, vecx, vecy, vecz, dmg, dmgrad);
			return true;
		}
	}
	return false;
}