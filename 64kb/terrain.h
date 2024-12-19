#pragma once

#include "main.h"

struct BotManager;
struct Game;
struct Player;

const coord buildnum = (int)(ters * 0.00004) + 1;
const coord cratenum = (int)(ters * 0.0001) + 1;

enum class Objects : byte
{
	NONE, GROUND, SAND, GRASS, GRASS1, GRASS2, GRASS3, ROCK, CONCRETE, CRATE, DEBUG, CRATEDEBUG, RAW_NOISE
};

class Terrain
{
	private:
		BotManager* bots_; 
		Game* game_; 
		Player* player_;
public:
	void bindReferences(BotManager* bots, Game* game, Player* player);
	bool AddCube(coord x, coord y, coord z, Objects type, bool using_adapted_coords);
	//void BuildScene(BotManager& bots, const Game& game, Player& player, bool rebuildgeo, bool rebuildclr);
	unsigned long seed_ = 0;

	struct Castle
	{
		bool Enabled;
		coord l, w, h;
		coord x, y, z;
	}Castles[buildnum];

	struct RGB
	{
		unsigned char r, g, b;
	}
	clr0rgb[EngineLevelSizeX][EngineLevelSizeZ];
	struct cubegrid
	{
		Objects type;
	}cubescene[terxh][128][terzh];
	struct cube
	{
		coord x, y, z;
	}cubes[testscale + 1][chunkx][chunkz];//CUBES_MAX];

	struct worldgrid
	{
		float h;
		Objects type;
	}scene[EngineLevelSizeX][EngineLevelSizeZ];
	
	int cubenum[chunkx][chunkz];
	bool IsGrass(worldgrid a) const;
	void BuildScene(bool rebuildgeo, bool rebuildclr, long long int seed = 0);
	void Smooth();
	worldgrid SafeScene(int xpos, int zpos);
	bool OnLevel(float x, float y, float z);

private:
	coord a_;
	float buffer_[chunkx * chunksize][chunkz * chunksize];
	//std::vector<float> buffer_[;
	//coord x,y;



	struct Objs
	{
		coord x, z;
		ubyte type;
		float param;
	}objects1_[MAX_Objs];
	coord objectsnum_ = 0;



	void JustMakeItBigger();
	void JustMakeItSmaller();
	void ClearAll(BotManager& bots);

	void SmoothBuffer(coord iteration);
	void ApplyBufferToScene(coord times);

	float points_[256][256];
	float points8_[8][8];

	void Interpolate(int i);

	void Points(int i, float height);

	void Add();// int xoffset = 0, int yoffset = 0);

	void Rise(float a);

	void Generate();
	void PlaceCrates();

	void BuildCastles();

	void Fractal(long long int seed);



	void MakeItBig();

};
