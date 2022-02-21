#include "main.h"
#include "bots.h"
#include "game.h"
#include "player.h"
#include "terrain.h"
#include "input.h"

bool Terrain::AddCube(coord x, coord y, coord z, Objects type, bool using_adapted_coords)
{
	int cnum = 0;

	if (!using_adapted_coords)
	{
		x /= 2; y /= 2; z /= 2;
	}

	if (y > 0 && y < 512 && x>0 && x < terxh && z>0 && z < terzh && cubescene[x][y][z].type == Objects::NONE)
	{
		int x2 = (int)(x / chunksize);
		int z2 = (int)(x / chunksize);

		if (cubenum[x2][z2] > testscale || cubescene[x][y][z].type == type)return false;

		cubescene[x][y][z].type = type;

		cnum = cubenum[x2][z2];
		cubes[cnum][x2][z2].x = x;
		cubes[cnum][x2][z2].y = y;
		cubes[cnum][x2][z2].z = z;
		cubenum[x2][z2]++;
		return true;
	}
	else
	{
		return false;
	}
}

bool Terrain::IsGrass(worldgrid a) const
{
	if (a.type >= Objects::GRASS1 && a.type <= Objects::GRASS3)
		return true;
	return false;
}

void Terrain::JustMakeItBigger()
{
	coord xsize_ = chunkx * chunksize;
	coord zsize_ = chunkz * chunksize;
	for (int x = 0; x < xsize_; x++) {
		for (int z = 0; z < zsize_; z++)
		{
			scene[x][z].h += 2;
		}
	}
}
void Terrain::JustMakeItSmaller()
{

	coord xsize_ = chunkx * chunksize;
	coord zsize_ = chunkz * chunksize;
	for (int x = 0; x < xsize_; x++) {
		for (int z = 0; z < zsize_; z++)
		{
			scene[x][z].h -= 2;
		}
	}
}
void Terrain::ClearAll(BotManager& bots)
{
	for (int z = 0; z < EngineLevelSizeZ; z++)
	{
		for (int x = 0; x < EngineLevelSizeX; x++)
		{
			buffer_[x][z] = 0;
			scene[x][z].h = 0;
			scene[x][z].type = Objects::NONE;
			clr0rgb[x][z].r = 0;
			clr0rgb[x][z].g = 0;
			clr0rgb[x][z].b = 0;
		}
	}
	for (int x = 0; x < chunkx; x++)for (int z = 0; z < chunkz; z++)cubenum[x][z] = 0;

	memset(cubescene, 0, terxh * 128 * terzh * sizeof(cubegrid));

	for (int a = 0; a < botsnum; a++)
		bots.bot[a].Kill();
}

Terrain::worldgrid Terrain::SafeScene(int xpos, int zpos)
{
	if (xpos > 0 && zpos > 0 && xpos < terx && zpos < terz)
		return scene[xpos][zpos];
	else
		return scene[0][0];
}

void Terrain::SmoothBuffer(coord iteration)
{
	coord a, x2, z2, b;
	for (int a = 0; a < iteration; a++)
	{
		for (int x = 0; x < terx; x++)
		{
			for (int z = 0; z < terz; z++)
			{
				buffer_[x][z] = (buffer_[x][z - 1] + buffer_[x][z + 1] + buffer_[x][z]) / 3.0f;
			}
		}
		for (int z = 0; z < terz; z++)
		{
			for (int x = 0; x < terx; x++)
			{
				buffer_[x][z] = (buffer_[x - 1][z] + buffer_[x + 1][z] + buffer_[x][z]) / 3.0f;
			}
		}
	}
}
void Terrain::ApplyBufferToScene(coord times)
{
	coord x, z;
	if (times == 1)
		for (int x = 0; x < terx; x++)
		{
			for (int z = 0; z < terz; z++)
			{
				scene[x][z].h += buffer_[x][z];
			}
		}
	else
		for (x = 0; x < terx; x++)
		{
			for (z = 0; z < terz; z++)
			{
				scene[x][z].h += times * buffer_[x][z];
			}
		}
}

void Terrain::Interpolate(int i)
{

	int size = landsize / i;

	for (int pointx = 0; pointx < i; pointx++)
		for (int pointz = 0; pointz < i; pointz++)
		{
			float ya = points_[pointx][pointz];
			float yb = points_[pointx + 1][pointz];
			int xa = 0;
			int xb = size;

			float k = (ya - yb) / (xa - xb);

			for (int n = 0; n < size; n++)
			{
				float y = k * n + ya;
				buffer_[pointx * size + n][pointz * size] = y;
			}
		}


	for (int x = 0; x < landsize; x++)
		for (int pointz = 0; pointz < i; pointz++)
		{
			float ya = buffer_[x][pointz * size];
			float yb = buffer_[x][(pointz + 1) * size];
			int xa = 0;
			int xb = size;

			float k = (ya - yb) / (xa - xb);

			for (int n = 0; n < size; n++)
			{
				float z = k * n + ya;
				buffer_[x][pointz * size + n] = z;
			}
		}
}

