#pragma once

class Textures;
class Player;
class Terrain;
class Game;
class Render;
class HUD;
class Sound;
class BotManager;
class Projectile;

enum class State : byte
{
	walking,
	cube,
	inair
};

class Input
{
public:
	bool windowFocused = false;
	void Check(BotManager& bots, Sound& sound, Terrain& terrain, Player& player, HUD& hud, Game& game);
	void ClearKeys();
	bool Hand = 0;
	struct Key
	{
		bool press;
		bool Rel;
		bool Hit;
		coord TICK;
	}keys[256], LMB, RMB, MMB;
	int xdeb, ydeb, zdeb, ydebspec;

private:
	int big_ = 1;
	bool physup_ = false;
	int smgtick_ = 0;
	float realcamera_[3] = { 0.0f, 0.0f,  0.0f };
	float fincamera_[3] = { 0.0f, 0.0f,  0.0f };

	bool deadonce_;
	coord myTick_;


	State state_;


	bool wlock_, alock_, slock_, dlock_;

	Vector2D move_;
	Vector2D finalmove_;

	float accel_;


	void ChatEnter(HUD& hud, Game& game);

	bool Hit(Terrain& terrain, const Player& player, BotManager& bots, float dist, float precision, int damage, bool destroyblocks, float pushforce, float maxdist, bool blood);

	bool Raytrace(Terrain& terrain, const Player& player, int& xout, int& zout);

	void Dig(Terrain& terrain, Player& player, const HUD& hud, Sound& sound, float level, bool addcube, bool debug, bool weak = 1);
	void SMGShoot(Sound& sound, Terrain& terrain, Player& player, BotManager& bots, float damage);
	void Jump(const Player& player);
	void Gravity(Player& player, const Terrain& terrain);
	

	void Collision(Player& player, const Terrain& terrain);


	void SmoothCamera(Terrain& terrain, Player& player, Game& game);

	void ProcChr(HUD& hud, Game& game);


};

