#include "main.h"
#include "render.h"
#include "textures.h"
#include "terrain.h"
#include "game.h"
#include "player.h"
#include "textures.h"
#include "input.h"
#include "hud.h"
#include "sound.h"
#include "bots.h"
#include "projectile.h"

void Render::Rotate(float angle_pitch, float angle_yaw, float angle_roll)
{
	glRotatef(angle_pitch, 1.0f, 0, 0);
	glRotatef(angle_yaw, 0, 1.0f, 0);
	glRotatef(angle_roll, 0, 0, 1.0f);
}

void Render::Clear()
{
	glLoadIdentity();
	gluPerspective(FOV, ratio, nearclip, farclip);

	//if(!OnLevel(xpos,ypos,zpos) && GAME.mode==GAME.MODES.game)
	//	GameOver=true;
	/*if (GAME.mode == GAME.MODES.spectate)
		GAME.mode = GAME.MODES.game;
	*/
	static float angle = 0.0f;

	coord a;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void Render::TransformScene(const Player& player)
{
	Rotate(player.xrot, player.yrot, player.zrot);
	float xscene = -player.xpos;
	float yscene = -player.ypos;
	float zscene = -player.zpos;
	glTranslatef(xscene, yscene, zscene);
}

void Render::Fog(const Player& player)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);        // Fog Mode
	glFogfv(GL_FOG_COLOR, fogclr);            // Set Fog Color
												//glFogf(GL_FOG_DENSITY, 0.35f);              // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST);          // Fog Hint Value
	if (!player.InWater())
	{
		glFogf(GL_FOG_START, fogstart);
		glFogf(GL_FOG_END, fogend);               // Fog End Depth
	}
	else
	{
		glFogf(GL_FOG_START, 0.0f);             // Fog Start Depth
		glFogf(GL_FOG_END, 64.0f);
	}
	glEnable(GL_FOG);                   // Enables GL_FOG
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void Render::Terrain(const ::Terrain &terrain, const Player &player)
{
	for (int x2 = 0; x2 < chunkx; x2++) {
		for (int z2 = 0; z2 < chunkz; z2++) {
			bLOD_ = 0;
			float dist = sqrt(pow(x2 * chunksize + chunksize / 2.0 - player.xpos, 2.0) + pow(z2 * chunksize + chunksize / 2.0 - player.zpos, 2.0));

			if (dist > 64.0f) bLOD_++;
			if (dist > 128.0f) bLOD_++;
			//if(dist > 512.0f) LOD=16;
			if (dist > 512.0f) bLOD_++;
			//if(dist > 768.0f) bLOD++;
			//if(dist > 1024.0f) bLOD++;

			LOD_ = (int)pow(2.0f, bLOD_);

			if (LOD_ < 2)glShadeModel(GL_FLAT); else glShadeModel(GL_SMOOTH);

			coord ChunkdivLOD = chunksize / LOD_;

			if (LOD_ == 1)
			{
				for (int x = 0 + chunksize * x2; x < chunksize + chunksize * x2; x++) {
					glBegin(GL_QUAD_STRIP);
					for (int z = chunksize * z2; z < chunksize + chunksize * z2 + 1; z++) {
						coord LODx2 = (x + 1), LODx3 = (x + 2);//LODz2=(z+1), LODz3=(z+2);
						float v0b[] = { x,  terrain.scene[x][z].h,  z };
						float v3b[] = { LODx2,  terrain.scene[LODx2][z].h,  z };
						GLubyte clr0[] = { terrain.clr0rgb[x][z].r,terrain.clr0rgb[x][z].g,terrain.clr0rgb[x][z].b,255 };
						GLubyte clr3[] = { terrain.clr0rgb[LODx2][z].r,terrain.clr0rgb[LODx2][z].g,terrain.clr0rgb[LODx2][z].b,255 };

						glColor4ubv(clr0);// 0 0
						glVertex3fv(v0b);
						glColor4ubv(clr3);// 1 0
						glVertex3fv(v3b);
					}
					glEnd();
				}
			}
			else {
				glDisable(GL_TEXTURE_2D);

				if (LOD_ < 30)glShadeModel(GL_FLAT); else glShadeModel(GL_SMOOTH);

				for (int x = 0 + chunksize * x2 / LOD_; x < chunksize / LOD_ + chunksize * x2 / LOD_; x++) {
					glBegin(GL_QUAD_STRIP);
					for (int z = chunksize * z2 / LOD_; z < chunksize / LOD_ + chunksize * z2 / LOD_ + 2; z++) {
						coord LODx = (x)*LOD_, LODz = (z)*LOD_, LODx2 = (x + 1) * LOD_;//, LODx3=(x+2)*LOD;// LODy2=(z+1)*LOD;//, LODy3=(z+2)*LOD;
						float addx = 0, addx2 = 0, addz = 0, addz2 = 0;
						float megalod;
						if (LOD_ > 1)
						{
							float add = -bLOD_ * 2 + 1;//-0.0-sqrt(LOD*1.0);//-LOD*2.0;
							if (x == x2 * chunksize / LOD_) { addx = add; }
							if (z == z2 * chunksize / LOD_) { addz = add; }
							if (x == ((x2 + 1) * chunksize / LOD_ - 1)) { addx2 = add; }
							if (z == ((z2 + 1) * chunksize / LOD_ - 1)) { addz2 = add; }
						}
						float v0b[] = { LODx,  terrain.scene[LODx][LODz].h + addx + addz,  LODz };
						float v3b[] = { LODx2,  terrain.scene[LODx2][LODz].h + addx2 + addz,  LODz };
						GLubyte clr0[] = { terrain.clr0rgb[LODx][LODz].r,terrain.clr0rgb[LODx][LODz].g,terrain.clr0rgb[LODx][LODz].b,255 };
						GLubyte clr3[] = { terrain.clr0rgb[LODx2][LODz].r,terrain.clr0rgb[LODx2][LODz].g,terrain.clr0rgb[LODx2][LODz].b,255 };

						glColor4ubv(clr0);// 0 0
						glVertex3fv(v0b);
						glColor4ubv(clr3);// 1 0
						glVertex3fv(v3b);

					}
					glEnd();
				}
			}
		}
	}
}

