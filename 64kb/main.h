#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <time.h>
#include <algorithm>
#include <math.h>


template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
	return max(lower, min(n, upper));
}

//const bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
//extern bool	fullscreen;	// Fullscreen Flag Set To Fullscreen Mode By Default

void Message(char* message);

int rnd(int from, int to);

float rndf();

const std::string gamename = "   64KB   ";

extern float deltaTick;

typedef unsigned char ubyte;

typedef unsigned int coord;

const coord SCREEN_W = 1920;
const coord SCREEN_H = 1080;

const coord chunksize = 16;

const coord chunksizeh = chunksize / 2;

const coord chunkx = 128, chunkz = 128;

const int landsize = chunkx * chunksize;
const int landsizeh = landsize / 2;

const float PI = 3.14159265358979323846;
const float PIover180 = PI / 180.0;


const short unsigned int SCREEN_WIDTH = SCREEN_W;
const short unsigned int SCREEN_HEIGHT = SCREEN_H;//800;

const coord SCREEN_CENTER_X = SCREEN_WIDTH / 2;
const coord SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

const short unsigned int HSCREEN_WIDTH = SCREEN_WIDTH / 2;
const short unsigned int HSCREEN_HEIGHT = SCREEN_HEIGHT / 2;//800;

const coord LevelSizeX = chunkx * chunksize;
const coord LevelSizeZ = chunkz * chunksize;

extern float ratio;
extern float FOV;
extern BOOL	done;

void seedrand_tick();

void SwapBuffers();

void mainLoop();

class Vector2D
{
public:

	float scalar;
	float angle;

	Vector2D::Vector2D()
	{
		scalar = 0;
		angle = 0;
	}

	bool NotNull()
	{
		if (scalar)return true;
	}

	void MakeFromXY(float x, float y)
	{
		if (true)//TODO
		{
			if (false && x == 0)
				x = 0.000001;

			if (x != 0)
			angle = atan(y / x);
		}
		else {

			if (x != 0)
				angle = atan(y / x);
			else
				angle = PI / 2;
		}
		scalar = sqrt(y * y + x * x);

		//if(x<0 && y>0)angle=PI-angle;
		//if(x<0 && y<0)angle=PI+angle;

		if (x < 0)scalar = -scalar;

	}

	void Normalize()
	{
		if (scalar == 0)return;
		if (scalar < 0)
			scalar = -1.0f;
		else
			scalar = 1.0f;
	}

	float GetX()
	{
		return cos(angle) * scalar;
	}

	float GetY()
	{
		return sin(angle) * scalar;
	}

	Vector2D Rotate(float rotangle)
	{
		Vector2D a;
		a.angle = (angle + rotangle);
		a.scalar = scalar;
		return a;
	}

	//float GetZ2()
	//{
	//	return cos((angle-PI/2))*scalar;
	//}

	void Multiply(Vector2D v2)
	{
		MakeFromXY(GetX() + v2.GetX(), GetY() + v2.GetY());
	}

};

float Dist3D(float x, float y, float z, float x2, float y2, float z2);





const float maxy = 0.0f;

//ubyte DrawInventory	

const coord EngineLevelSizeX = chunkx * chunksize + 4; const coord EngineLevelSizeZ = chunkz * chunksize + 4;

const int MAX_Objs = 65536;

const int GRASS_DIST = 128;

//static float scene[EngineLevelSizeX][EngineLevelSizeZ];


//static bool physup=false;

//static float xpos,ypos,zpos;

// NULL // Slots::HANDS// BLOCK// Slots::SPADE// MACHG// BAZOK// SHOTG//

const float CAM_HEIGHT = 5.0f;
const float WATER_HEIGHT = 14.0f;
const float GROUND_HEIGHT = 0.0f;
const float ADD_HEIGHT = 10.0f;


const int terx = chunkx * chunksize, terz = chunkz * chunksize, ters = terx * terz, terh = 32, terxh = terx / 2, terzh = terz / 2;
const int terxl = terx - 1, terzl = terz - 1;
const coord CUBES_MAX = ters;


const float GRAVITY = 0.04f;






#define FPS 60
const bool simulateFPSdrops = false;
const DWORD DELTA = 1000 / FPS;
#define PI            3.14159265358979


const float AdditionalCastleLODHeight = 8.0f;

const float nearclip = 0.1f;
const float farclip = 4096.0f;//8096.0f;//
//float FOV=45.0f;

const float fogstart = 96.0f;
const float fogend = 3072.0f;

const float fogclr[] = { 97 / 255.f,186 / 255.f,255 / 255.f,255/255.f }; //{ 100.0/255.0, 10.0/215.0, 1, 1.0f };

//float fogclr2[] = { 0.192f, 0.160f, 0.6f, 1.0f }; //{ 100.0/255.0, 10.0/215.0, 1, 1.0f };

const float waterclr[] = { 0.0, 0.4, 1.0, 0.5 };

const float groundclr[] = { 180.0f / 256.0f, 155.0f / 256.0f, 110.0f / 256.0f, 1.0 };

const float projectile_half_side = 0.25f;

const float EdgeDampingK = -1.0f;

const float EdgeDampingMaxDist = 192.0f;


const GLubyte skyclrup[] = { 97,186,255,255 };//102,62,255,255 };
const GLubyte skyclrdown[] = { 97,186,255,255 };//80,200,255,255 };


const int testscale = 1024;



