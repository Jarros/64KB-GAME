
//CODELINE FORMATTING: CTRL+K,CTRL+F;


//#include <dsound.h>
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library
//#include <stdio.h>		  
//#include <stdlib.h>
#include <math.h>
//#include <time.h>
#include <MMSystem.h>							 
#include <time.h>

//float PhysTestx; float PhysTesty; float PhysTestz;
//int InitPhys();
//void Simulate();



int rnd(int from, int to)
{
	return rand()%(1+to-from)+from;
}

float rndf()
{
	return rand()%64/64.0f;
}

float T=1.0f;

//int rndf()
//{
//	return rand()%256/256.0f;
//}
//
GLfloat FOV=90.0f;

const std::string gamename = "   TITLE   "; //"  LOLIGRA  "; //"  FUBGER  ";

bool Cheat=true;
bool Menu=false;
bool Stop=false;

BOOL	done=FALSE;	

typedef unsigned char ubyte ;

typedef unsigned int coord;

coord SCREEN_W=1920;
coord SCREEN_H=1080;			

	const coord chunksize=16;

	const coord chunksizeh=chunksize/2;

	const coord chunkx=128,chunkz=128;

 const int landsize = chunkx*chunksize;
 const int landsizeh = landsize/2;

	const coord RB_MAX=0;

#define maxb 255
#define	maxf 1.0f

short unsigned int SCREEN_WIDTH = SCREEN_W;
short unsigned int SCREEN_HEIGHT = SCREEN_H;//800;

const short unsigned int HSCREEN_WIDTH = SCREEN_WIDTH/2;
const short unsigned int HSCREEN_HEIGHT = SCREEN_HEIGHT/2;//800;

const coord LevelSizeX = chunkx*chunksize;
const coord LevelSizeZ = chunkz*chunksize;
 
struct GAMEMODE{
	struct MODES{
		enum MODE{
			menu,
			spectate,
			game,
			gamefly
		};
	}MODES;
	ubyte mode;
}GAME;
 
struct InvSlot
{
	ubyte MaxAmmo;
	ubyte Ammo;
};


enum objectslist
{
	NONE,GROUND,SAND,GRASS,GRASS1,GRASS2,GRASS3,ROCK,SOMELITTLESHTTYTHNG,ANDSOMEOTHERLITTLESHTTYSTUFF,CONCRETE,CRATE,DEBUG,CRATEDEBUG,RAW_NOISE,CLOUDS
};



enum Sluts{
	SOMENULLSHITHERE,HANDS,BLOCKS,SPADE,MACHINEGUN,BAZOOKA,SHOTGUN
};


const float maxy = 0.0f;

//ubyte Inventory	

	const coord EngineLevelSizeX = chunkx*chunksize+4; const coord EngineLevelSizeZ=chunkz*chunksize+4;

	const int MAX_Objs=65536;

	int GRASS_DIST=128;

	//static float scene[EngineLevelSizeX][EngineLevelSizeZ];


	//static bool physup=false;

	//static float xpos,ypos,zpos;

	const ubyte MaxAmmo[] = {  0,        0,      8,      0,      64,     8,      8  };
							// NULL // HANDS// BLOCK// SPADE// MACHG// BAZOK// SHOTG//

const float CAM_HEIGHT=5.0f;
const float WATER_HEIGHT=14.0f;
const float GROUND_HEIGHT=0.0f;
const float ADD_HEIGHT=10.0f;


const int terx=chunkx*chunksize, terz=chunkz*chunksize, ters=terx*terz, terh=32, terxh=terx/2, terzh=terz/2;

const coord CUBES_MAX=ters;

#define dotk 0.0003
const coord dotnum=(int)(ters*dotk)+1;

float GRAVITY=0.04f;

#define buildk 0.00004
const coord buildnum=(int)(ters*buildk)+1;

#define cratesk 0.0001
const coord cratenum=(int)(ters*cratesk)+1;

     #define FREQUENCY     440                 // 440hz = Musical A Note
     #define BUFFERSIZE    64*1024*16//8000*60//19440//4860                // 4k sound buffer
     
#define FPS 60
const DWORD DELTA	=	   1000/FPS;     
     #define PI            3.14159265358979

DWORD TICKWIN=0;
DWORD TICKFRM=0;
		
	float AdditionalCastleLODHeight=8.0f;

	const GLfloat nearclip = 0.1f;
	const GLfloat farclip = 4096.0f;//8096.0f;//
	//float FOV=45.0f;

	GLfloat fogstart = 96.0f;
	GLfloat fogend = 3072.0f;

	GLfloat fogclr[] = { 0.292f, 0.160f, maxf, maxf }; //{ 100.0/255.0, 10.0/215.0, 1, maxf };

	GLfloat fogclr2[] = { 0.192f, 0.160f, 0.6f, maxf }; //{ 100.0/255.0, 10.0/215.0, 1, maxf };

	GLfloat waterclr[] = { 0.0, 0.4, 1.0, 0.5 };

	GLfloat groundclr[] = { 180.0f/256.0f, 155.0f/256.0f, 110.0f/256.0f, 1.0 };

	const float projectile_half_side = 0.25f;

	const float EdgeDampingK = -1.0f;

	const float EdgeDampingMaxDist = 192.0f;

	int botsnum=16;
						
	GLubyte skyclrup[]    = { 62,122,255,255 };//102,62,255,255 };
		GLubyte skyclrdown[]    = { 62,122,255,255 };//80,200,255,255 };

		float movespeed=16.0;

		const int testscale=1024;


		