void Render::Grass(const ::Terrain &terrain, const Player &player) {

	float cosa = cos(player.yrot * PIover180);
	float sina = sin(player.yrot * PIover180);

	int currentchunkx = (int)(player.xpos / chunksize);
	int currentchunkz = (int)(player.zpos / chunksize);

	int fullnum = 5;

	for (int x2 = currentchunkx - fullnum; x2 < currentchunkx + fullnum; x2++)
		for (int z2 = currentchunkz - fullnum; z2 < currentchunkz + fullnum; z2++)
		{
			if (x2 > 0 && z2 > 0)
			{

				for (int x = x2 * chunksize; x < x2 * chunksize + chunksize; x++)
					for (int z = z2 * chunksize; z < z2 * chunksize + chunksize; z++)
					{
						if (terrain.IsGrass(terrain.scene[x][z]))
						{

							float addheight = 0.0f, height = (static_cast<int>(terrain.scene[x][z].type) - static_cast<int>(Objects::GRASS1)) / 2.0f + 1.0f;//-dist/48.0f;
							float width = 0.35f- height*0.1f;
							GLubyte clr[] = { 20,140,20,255 };
							GLubyte clr0[] = { terrain.clr0rgb[x][z].r - 8,terrain.clr0rgb[x][z].g + 8,terrain.clr0rgb[x][z].b - 8,255 };

							float vert1[] = { x, terrain.scene[x][z].h - 0.5f, z };
							float vert2[] = { x, terrain.scene[x][z].h + addheight + height, z };
							float vert3[] = { x - cosa * width, terrain.scene[x][z].h - 0.5f + addheight, z - sina * width };
							float vert4[] = { x - cosa * width, terrain.scene[x][z].h + height + addheight, z - sina * width };

							glBegin(GL_QUADS);

							glColor4ubv(clr0);
							//glTexCoord2f(8, 0);
							glVertex3fv(vert2);
							//glTexCoord2f(8, 8);
							glVertex3fv(vert1);
							//glTexCoord2f(0, 8);
							glVertex3fv(vert3);
							//glTexCoord2f(0, 0);
							glVertex3fv(vert4);
							glEnd();
						}
					}
			}
		}




}

