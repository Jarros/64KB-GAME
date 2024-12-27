#pragma once

class Terrain;
class Player;
class Game;
class Sound;

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
	float x_i, y_i, z_i, yr_i;

private:
	bool found_;
	int freezetime_;
	bool exists_;

	bool NewPosition(const Terrain& terrain);
	void BreakWalls(Terrain& terrain);
};

class otherPlayer : public Bot {
public:
	bool online = false;
	void Process();
};

class BotManager
{
	public:
		Bot bot[256];
		otherPlayer otherPlayers[16];
		void Process(Terrain& terrain, const Game& game, Player& player, Sound& sound);

};
