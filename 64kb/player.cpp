#include "player.h"
#include "terrain.h"


Player::Weaponary& Player::GetWeapon(Slots slot)
{
	return Weapon[static_cast<int>(slot)];
}

void Player::AddAmmo(ubyte slot, int ammo)
{
	if (slot == 2)
	{
		if (HP < 64)
			HP += 192;
		else
		{
			HP = 255;
		}
		return;
	}
	if (slot == 3 && Weapon[3].Own)
	{
		slot = rnd(4, 5);
		ammo = MaxAmmo[slot] / 4;
	}
	Weapon[slot].Own = true;
	int ammo2 = Weapon[slot].Ammo;
	ammo2 += ammo;
	if (ammo2 > MaxAmmo[slot])
		Weapon[slot].Ammo = MaxAmmo[slot] - 1;
	else
	{
		Weapon[slot].Ammo = ammo2;
	}
}

bool Player::InWater() const
{
	return ypos < WATER_HEIGHT;
}


bool Player::NewPosition(const Terrain& terrain)
{
	for (int tries = 0; tries < 16; tries++)
	{
		xpos = rand() % terx;
		zpos = rand() % terz;
		if (terrain.scene[(int)xpos][(int)zpos].h > WATER_HEIGHT)
			return true;
	}
	return false;
}

bool Player::Spawn(const Terrain& terrain)
{
	HP = 100;
	if (NewPosition(terrain))
	{
		return true;
	}
	return false;
}