void Render::SpecialObjects(const Textures& textures)
{
	glFrontFace(GL_CCW);
	

	for (coord a = 0; a < 10; a++)
	{
		if (projectile.IsExists)
		{
			Projectile(projectile.px, projectile.py, projectile.pz, textures.GetWeaponTexture(Slots::SPADE).B);
			
		}
	}
		
}

void Render::Cubes(const ::Terrain& terrain, const Player& player, const Textures& textures)
{

	int xposh = player.xpos / 2;
	int zposh = player.zpos / 2;

	for (int x4 = 0; x4 < chunkx; x4++)
		for (int z4 = 0; z4 < chunkz; z4++)
		{
			if ((abs((float)(x4 * chunksize - chunksizeh) - player.xpos) > 64 || abs((float)(z4 * chunksize - chunksizeh) - player.zpos) > 64))
				for (coord cn = 0; cn < terrain.cubenum[x4][z4]; cn++)
				{
					Terrain::cube a = terrain.cubes[cn][x4][z4];

					if (terrain.cubescene[a.x][a.y][a.z].type != Objects::NONE)
						if (abs((int)a.x - xposh) < 48)
							if (abs((int)a.z - zposh) < 48)
								Cube(a.x * 2, a.y * 2, a.z * 2, 0, 0, 0, 1.0f, terrain.cubescene[a.x][a.y][a.z].type, textures);
				}
		}
}

void Render::BuildingLODS(const ::Terrain &terrain, const Player &player, const Textures &textures)
{
	for (coord cn = 0; cn < buildnum; cn++)
	{
		if (terrain.Castles[cn].Enabled && (abs((float)terrain.Castles[cn].x - player.xpos) > 64 || abs((float)terrain.Castles[cn].z - player.zpos) > 64))
			Box(terrain.Castles[cn].x, terrain.Castles[cn].y - AdditionalCastleLODHeight, terrain.Castles[cn].z, terrain.Castles[cn].w, terrain.Castles[cn].h + AdditionalCastleLODHeight * 2, terrain.Castles[cn].l, 0.0f, 0.0f, 0.0f, textures.Cube[static_cast<int>(Objects::CONCRETE)]);

	}
}

void Render::GameOver(const Game& game)
{
	if (game.GameOver)
	{
		glLoadIdentity();
		BloodFilter();
		return;
	}
}

void Render::DebugCube(const Input& input, const Textures& textures)
{
	glColor4f(1.0, 1.0, 1.0, 1.0);

	if (input.ydebspec == 0)
		Cube(input.xdeb * 2, input.ydeb * 2, input.zdeb * 2, 0, 0, 0, 1, Objects::DEBUG, textures);
	else
	{
		Cube(input.xdeb * 2, input.ydebspec * 2, input.zdeb * 2, 0, 0, 0, 1.1, Objects::CRATEDEBUG, textures);
	}
}

void Render::Water(const Player& player)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	bool GroundBase = false;
	
	float h;
	if (GroundBase)
	{
		h = -0.25f;
		glColor4fv(groundclr);
		glDisable(GL_BLEND);
	}
	else
	{
		h = WATER_HEIGHT;
		glColor4fv(waterclr);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	int size = 6584;
	glPushMatrix();

	for (int a = 0; a < 4; a++)
	{

		int x = ((a == 3) || (a == 2)) * size;
		int z = ((a == 1) || (a == 2)) * size;

		float v0b[] = { -size + x + landsizeh, h, -size + z + landsizeh };
		float v1b[] = { x + landsizeh, h, -size + z + landsizeh };
		float v2b[] = { x + landsizeh, h, z + landsizeh };
		float v3b[] = { -size + x + landsizeh, h, z + landsizeh };

		glBegin(GL_TRIANGLES);

		glVertex3fv(v0b);

		glVertex3fv(v1b);

		glVertex3fv(v2b);

		glVertex3fv(v0b);

		glVertex3fv(v2b);

		glVertex3fv(v3b);
		glEnd();

	}

	glDisable(GL_BLEND);
	glPopMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);



	if (player.InWater())
	{
		//HP -= 0.25f;
		glLoadIdentity();
		SeaFilter();
	}
}