#define _ false
#define X true


bool font_letter[6][4*26] = {
	{ _,X,X,_,  X,X,X,_,  _,X,X,X,  X,X,X,_,  X,X,X,X,  X,X,X,X,  _,X,X,X,  X,_,_,X,  _,X,X,X,  _,X,X,X,  X,_,_,X,  X,_,_,_,  X,_,X,X,  X,_,_,X,  X,X,X,X,  X,X,X,X,  X,X,X,X,  X,X,X,X,  _,X,X,X,  X,X,X,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,X,X,X }, 
	{ X,_,_,X,  X,_,_,X,  X,_,_,_,  X,_,_,X,  X,_,_,_,  X,_,_,_,  X,_,_,_,  X,_,_,X,  _,_,X,_,  _,_,_,X,  X,_,X,_,  X,_,_,_,  X,X,_,X,  X,X,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,_,  _,_,X,_,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  _,_,_,X },
	{ X,_,_,X,  X,X,X,_,  X,_,_,_,  X,_,_,X,  X,X,X,_,  X,X,X,_,  X,_,_,_,  X,_,_,X,  _,_,X,_,  _,_,_,X,  X,X,X,_,  X,_,_,_,  X,X,_,X,  X,X,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,X,X,X,  X,X,X,_,  _,_,X,_,  X,_,_,X,  X,_,_,X,  X,_,_,X,  _,X,X,_,  X,X,X,X,  _,_,X,_ },
	{ X,X,X,X,  X,_,_,X,  X,_,_,_,  X,_,_,X,  X,_,_,_,  X,_,_,_,  X,_,X,X,  X,X,X,X,  _,_,X,_,  _,_,_,X,  X,_,_,X,  X,_,_,_,  X,_,_,X,  X,_,X,X,  X,_,_,X,  X,X,X,X,  X,_,_,X,  X,X,_,_,  _,_,_,X,  _,_,X,_,  X,_,_,X,  X,_,_,X,  X,_,X,_,  _,X,X,_,  _,_,_,X,  _,X,_,_ },
	{ X,_,_,X,  X,_,_,X,  X,_,_,_,  X,_,_,X,  X,_,_,_,  X,_,_,_,  X,_,_,X,  X,_,_,X,  _,_,X,_,  _,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,_,X,  X,_,X,X,  X,_,_,X,  X,_,_,_,  X,_,X,_,  X,_,X,_,  _,_,_,X,  _,_,X,_,  X,_,_,X,  X,_,_,X,  X,X,_,X,  X,_,_,X,  _,_,X,_,  X,_,_,_ },
	{ X,_,_,X,  X,X,X,X,  _,X,X,X,  X,X,X,_,  X,X,X,X,  X,_,_,_,  _,X,X,X,  X,_,_,X,  _,X,X,X,  X,X,X,_,  X,_,_,X,  X,X,X,X,  X,_,_,X,  X,_,_,X,  X,X,X,X,  X,_,_,_,  X,X,_,X,  X,_,_,X,  X,X,X,_,  _,_,X,_,  _,X,X,_,  _,X,X,_,  _,X,X,_,  X,_,_,X,  X,X,_,_,  X,X,X,X }
};


bool font_number[6][4*10] = {
	{ _,X,X,_,  _,_,X,_,  X,X,X,X,  X,X,X,X,  _,_,X,X,  X,X,X,X,  _,_,X,X,  X,X,X,X,  _,X,X,_,  _,X,X,_   }, 
	{ X,_,_,X,  _,X,X,_,  _,_,_,X,  _,_,_,X,  _,X,_,X,  X,_,_,_,  _,X,_,_,  _,_,_,X,  X,_,_,X,  X,_,_,X   },
	{ X,_,_,X,  _,_,X,_,  _,_,X,X,  _,X,X,_,  X,_,_,X,  X,X,X,X,  X,_,_,_,  _,_,X,_,  _,X,X,_,  X,X,X,X   },
	{ X,_,_,X,  _,_,X,_,  X,X,_,_,  _,_,_,X,  X,X,X,X,  _,_,_,X,  X,X,X,X,  _,X,_,_,  X,_,_,X,  _,_,_,X   },
	{ X,_,_,X,  _,_,X,_,  X,_,_,_,  _,_,_,X,  _,_,_,X,  _,_,_,X,  X,_,_,X,  _,X,_,_,  X,_,_,X,  _,_,X,_   },
	{ _,X,X,_,  _,X,X,X,  X,X,X,X,  X,X,X,X,  _,_,_,X,  X,X,X,X,  X,X,X,X,  _,X,_,_,  _,X,X,_,  X,X,_,_   }
};


#undef _
#undef X