void Terrain::Points(int i, float height)
{
	for (int x = 0; x < i; x++)
		for (int z = 0; z < i; z++)
			points_[x][z] = height * (rand() % 256) / 256.0f;
}

void Terrain::Add()
{
	for (int x = 0; x < landsize; x++)
		for (int z = 0; z < landsize; z++)
			scene[x][z].h += buffer_[x][z];
}

void Terrain::Rise(float a)
{
	for (int x = 0; x < landsize; x++)
		for (int z = 0; z < landsize; z++)
			scene[x][z].h += a;
}

void Terrain::Generate()
{

	int a = landsize / 128.0f / 2.0f / 2.0f;

	Points(a, 384);

	a *= 2;
	Points(a, 128);

	Interpolate(a);
	Add();

	a *= 2;
	Points(a, 64);


	for (int m = 0; m < 4; m++)
	{

		int x1 = rnd(0, a);
		int z1 = rnd(0, a);
		for (int n = 0; n < 64; n++)
		{
			x1 += rnd(-1, 1);
			z1 += rnd(-1, 1);
			points_[x1][z1] += 64.0f;
		}
	}


	for (int m = 0; m < 4; m++)
	{

		int x1 = rnd(0, a);
		int z1 = rnd(0, a);
		for (int n = 0; n < 64; n++)
		{
			x1 += rnd(-1, 1);
			z1 += rnd(-1, 1);
			points_[x1][z1] -= 64.0f;
		}
	}

	Interpolate(a);
	Add();


	a *= 2;
	Points(a, 32);
	Interpolate(a);
	Add();

	a *= 2;
	Points(a, 16);
	Interpolate(a);
	Add();

	a *= 2;
	Points(a, 8);
	Interpolate(a);
	Add();

	a *= 2;
	Points(a, 4);
	Interpolate(a);
	Add();
	/*
	a*=2;
	Points(a,2);
	Interpolate(a);
	Add(-1,1);

	a*=2;
	Points(a,0.5);
	Interpolate(a);
	Add(1,-1);
	*/
	Rise(rnd(-96, -64));


}
void Terrain::PlaceCrates()
{
	for (coord a3 = 0; a3 < cratenum; a3++)
	{
		coord ranx = rand() % terx, ranz = rand() % terz;
		coord h = scene[ranx][ranz].h;
		if (h > WATER_HEIGHT)
			AddCube(ranx, h + 2, ranz, Objects::CRATE, false);
	}
}