void Render::Menu(::BotManager& bots, Game& game, Player& player, ::HUD& hud, ::Terrain& terrain, const Input& input, Sound& sound)
{

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_CULL_FACE);

	glDisable(GL_BLEND);
	glDisable(GL_FOG);

	glLoadIdentity();
	glColor4f(1.0, 1.0, 1.0, 1.0);

	if (game.Menu)
	{
		hud.DrawMenu(bots, game, player, terrain, input, sound);
	}
	if (true)
	{
	}
}


void Render::HUD(Player& player, const Game& game, ::HUD &hud, const Input& input)
{
	if (!game.Menu) {
		hud.DrawHUD(player);
	}
}


void Render::drawBot(::Bot& bot, const Textures &textures, const eClrs eClr) {
	//::Bot& bot = bots.bot[n];
	ubyte color = bot.HPb;
	{
		//if (Dist3D(x, y, z, xpos, ypos, zpos) > 768.0f)
		//	return;

		float cl = 0.5f + color / 512.0f;
		glDisable(GL_BLEND);

		switch (eClr) {
			case eClrs::default:
				glColor4f(1.0f, cl, cl, 1.0f);
				break;
			case eClrs::blue:
				glColor4f(0.5f, 0.5f, 1.0f, 1.0f);
			break;
		}

		float yr2 = bot.yr_i * 180 / PI;
		Box(bot.x_i, bot.y_i, bot.z_i, 2.5f, 5.5f, 1.25f, 0.0f, yr2, 0.0f, textures.GetWeaponTexture(Slots::BAZOOKA).B);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		{
			Vector2D a;

			a.MakeFromXY(0.15, 0.0);
			a = a.Rotate(bot.yr_i);
			Box(bot.x_i + a.GetY(), bot.y_i + 0.5f, bot.z_i + a.GetX(), 1.8f, 0.2f, 1.5f, 0.0f, yr2, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);

			a.MakeFromXY(0.5, 0.45);
			a = a.Rotate(bot.yr_i);
			Box(bot.x_i + a.GetY(), bot.y_i + 1.5f, bot.z_i + a.GetX(), 0.2f, 0.2f, 0.5f, 0.0f, yr2, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);

			a.MakeFromXY(0.5, -0.45);
			a = a.Rotate(bot.yr_i);
			Box(bot.x_i + a.GetY(), bot.y_i + 1.5f, bot.z_i + a.GetX(), 0.2f, 0.2f, 0.5f, 0.0f, yr2, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);
		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	}
}

void Render::BotManager(::BotManager& bots, const Textures& textures)
{
	for (coord n = 0; n < botsnum; n++)
	{
		drawBot(bots.bot[n], textures);
	}

}

void Render::Cycle(::Terrain& terrain, Player& player, Textures& textures, Game& game, ::HUD& hud, Sound& sound, ::BotManager &bots, Input &input)
{

	Clear();

	Sky(game, player);

	TransformScene(player);

	Fog(player);

	Terrain(terrain, player);

	Grass(terrain, player);

	SpecialObjects(textures);
	projectile.Move(bots, terrain, sound);

	BotManager(bots, textures);

	Cubes(terrain, player, textures);

	BuildingLODS(terrain, player, textures);

	GameOver(game);

	DebugCube(input, textures);

	Water(player);

	//Clouds();

	Menu(bots, game, player, hud, terrain, input, sound);

	Hands(game, textures, input, player, hud);

	HUD(player, game, hud, input);

	SwapBuffers();
}



void Render::PrintLine(float x, float y, float z, float x2, float y2, float z2, ubyte r, ubyte g, ubyte b, ubyte a, float width, bool machinegun)
{
	float v[] = { x,y,z };
	float v2[] = { x2,y2,z2 };
	float R = r / 256.0f, G = g / 256.0f, B = b / 256.0f, A = a / 256.0f;
	if (machinegun)
	{
		glLineStipple(16, 0x1C47);
		if (rnd(0, 3) == 0)
		{
			glLineWidth(width);
			glEnable(GL_BLEND);
			glColor4f(R, G, B, A);
			glBegin(GL_LINES);

			glVertex3fv(v2);
			glVertex3fv(v);
			glEnd();
			glDisable(GL_BLEND);
			glLineStipple(1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);
		}
	}
}

void Render::CubeFaces(float v0a[], float v1a[], float v2a[], float v3a[], float v0b[], float v1b[], float v2b[], float v3b[])
{
	const float one = 1.0f;
	const float zer = 0.0f;

	glTexCoord2f(zer, zer);
	glVertex3fv(v1b);
	glTexCoord2f(one, zer);
	glVertex3fv(v2b);
	glTexCoord2f(one, one);
	glVertex3fv(v3b);
	glTexCoord2f(zer, one);
	glVertex3fv(v0b);

	glTexCoord2f(zer, zer);
	glVertex3fv(v1a);	glTexCoord2f(one, zer);
	glVertex3fv(v0a);	glTexCoord2f(one, one);
	glVertex3fv(v3a);	glTexCoord2f(zer, one);
	glVertex3fv(v2a);

	glTexCoord2f(zer, zer);
	glVertex3fv(v2b);	glTexCoord2f(one, zer);
	glVertex3fv(v2a);	glTexCoord2f(one, one);
	glVertex3fv(v3a);	glTexCoord2f(zer, one);
	glVertex3fv(v3b);

	glTexCoord2f(zer, zer);
	glVertex3fv(v1b);	glTexCoord2f(one, zer);
	glVertex3fv(v0b);	glTexCoord2f(one, one);
	glVertex3fv(v0a);	glTexCoord2f(zer, one);
	glVertex3fv(v1a);

	glTexCoord2f(zer, zer);
	glVertex3fv(v2a);	glTexCoord2f(one, zer);
	glVertex3fv(v2b);	glTexCoord2f(one, one);
	glVertex3fv(v1b);	glTexCoord2f(zer, one);
	glVertex3fv(v1a);

	glTexCoord2f(zer, zer);
	glVertex3fv(v3a);	glTexCoord2f(one, zer);
	glVertex3fv(v0a);	glTexCoord2f(one, one);
	glVertex3fv(v0b);	glTexCoord2f(zer, one);
	glVertex3fv(v3b);
}

void Render::ShortCubeFaces(float W, float H, float L)
{

	float v3a[] = { -W,-H,-L };
	float v2a[] = { -W,H,-L };
	float v1a[] = { W,H,-L };
	float v0a[] = { W,-H,-L };
	float v3b[] = { -W,-H,L };
	float v2b[] = { -W,H,L };
	float v1b[] = { W,H,L };
	float v0b[] = { W,-H,L };

	CubeFaces(v0a, v1a, v2a, v3a, v0b, v1b, v2b, v3b);
}

void Render::Projectile(float x, float y, float z, unsigned int TEXTURE)
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TEXTURE);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	float ang = rand() % 1024 / 16.0;
	glPushMatrix();
	glTranslatef(x, y, z);

	ShortCubeFaces(projectile_half_side, projectile_half_side, projectile_half_side);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void Render::Cube(float x, float y, float z, float pitch, float yaw, float roll, float h, Objects type, const Textures& textures)
{
	//float h=cubeside/2;
	float v3a[] = { -h + x,-h + y,-h + z };
	float v2a[] = { -h + x,h + y,-h + z };
	float v1a[] = { h + x,h + y,-h + z };
	float v0a[] = { h + x,-h + y,-h + z };
	float v3b[] = { -h + x,-h + y,h + z };
	float v2b[] = { -h + x,h + y,h + z };
	float v1b[] = { h + x,h + y,h + z };
	float v0b[] = { h + x,-h + y,h + z };

	if (type != drawtype)
	{
		//glEnd();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures.Cube[static_cast<int>(type)]);
		drawtype = type;

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glColor4f(1.0, 1.0, 1.0, 1.0);

		if (type == Objects::DEBUG || type == Objects::CRATEDEBUG)
		{
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glColor4f(1.0, 1.0, 1.0, 0.25);
		}

	}

	glBegin(GL_QUADS);
	CubeFaces(v0a, v1a, v2a, v3a, v0b, v1b, v2b, v3b);
	glEnd();

}

