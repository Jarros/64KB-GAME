#pragma once
#include "main.h"
//#include "main.h"

enum class Objects : byte;
enum class Slots : byte;

class Textures {

public:

	struct WeapTextures {
		GLuint A, B, C;
	}Weapon[10];

	const WeapTextures& GetWeaponTexture(Slots slot) const;
	WeapTextures& GetWeaponTextureWriteable(Slots slot);

	GLuint Cube[16];
	GLuint Zombie[6];
	GLuint Clouds[4];

	void BuildTexture(GLuint& tex, coord side, bool smooth, Objects type, coord r_param1 = 0, coord g_param1 = 0, coord b_param1 = 0, coord r_param2 = 1, coord g_param2 = 1, coord b_param2 = 1);


	void Build();
};
