#pragma once

struct Terrain;
struct Player;
struct Game;
struct Sound;

const int botsnum = 16;

class Bot {
	friend class BotManager;
public:
	void Process(Terrain& terrain, const Game& game, Player& player, Sound& sound);
	float x, y, z;
	int HPb;
	float randyr;
	void Kill();
	bool Spawn(const Terrain& terrain);
	float yr;

private:
	bool found_;
	int freezetime_;
	bool exists_;

	bool NewPosition(const Terrain& terrain);
	void BreakWalls(Terrain& terrain);
};

class BotManager
{
	public:

		Bot bot[256];
		void Process(Terrain& terrain, const Game& game, Player& player, Sound& sound);

};