void Render::Box(float x, float y, float z, float W, float H, float L, float pitch, float yaw, float roll, GLuint TEXTURE)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TEXTURE);

	glPushMatrix();

	glTranslatef(x, y, z);
	Rotate(pitch, yaw, roll);

	glBegin(GL_QUADS);
	ShortCubeFaces(W / 2, H / 2, L / 2);
	glEnd();

	glPopMatrix();
}

void Render::Hands(const Game& game, const Textures& textures, const Input& input, const Player& player, const ::HUD& hud)
{
	if (game.Menu)
		return;


	glDisable(GL_DEPTH_TEST);
	if (player.InWater())
		glColor4f(0.7f, 0.7f, 1.0f, 1.0f);
	switch (hud.slot)
	{
	case Slots::HANDS:

		if (input.RMB.press)
		{
			Box(1.0f, -2.75f, -0.75f, 1.0f, 1.0f, 4.0f, 60.0f + rndf(), -0.0f + rndf(), -0 + rndf(), textures.GetWeaponTexture(Slots::HANDS).A);
			Box(-1.0f, -2.75f, -0.75f, 1.0f, 1.0f, 4.0f, 60.0f + rndf(), -0.0f + rndf(), -0 + rndf(), textures.GetWeaponTexture(Slots::HANDS).A);
		}
		else
		{
			static int i = 0;// rand() % 2;
			if (input.LMB.press && input.Hand)
				Box(-1.5f, -2.75f, -1.5f, 1.0f, 1.0f, 4.0f, 45.0f + rndf(), -0.0f + rndf(), -0 + rndf(), textures.GetWeaponTexture(Slots::HANDS).A);
			else
				Box(-2.0f, -3.5f, -1.5f, 1.0f, 1.0f, 4.0f, 45.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::HANDS).A);
			if (input.LMB.press && !input.Hand)
				Box(1.5f, -2.75f, -1.5f, 1.0f, 1.0f, 4.0f, 45.0f + rndf(), -0.0f + rndf(), -0 + rndf(), textures.GetWeaponTexture(Slots::HANDS).A);
			else
				Box(2.0f, -3.5f, -1.5f, 1.0f, 1.0f, 4.0f, 45.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::HANDS).A);
		}
		break;
	case Slots::SPADE:
		if (input.LMB.press)
		{
			Box(1.0f, -2.25f, -1.5f, -0.75f, 0.75f, 4.0f, 45.0f + rndf(), 0.0f + rndf(), 0.0f + rndf(), textures.GetWeaponTexture(Slots::SPADE).A);
		}
		else
		{
			if (input.RMB.press)
			{
				Box(1.0f, -2.25f, -1.5f, -0.75f, 0.75f, 4.0f, 20.0f + rndf(), 0.0f + rndf(), 0.0f + rndf(), textures.GetWeaponTexture(Slots::SPADE).A);
			}
			else
			{
				Box(1.25f, -1.75f, -1.75f, -0.75f, 0.75f, 4.0f, 60.0f, -0.0f, 0.0f, textures.GetWeaponTexture(Slots::SPADE).A);
			}
		}
		break;

	case Slots::BLOCKS:
		if (input.LMB.press)
		{
			Box(-1.5f, -2.75f, -2.5f, 1.0f, 1.0f, 4.0f, 45.0f, -0.0f, -0, textures.GetWeaponTexture(Slots::HANDS).A);
			Box(1.5f, -2.75f, -2.5f, 1.0f, 1.0f, 4.0f, 45.0f, -0.0f, -0, textures.GetWeaponTexture(Slots::HANDS).A);
		}
		else
			if (input.RMB.press)
			{
				Box(1.5f, -2.75f, -0.5f, 1.0f, 1.0f, 4.0f, 45.0f, -0.0f, -0, textures.GetWeaponTexture(Slots::HANDS).A);
				Box(-1.5f, -2.75f, -0.5f, 1.0f, 1.0f, 4.0f, 45.0f, -0.0f, -0, textures.GetWeaponTexture(Slots::HANDS).A);
			}
			else
			{
				Box(-1.0f, -3.5f, -1.5f, 1.0f, 1.0f, 4.0f, 45.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::HANDS).A);
				Box(1.0f, -3.5f, -1.5f, 1.0f, 1.0f, 4.0f, 45.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::HANDS).A);
			}
		break;

	case Slots::MACHINEGUN:
		if (input.LMB.press)
		{
			float rnd1 = rndf() / 4.0f, rnd2 = rndf() * 2.0f;

			glEnable(GL_DEPTH_TEST);
			PrintLine(0.25f, -0.25f, -0.5f, 0.15f + rnd(-4, 3), -0.25f + rnd(-4, 3), -0.5f - 256.0f + rnd1, rand() % 30 + 200, rand() % 50 + 206, 150, 100, rand() % 16 / 8.0f, true);
			PrintLine(0.25f, -0.25f, -0.5f, 0.15f + rnd(-4, 3), -0.25f + rnd(-4, 3), -0.5f - 256.0f + rnd1, rand() % 30 + 200, rand() % 50 + 206, 150, 100, rand() % 16 / 8.0f, true);
			glDisable(GL_DEPTH_TEST);
			coord b = rnd(24, 39);
			for (int a = 0; a < b; a++)
				PrintLine(0.25f, -0.25f, -0.75f + rnd1, 0.25f + rand() % 8 / 32.0f - 0.125, -0.25f + rand() % 8 / 32.0f - 0.125f, -1.25f + rnd1, rand() % 26 + 230, rand() % 30 + 160, 120, 120, 2.0f + rand() % 8 / 2.0f, true);

			Box(2.0f, -3.25f, -5.0f + rnd1, 0.5f, 1.0f, 0.25f, 0.0f + rnd2, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);
			Box(2.0f, -2.0f, -4.0f + rnd1, 0.75f, 1.5f, 3.0f, 0.0f + rnd2, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).C);
			Box(2.0f, -1.0f, -5.0f + rnd1, 0.25f, 0.5f, 0.15f, 0.0f + rnd2, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).A);
			Box(2.0f, -1.5f, -2.0f + rnd1, 0.5f, 0.75f, 0.75f, 45.0f + rnd2, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);
		}
		else
		{
			Box(2.0f, -3.25f, -5.0f, 0.5f, 1.0f, 0.25f, 0.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);
			Box(2.0f, -2.0f, -4.0f, 0.75f, 1.5f, 3.0f, 0.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).C);
			Box(2.0f, -1.0f, -5.0f, 0.25f, 0.5f, 0.15f, 0.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).A);
			Box(2.0f, -1.5f, -2.0f, 0.5f, 0.75f, 0.75f, 45.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);
		}
		break;
	case Slots::SHOTGUN:
		break;
	case Slots::BAZOOKA:
		if (input.LMB.press)
		{
			float rnd1 = 0.5f, rnd2 = 16.0f;//float rnd1=rand()%16/64.0f, rnd2=//rand()%16/8.0f;
			Box(2.0f, -3.75f, -5.0f + rnd1, 1.5f, 1.5f, 0.5f, 0.0f + rnd2, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);
			Box(2.0f, -2.0f, -4.0f + rnd1, 2.0f, 2.0f, 4.0f, 0.0f + rnd2, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).C);
			Box(2.0f, -0.75f, -5.0f + rnd1, 0.5f, 0.5f, 0.15f, 0.0f + rnd2, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).A);
		}
		else
		{
			Box(2.0f, -3.75f, -5.0f, 1.5f, 1.5f, 0.5f, 0.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).B);
			Box(2.0f, -2.0f, -4.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).C);
			Box(2.0f, -0.75f, -5.0f, 0.5f, 0.5f, 0.15f, 0.0f, 0.0f, 0.0f, textures.GetWeaponTexture(Slots::MACHINEGUN).A);
		}
		break;
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_BLEND);
	glDisable(GL_FOG);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
}