void Terrain::BuildCastles()
{
	for (coord a = 0; a < buildnum; a++) {

		coord size = (int)((16 + rand() % 4 * 4) / 2) * 2;

		coord x1 = (int)(
			(rand() % (terx - 2 * size) + size) / 2) * 2;
		coord z1 = (int)(
			(rand() % (terz - 2 * size) + size) / 2) * 2;

		float y1 = abs(scene[x1 + size / 2][z1 + size / 2].h);

		if (y1 < 10.0f) { Castles[a].Enabled = false; }
		else {
			Castles[a].Enabled = true;

			coord floors = (rnd(1, 5));

			coord texture = rand() % 32;
			int clrg = rand() % 4 * 16 - 32, clrb = rand() % 4 * 16 - 32;
			coord clradd = 32 + rand() % 4 * 32;


			coord floorheight = (5);//rand()%6+8;

			coord flh = floorheight * 2;

			Castles[a].x = x1 + size / 2;
			Castles[a].z = z1 + size / 2;

			Castles[a].h = floors * flh;
			Castles[a].y = y1 + Castles[a].h / 2;

			Castles[a].w = size;
			Castles[a].l = size;


			for (int x = x1; x < x1 + size; x++)
			{
				for (int z = z1; z < z1 + size; z++)
				{
					coord y2 = (int)(y1 / 2.0f);
					scene[x][z].h = y2 * 2 + 1.0f;

				}
			}

			for (int x = x1; x < x1 + size; x++)
			{
				coord xl = x - x1;
				for (int z = z1; z < z1 + size; z++)
				{
					coord yl = z - z1;
					scene[x][z].type = Objects::CONCRETE;
					if (xl != 0 && yl != 0 && xl != size && yl != size)
					{
						clr0rgb[x][z].r = ((xl * xl + yl * yl) * texture) % 256 / 4 + clradd;
						clr0rgb[x][z].g = clr0rgb[x][z].r + clrg;
						clr0rgb[x][z].b = clr0rgb[x][z].r + clrb;
					}
					else
					{
						clr0rgb[x][z].r = clradd;
						clr0rgb[x][z].g = clr0rgb[x][z].r;
						clr0rgb[x][z].b = clr0rgb[x][z].r;
					}
				}
			}

			coord xd = 0, zd = 0, xd2 = 0, zd2 = 0;

			xd = x1 + rand() % (size - 6) - 3;

			zd = z1 + rand() % (size - 6) - 3;


			for (coord floor = 0; floor < floors; floor++)
			{

				for (coord h2 = 0; h2 < flh; h2 += 2)
				{

					for (int x = x1; x < x1 + size; x += 2)
					{
						if (floor == 0 && ((x<xd - 2 || x>xd + 2)) || floor > 0)
						{
							AddCube(x, y1 + h2 + floor * flh, z1, Objects::CONCRETE, false);
							AddCube(x, y1 + h2 + floor * flh, z1 + size, Objects::CONCRETE, false);
						}

					}
					for (coord z = z1; z < z1 + size; z += 2)
					{
						if (floor == 0 && ((z<zd - 2 || z>zd + 2)) || floor > 0)
						{
							AddCube(x1, y1 + h2 + floor * flh, z, Objects::CONCRETE, false);
							AddCube(x1 + size, y1 + h2 + floor * flh, z, Objects::CONCRETE, false);
						}
					}
				}

				//ÏÎË ÍÀ ÄÐÓÃÈÕ ÝÒÀÆÀÕ

				if (floor > 0)
					for (int x = x1; x < x1 + size; x += 2)
					{
						for (int z = z1; z < z1 + size; z += 2)
						{
							AddCube(x, y1 + floor * flh, z, Objects::CONCRETE, false);
						}
					}

			}

			//ÑÓÍÄÓÊÈ

			coord crts = rnd(1, 4);
			for (coord r = 0; r < crts; r++)
				AddCube(x1 + rand() % size, y1 + (rand() % floors) * flh + 2, z1 + rand() % size, Objects::CRATE, false);
		}
	}
}
bool Terrain::OnLevel(float x, float y, float z)
{
	if (x > 0 && x < terx)
		if (y > -16 && y < maxy)
			if (z > 0 && z < terz)
				return true;
	return false;
}
void Terrain::Fractal()
{
	float GlobalAdd;
	seedrand_tick();

	Generate();

	float k = EdgeDampingK;

	int dist;
	coord max = terxh - EdgeDampingMaxDist;
	float finaddx = 0.0f, finaddy = 0.0f;
	for (int x = 0; x < terx; x++)
	{
		dist = abs((int)x - (int)terxh);
		if (dist > max)
		{
			finaddx = ((float)dist - (float)max) / k;
		}
		else
		{
			finaddx = 0;
		}
		for (int z = 0; z < terz; z++)
		{

			dist = abs((int)z - (int)terzh);
			if (dist > max)
			{
				finaddy = ((float)dist - (float)max) / k;
			}
			else
			{
				finaddy = 0;
			}

			scene[x][z].h -= 17.0f;//13.0f;
			if (scene[x][z].h < 6.0f) { scene[x][z].h = scene[x][z].h / 1.5f - 2.0f + rndf(); }

			scene[x][z].h += finaddx + finaddy;
			if (!rnd(0, 7) && objectsnum_ < MAX_Objs && scene[x][z].h>36.0f && scene[x][z].h < 80.0f)// && scene[x][z]<60.0f && scene[x][z]>30.0f)
			{
				scene[x][z].type = static_cast<Objects>(static_cast<int>(Objects::GRASS1) + rnd(0, 2));
			}

			if (scene[x][z].h < 0.0f) { scene[x][z].h = 0 + rndf(); }

		}
	}

	BuildCastles();
	PlaceCrates();
}


