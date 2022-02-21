#pragma once
#include "main.h"

struct Terrain;

const ubyte MaxAmmo[] = { 0,        0,      8,      0,      64,     8,      8 };

enum class Slots : byte {
	NONE, HANDS, BLOCKS, SPADE, MACHINEGUN, BAZOOKA, SHOTGUN
};

class Player
{
public:
	bool Spawn(const Terrain& terrain);
	bool InWater() const;
	void AddAmmo(ubyte slot, int ammo);
	float HP = 255;
	float movespeed = 16.0;
	struct Weaponary {
		GLubyte Ammo;
		bool Own;
	}Weapon[8];
	Weaponary& GetWeapon(Slots slot);

	float xpos, ypos, zpos;
	float xrot, yrot, zrot;

private:
	

	struct InvSlot
	{
		ubyte MaxAmmo;
		ubyte Ammo;
	};

	bool NewPosition(const Terrain& terrain);

};