void Render::Clouds(const Textures& textures)
{
	glDisable(GL_FOG);
	float b = 64.0f;
	float h = 256.0f;
	glColor4f(0.8, 0.8, 0.8, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int size = 3584;
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures.Weapon[1].A);

	for (int a = 0; a < 4; a++)
	{
		int x = ((a == 3) || (a == 2)) * size;
		int z = ((a == 1) || (a == 2)) * size;

		float v0b[] = { -size + x + landsizeh, h, -size + z + landsizeh };
		float v1b[] = { 0 + x + landsizeh, h, -size + z + landsizeh };
		float v2b[] = { 0 + x + landsizeh, h, z + landsizeh };
		float v3b[] = { -size + x + landsizeh, h, z + landsizeh };

		glBegin(GL_QUADS);
		glTexCoord2f(b, 0.0);
		glVertex3fv(v0b);
		glTexCoord2f(b, b);
		glVertex3fv(v1b);
		glTexCoord2f(0.0, b);
		glVertex3fv(v2b);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(v3b);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glPopMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_FOG);
}



void Render::Sky(const Game& game, const Player& player)
{
	if (!game.Debug && !player.InWater())
	{
		glDisable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		float v0b[] = { -2.0f, -1.0f, -1.0f };
		float v1b[] = { 2.0f, -1.0f, -1.0f };
		float v2b[] = { 2.0f,  1.0f, -1.0f };
		float v3b[] = { -2.0f,  1.0f, -1.0f };

		glBegin(GL_TRIANGLES);
		glColor4ubv(skyclrdown);
		glVertex3fv(v0b);
		glColor4ubv(skyclrdown);
		glVertex3fv(v1b);
		glColor4ubv(skyclrup);
		glVertex3fv(v2b);
		glColor4ubv(skyclrdown);
		glVertex3fv(v0b);
		glColor4ubv(skyclrup);
		glVertex3fv(v2b);
		glColor4ubv(skyclrup);
		glVertex3fv(v3b);
		glEnd();
		glEnable(GL_DEPTH_TEST);
	}
	glLoadIdentity();
}