void Terrain::BuildScene(BotManager& bots, const Game& game, Player& player, bool rebuildgeo, bool rebuildclr) {

	objectsnum_ = 0;
	if (rebuildgeo)
	{
		ClearAll(bots); Fractal();
	}

	for (coord n = 0; n < botsnum; n++)
		bots.bot[n].Spawn(*this);

	if (game.Cheat)
	{
		player.GetWeapon(Slots::SPADE).Own = true;
		player.GetWeapon(Slots::BAZOOKA).Own = true;
		player.GetWeapon(Slots::MACHINEGUN).Own = true;
		player.GetWeapon(Slots::SPADE).Ammo = 1;
		player.GetWeapon(Slots::BAZOOKA).Ammo = 8;
		player.GetWeapon(Slots::MACHINEGUN).Ammo = 64;
	}
	player.GetWeapon(Slots::HANDS).Own = true;

	player.GetWeapon(Slots::HANDS).Ammo = 0;

	player.GetWeapon(Slots::BLOCKS).Own = true;

	player.GetWeapon(Slots::BLOCKS).Ammo = 0;

	const int LevelSand = 32;
	const int LevelGrass = 114;
	const int LevelRock = 196;

	if (rebuildclr)
		for (int x = 0; x < EngineLevelSizeX; x++) {
			for (int z = 0; z < EngineLevelSizeZ; z++) {
				float h = scene[x][z].h;
				coord num = x * z + z;
				if (scene[x][z].type != Objects::CONCRETE)
				{
					if (!game.Debug)
					{
						coord rg = rnd(0, 15);
						if (x == 0 || z == 0 || x > EngineLevelSizeX || z > EngineLevelSizeZ)
						{
							clr0rgb[x][z].r = 0;
							clr0rgb[x][z].g = clr0rgb[x][z].r / 1.8;
							clr0rgb[x][z].b = clr0rgb[x][z].r / 3.2;
						}
						else
						{
							
							/*SHORE*/				if (h < LevelSand)
							{
								clr0rgb[x][z].r = rnd(175, 180) + h;///2.0f;
								clr0rgb[x][z].g = rnd(145, 155) + h;///2.0f;
								clr0rgb[x][z].b = rnd(105, 110) + h;///2.0f;

								if (!IsGrass(scene[x][z]))scene[x][z].type = Objects::SAND;
							}
							/*GRASS*/				if (h >= LevelSand && h < LevelRock)
							{
								if (scene[x + 1][z].h < LevelGrass && scene[x - 1][z].h < LevelGrass && scene[x][z + 1].h < LevelGrass && scene[x][z - 1].h < LevelGrass && scene[x - 2][z].h < LevelGrass && scene[x][z - 2].h < LevelGrass)
								{
									clr0rgb[x][z].r = rand() % 8 + 80 + h - 12.0f + 0.5 * (h - 12.0f);
									clr0rgb[x][z].g = rand() % 8 + 114 + h - 12.0f;
									clr0rgb[x][z].b = rand() % 4 + 50 + h - 12.0f;

									if (!IsGrass(scene[x][z]))scene[x][z].type = Objects::GRASS;
								}
								/*ROCK1*/					else
								{
									clr0rgb[x][z].r = rand() % 16 + 100;
									clr0rgb[x][z].g = rand() % 16 + 104;
									clr0rgb[x][z].b = rand() % 8 + 110;
								}
							}
							/*SNOW*/				if (h >= LevelRock && h < 256.0f)
							{
								clr0rgb[x][z].r = rand() % 6 + 240;
								clr0rgb[x][z].g = rand() % 6 + 240;
								clr0rgb[x][z].b = rand() % 8 + 245;
							}
							/*ALSO SNOW*/				if (h >= 256.0f)
							{
								clr0rgb[x][z].r = rand() % 6 + 240;
								clr0rgb[x][z].g = rand() % 6 + 240;
								clr0rgb[x][z].b = rand() % 8 + 245;
							}
							coord a;
						}
					}
					else
					{
						clr0rgb[x][z].r = h * 16.0;
						clr0rgb[x][z].g = h * 16.0;
						clr0rgb[x][z].b = h * 16.0;
					}
				}
			}
		}
}

void Terrain::Smooth() {
	coord a, x, y, x2, z2;
	coord b = 4 * chunkx * chunkz * chunksize * chunksize;
	for (x = 0; x < terx / 2; x++)
	{
		for (y = 0; y < terz / 2; y++)
		{
			x2 = x * 2, z2 = y * 2;
			scene[x2][z2].h = (scene[x2 + 1][z2 + 1].h + scene[x2 + 1][z2].h + scene[x2][z2 + 1].h + scene[x2][z2].h + scene[x2 - 1][z2 - 1].h + scene[x2 - 1][z2].h + scene[x2][z2 - 1].h + scene[x2 + 1][z2 - 1].h + scene[x2 - 1][z2 + 1].h) / 9.0f;
		}
	}
	for (x = 0; x < terx / 2; x++)
	{
		for (y = 0; y < terz / 2; y++)
		{
			x2 = x * 2 + 1, z2 = y * 2 + 1;
			scene[x2][z2].h = (scene[x2 + 1][z2 + 1].h + scene[x2 + 1][z2].h + scene[x2][z2 + 1].h + scene[x2][z2].h + scene[x2 - 1][z2 - 1].h + scene[x2 - 1][z2].h + scene[x2][z2 - 1].h + scene[x2 + 1][z2 - 1].h + scene[x2 - 1][z2 + 1].h) / 9.0f;
		}
	}
}
void Terrain::MakeItBig()
{
	coord a, x, z;
	coord xsize_ = chunkx * chunksize;
	coord ysize = chunkz * chunksize;
	for (x = 0; x < xsize_; x++) {
		for (z = 0; z < ysize; z++)
		{
			scene[x][z].h = pow(scene[x][z].h, 2.0f) / 16.0f;
		}
	}
}
