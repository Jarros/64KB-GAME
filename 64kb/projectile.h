#pragma once
#include "main.h"

struct BotManager;
struct Terrain;
struct Sound;

class Projectile
{
public:
	bool IsExists;
	float px, py, pz;
	float mx, my, mz;
	float p, y, r;

	float ystart;

	float dmgrad;
	float accel;
	coord TICK;
	void MakeProj(float x, float y, float z, float vecx, float vecy, float vecz, float dmg, float dmgrad2);
	void Explode(BotManager& bots, Terrain& terrain, Sound& sound);
	void Move(BotManager& bots, Terrain& terrain, Sound& sound);
	bool MakeRocket(int a, float x, float y, float z, float vecx, float vecy, float vecz, float dmg, float dmgrad);
};