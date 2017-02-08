#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_mixer.h"

#include <math.h>


//#include <windows.h>

//#pragma comment (lib, "ws2_32.lib")    // ищем нужную библиотеку
//#include <winsock2.h>        // winsock2.h: typedef u_int SOCKET









//#include "Windows.h"

//SDL_Surface* hello = NULL;
 Mix_Music *music; 
SDL_Surface* screen = NULL;

SDL_Surface* screenbuffer = NULL;

SDL_Surface* postprocess = NULL;
SDL_Surface* coastwater = NULL;
SDL_Surface* Boat;

SDL_Surface* waterjoint = NULL;

SDL_Surface* Aim[3];
SDL_Surface* Sun;
SDL_Surface* SunFlare;
SDL_Surface* TestCloud;
SDL_Surface* PickAxe;

SDL_Surface* Meteor;
SDL_Surface* Trail;

//SDL_Surface* CloudBMP[8];

SDL_Surface* HUD1;
SDL_Surface* HUD2;

SDL_Event eventing;

int StartTime;

typedef short unsigned int coord;
typedef Uint16 clr;
typedef unsigned char ubyte;

unsigned int timer=0;
int meh;
int zz=0,zz2=0,zz3=0,zz4=0;

bool BeepLR[2];
unsigned char Beep,BeepSpeed=16;

const short int DeltaTime=30;

//const SDL_VideoInfo* myPointer = SDL_GetVideoInfo();


const short unsigned int SCREEN_WIDTH = 1280;
const short unsigned int SCREEN_HEIGHT = 1023;

const coord SCREEN_CENTER_X = SCREEN_WIDTH/2;
const coord SCREEN_CENTER_Y = SCREEN_HEIGHT/2;

const short int aimsizex = 16;
const short int aimsizey = 16;



const short unsigned int LevelSizeX = 10000;//SCREEN_WIDTH*16;
const short unsigned int LevelSizeY = 8184;//SCREEN_HEIGHT*8;




const short unsigned int MaximumHeight = SCREEN_HEIGHT*6-256;
const short signed int MathY = -LevelSizeY+SCREEN_HEIGHT;

const short unsigned int SeaLevelLoc = 256;

const short unsigned int SeaLevel = LevelSizeY-SeaLevelLoc;

const short unsigned int SandLevel = SeaLevel-350;

const float xscreen=255.0/SCREEN_WIDTH,yscreen=255.0/SCREEN_HEIGHT;

const int SCREEN_BPP = 16;

short int land[SCREEN_WIDTH];
const short unsigned int MAX_GRASS=1024;

clr screenpost[SCREEN_WIDTH][SCREEN_HEIGHT];



struct Pixel{
	unsigned char type;
	short unsigned int clr;
	unsigned char ftype;
}scene[LevelSizeX][LevelSizeY];



enum PixelType : unsigned char
{
   AIR,BG,BG_WOOD,BG_TREEWOOD,BG_ROCK,BG_VEG,BG_MARBLE,BG_HEAVYSAND,BG_DIRT,BG_GRAVEL,CAVEEDGE,ROCK,VEGETATION,SAND,THINGRAVEL,DIRT,CONCRETE,MARBLE,GRAVEL,WOOD,EDGE,SPECIAL,SNOW
};

enum FPixelType : unsigned char
{
   FAIR,FWATERSEED,FWATERSEED1,FWATERSEED2,FWATERSEED3,FWATERSEED4,FWATERSEED5,FWATERSEED6,FWATERSEED7,FWATERSEED8,FWATERSEED9,FWATERFALL,FEDGE,FMETEOR,FSMOKETRAIL,FWATER=245,FSNOW,FSNOWPASS,FROCKDUSTL,FROCKDUSTR,FROCKDUSTEDL,FROCKDUSTEDR,FSANDL,FSANDR,FSANDEDL,FSANDEDR
};



struct Coord2D{
	coord x,y;
};

struct Big2D{
	int x,y;
};


struct RGB{
	Uint8 R,G,B;
};


struct MyMaterials{
	clr FTypeClr[255];
	clr WaterFilter[65536];
	clr Red,Green,Blue,Pink,Yellow,Orange,Cyan,White,Black;
}Materials;