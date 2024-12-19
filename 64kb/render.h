#pragma once

#include "main.h"

class Textures;
class Player;
class Terrain;
class Game;
class Input;
class HUD;
class Sound;
class BotManager;
enum class Objects : byte;

class Render
{
public:

	bool CubeIsInit;
	bool DrawCube;
	Objects drawtype;

	void PrintLine(GLfloat x, GLfloat y, GLfloat z, GLfloat x2, GLfloat y2, GLfloat z2, ubyte r, ubyte g, ubyte b, ubyte a, GLfloat width, bool machinegun);

	void CubeFaces(GLfloat v0a[], GLfloat v1a[], GLfloat v2a[], GLfloat v3a[], GLfloat v0b[], GLfloat v1b[], GLfloat v2b[], GLfloat v3b[]);


	void ShortCubeFaces(GLfloat W, GLfloat H, GLfloat L);

	void Projectile(GLfloat x, GLfloat y, GLfloat z, GLuint TEXTURE);

	void Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat pitch, GLfloat yaw, GLfloat roll, GLfloat h, Objects type, const Textures& textures);

	void Box(GLfloat x, GLfloat y, GLfloat z, GLfloat W, GLfloat H, GLfloat L, GLfloat pitch, GLfloat yaw, GLfloat roll, GLuint TEXTURE);

	void Hands(const Game& game, const Textures& textures, const Input& input, const Player& player, const ::HUD& hud);

	void Clouds(const Textures& textures);

	void Water(bool GroundBase);

	void SeaFilter();

	void BloodFilter();

	void Cycle(::Terrain& terrain, Player& player, Textures& textures, Game& game, ::HUD& hud, Sound& sound, ::BotManager& bots, Input& input);

	void BotManager(::BotManager& bots, const Textures& textures);

private:

	int LOD_ = 1;
	ubyte bLOD_ = 1;

	void Rotate(float angle_pitch, float angle_yaw, float angle_roll);

	void Clear();

	void Sky(const Game& game, const Player& player);

	void TransformScene(const Player& player);

	void Fog(const Player& player);

	void Terrain(const ::Terrain& terrain, const Player& player);

	void Grass(const ::Terrain& terrain, const Player& player);

	void SpecialObjects(const Textures& textures);

	void Cubes(const ::Terrain& terrain, const Player& player, const Textures& textures);

	void BuildingLODS(const ::Terrain& terrain, const Player& player, const Textures& textures);

	void GameOver(const Game& game);

	void DebugCube(const Input& input, const Textures& textures);

	void Water(const Player& player);

	void Menu(::BotManager& bots, Game& game, Player& player, ::HUD& hud, ::Terrain& terrain, const Input& input, Sound& sound);

	void HUD(Player& player, const Game& game, ::HUD& hud, const Input& input);

	void Bot(::BotManager& bot);

};
