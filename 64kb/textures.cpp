#include "Textures.h"
#include "Input.h"
#include "Terrain.h"
#include "Player.h"

const Textures::WeapTextures& Textures::GetWeaponTexture(Slots slot) const
{
	return Weapon[static_cast<int>(slot)];
}
Textures::WeapTextures& Textures::GetWeaponTextureWriteable(Slots slot)
{
	return Weapon[static_cast<int>(slot)];
}

void Textures::BuildTexture(GLuint& tex, coord side, bool smooth, Objects type, coord r_param1, coord g_param1, coord b_param1, coord r_param2, coord g_param2, coord b_param2) {
	glEnable(GL_TEXTURE_2D);
	coord sideh = side / 2;
	ubyte surface[256 * 256 * 3];
	glGenTextures(1, &tex);

#define pixel (side*(y)+x)*3
#define border (x==0 || x==side-1 || y==0 || y==side-1)

	switch (type)
	{

	case Objects::RAW_NOISE:
		for (int x = 0; x < side; x++) {
			for (int y = 0; y < side; y++) {
				int b = pixel;
				surface[b] = r_param1 + rnd(0, r_param2);
				surface[b + 1] = g_param1 + rnd(0, g_param2);
				surface[b + 2] = b_param1 + rnd(0, b_param2);
			}
		}
		break;

	case Objects::CONCRETE:
		
		for (int x = 0; x < side; x++) {
			for (int y = 0; y < side; y++) {
				int b = pixel;//(side*(y)+x)*3;
				surface[b] = 100 + rnd(0, 15);
				surface[b + 1] = 100 + rnd(0, 15);
				surface[b + 2] = 100 + rnd(0, 15);
				if (border)
				{
					surface[b] += 16; surface[b + 1] += 16; surface[b + 2] += 16;
				}
			}
		}

		break;

	case Objects::CRATE:
		
		for (int x = 0; x < side; x++) {
			for (int y = 0; y < side; y++) {
				int b = pixel;
				surface[b] = x ^ y;
				surface[b + 1] = surface[b] + 80;
				surface[b + 2] = surface[b];
				surface[b + 2] += 32;
				surface[b] += 96;
				if (border)
				{
					surface[b] -= 16; surface[b + 1] -= 16; surface[b + 2] -= 16;
				}
			}
		}
		
		break;

	case Objects::DEBUG:
		
		for (int x = 0; x < side; x++) {
			for (int y = 0; y < side; y++) {
				int b = pixel;//(side*(y)+x)*3;
				surface[b] = 128;
				surface[b + 1] = 128;
				surface[b + 2] = 128;
				if (border)//(x==0 || x==side-1 || y==0 || y==side-1)
				{
					surface[b] = 255; surface[b + 1] = 255; surface[b + 2] = 255;
				}
			}
		}

		break;


	case Objects::CRATEDEBUG:
		
		for (int x = 0; x < side; x++) {
			for (int y = 0; y < side; y++) {
				int b = pixel;//(side*(y)+x)*3;

				surface[b] = 32;

				surface[b + 1] = 64;

				surface[b + 2] = 96;
				if (border)//(x==0 || x==side-1 || y==0 || y==side-1)
				{
					surface[b + 2] = 128; surface[b + 1] = 255; surface[b] = 128;
				}

			}
		}
		break;

	}

#undef border
#undef pixel

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, side, side, 0, GL_RGB, GL_UNSIGNED_BYTE, surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (!smooth)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glDisable(GL_TEXTURE_2D);
}


void Textures::Build() {

	BuildTexture(GetWeaponTextureWriteable(Slots::HANDS).A, 64, false, Objects::RAW_NOISE, 189, 158, 157, 16, 16, 16);
	BuildTexture(GetWeaponTextureWriteable(Slots::SPADE).A, 64, false, Objects::RAW_NOISE, 80, 60, 45, 16, 16, 16);
	BuildTexture(GetWeaponTextureWriteable(Slots::BAZOOKA).A, 4, false, Objects::RAW_NOISE, 100, 110, 105, 16, 16, 16);
	BuildTexture(GetWeaponTextureWriteable(Slots::BAZOOKA).B, 8, false, Objects::RAW_NOISE, 120, 90, 75, 24, 24, 24);
	BuildTexture(GetWeaponTextureWriteable(Slots::BAZOOKA).C, 16, false, Objects::RAW_NOISE, 90, 110, 80, 16, 16, 16);
	BuildTexture(GetWeaponTextureWriteable(Slots::MACHINEGUN).A, 4, false, Objects::RAW_NOISE, 90, 100, 95, 16, 16, 16);
	BuildTexture(GetWeaponTextureWriteable(Slots::MACHINEGUN).B, 8, false, Objects::RAW_NOISE, 36, 24, 30, 16, 16, 16);
	BuildTexture(GetWeaponTextureWriteable(Slots::MACHINEGUN).C, 32, false, Objects::RAW_NOISE, 100, 110, 105, 16, 16, 16);
	//BuildNoiseTexture(Textures.Cube[Objects::CONCRETE],32,true, 110, 110, 110, 16,16,12 );

	BuildTexture(Zombie[0], 8, false, Objects::DEBUG);
	BuildTexture(Cube[static_cast<int>(Objects::CONCRETE)], 32, false, Objects::CONCRETE);
	BuildTexture(Cube[static_cast<int>(Objects::CRATE)], 32, false, Objects::CRATE);
	BuildTexture(Cube[static_cast<int>(Objects::DEBUG)], 8, false, Objects::DEBUG);
	BuildTexture(Cube[static_cast<int>(Objects::CRATEDEBUG)], 8, false, Objects::CRATEDEBUG);
	//BuildTexture(Clouds[0], 32, false, Objects::CLOUDS);

}