void Render::SeaFilter()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	float v0b[] = { -2.0f, -1.0f, -1.0f };
	float v1b[] = { 2.0f, -1.0f, -1.0f };
	float v2b[] = { 2.0f,  1.0f, -1.0f };
	float v3b[] = { -2.0f,  1.0f, -1.0f };
	GLubyte clr0[] = { 100,200,255,255 };
	GLubyte clr1[] = { 100,180,255,255 };
	GLubyte clr2[] = { 132,22,255,255 };
	GLubyte clr3[] = { 132,22,255,255 };

	glBegin(GL_TRIANGLES);
	glColor4f(0.0, 0.4, 1.0, 0.5);
	glVertex3fv(v0b);
	glVertex3fv(v1b);
	glVertex3fv(v2b);
	glVertex3fv(v0b);
	glVertex3fv(v2b);
	glVertex3fv(v3b);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


}

void Render::BloodFilter()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	float v0b[] = { -2.0f, -1.0f, -1.0f };
	float v1b[] = { 2.0f, -1.0f, -1.0f };
	float v2b[] = { 2.0f,  1.0f, -1.0f };
	float v3b[] = { -2.0f,  1.0f, -1.0f };
	GLubyte clr0[] = { 100,200,255,255 };
	GLubyte clr1[] = { 100,180,255,255 };
	GLubyte clr2[] = { 132,22,255,255 };
	GLubyte clr3[] = { 132,22,255,255 };

	glBegin(GL_TRIANGLES);
	glColor4f(0.7, 0.2, 0.5, 0.5);
	glVertex3fv(v0b);
	glVertex3fv(v1b);
	glVertex3fv(v2b);
	glVertex3fv(v0b);
	glVertex3fv(v2b);
	glVertex3fv(v3b);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}