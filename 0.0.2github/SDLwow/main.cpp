/*
* VERY FUNNY GAME :/
*/
#include <string>
#include <iostream>
#include "main.h"
//#include "minimath.h"


HWAVEOUT     hWaveOut;          // Handle to sound card output
WAVEFORMATEX WaveFormat;        // The sound format
WAVEHDR      WaveHeader;        // WAVE header for our sound data
char         Data[BUFFERSIZE];  // Sound data buffer
HANDLE       Done;              // Event Handle that tells us the sound has finished being played.
// This is a real efficient way to put the program to sleep
// while the sound card is processing the sound buffer

float HP=maxb;
bool GAMEOVER=false;
void Message(LPCSTR message)
{
	MessageBox(NULL, (LPCSTR)message, (LPCSTR)"Sound Error", MB_OK | MB_ICONSTOP);
}
float Testx; float Testy; float Testz;
static bool physup=false;
static float xpos,ypos,zpos;//=LevelSizeX/2.0,ypos,zpos=-LevelSizeZ/2.0;
float ratio = (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT;
const coord SCREEN_CENTER_X = SCREEN_WIDTH/2;
const coord SCREEN_CENTER_Y = SCREEN_HEIGHT/2;
static GLboolean should_rotate = GL_TRUE;
bool Debug=false;
GLuint		texture[1];							// Storage For One Texture ( NEW )
typedef struct RGBImageRec {
	GLint sizeX, sizeY;
	unsigned char *data;
} RGBImageRec;


class projectile
{
public:
	bool IsExists;
	float px,py,pz;
	float mx,my,mz;
	GLfloat p,y,r;

	float ystart;

	float dmgrad;
	float accel;
	coord TICK;
	void MakeProj(float x, float y, float z, float vecx, float vecy, float vecz, float dmg,float dmgrad2);
	void DrawProj();
	void Explode();
	void Move();
}proj[10];

class GroundBot
{
public:
	bool Found;
	float randyr;
	int freezetime;
	bool Exists;
	int HPb;
	float x,y,z;
	float yr;
	void Kill();
	bool Spawn();
	void draw(ubyte color);
	void Process();
	bool NewPosition();
	void BreakWalls();
}GroundBot[256];

GLfloat Dist2D(float x, float y, float x2, float y2)
{
	return sqrt(pow(x-x2,2.0f)+pow(y-y2,2.0f));
}
bool OnLevel(float x, float y, float z)
{
	if(x>0 && x<terx)
		if(y>-16 && y<maxy)
			if(z>0 && z<terz)
				return true;
	return false;
}
GLfloat Dist3D(float x, float y, float z, float x2, float y2, float z2)
{
	return sqrt(pow(x-x2,2.0f)+pow(y-y2,2.0f)+pow(z-z2,2.0f));
}

void Rotate(float angle_pitch, float angle_yaw, float angle_roll)
{
	glRotatef(angle_pitch,maxf,0,0);
	glRotatef(angle_yaw,0,maxf,0);
	glRotatef(angle_roll,0,0,maxf);
}

GLfloat realcamera[] = { 0.0f, 0.0f,  0.0f };
GLfloat fincamera[] = {  0.0f, 0.0f,  0.0f };


struct Key
{
	bool press;
	bool Rel;
	bool Hit;
	coord TICK;
}keys[256],LMB,RMB,MMB;
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default


struct Castle
{
	bool Enabled;
	coord l,w,h;
	coord x,y,z;
}Castles[buildnum];


static float xscene,yscene,zscene;
static float xrot,yrot,zrot;

const float M_PI = 3.14159265358979323846;
const float PIover180 = M_PI/180.0;
int LOD=1;
ubyte bLOD=1;

//coord x,y;
struct worldgrid
{
	float h;
	ubyte type;
}scene[EngineLevelSizeX][EngineLevelSizeZ];
struct cubegrid
{
	ubyte type;
}cubescene[terxh][128][terzh];
struct cube
{
	coord x,y,z;
}cubes[testscale+1][chunkx][chunkz];//CUBES_MAX];

int cubenum[chunkx][chunkz];

bool AddCube(coord x, coord y, coord z, ubyte type, bool using_adapted_coords)
{
	int cnum=0;

	if(!using_adapted_coords)
	{x/=2;y/=2;z/=2;}

	if(y>0 && y<512 && x>0 && x<terxh && z>0 && z<terzh && cubescene[x][y][z].type==0)
	{
		int x2=(int)(x/chunksize);
		int z2=(int)(x/chunksize);

		if(cubenum[x2][z2]>testscale || cubescene[x][y][z].type==type)return false;

		cubescene[x][y][z].type=type;

		cnum=cubenum[x2][z2];
		cubes[cnum][x2][z2].x=x;
		cubes[cnum][x2][z2].y=y;
		cubes[cnum][x2][z2].z=z;
		cubenum[x2][z2]++;
		return true;
	}
	else
	{
		return false;
	}
}

struct Objs
{
	coord x,z;
	ubyte type;
	float param;
}objects1[MAX_Objs];
coord objectsnum=0;


struct RGB
{
	unsigned char r,g,b;
}
clr0rgb[EngineLevelSizeX][EngineLevelSizeZ];

int big=1;


bool LimitCoord(coord input, int min, int max)
{
	if(input<min){input=min;return false;}
	if(input>max){input=max;return false;}
	return true;
}

class SoundPlay
{
public:

	void Govbuf(float a, int b)	
	{
		double x;
		int i;             
		float t2=2.0*PI/(double)WaveFormat.nSamplesPerSec,t;
		// ** Make the sound buffer **    
		if(b==4)
			for (i=0; i < BUFFERSIZE/16; i++)
			{        
				// ** Generate the sound wave based on FREQUENCY define
				// ** x will have a range of -1 to +1
				if(i<512)
				{
					t=t2*i;
					x = sin(t*(1000))/0.1f;//sin((t)*(440+a))/4.0+sin((t)*(220+a))/4.0;//tan(t);//sin(t*220);//tan(t*(FREQUENCY)); 
				}

				// ** scale x to a range of 0-maxb (signed char) for 8 bit sound reproduction **
				Data[i] = (char)(127*x+128);
			}	

			if(b==2)
				for (i=0; i < BUFFERSIZE; i++)
				{        
					// ** Generate the sound wave based on FREQUENCY define
					// ** x will have a range of -1 to +1
					t=t2*i;
					x = sin(t*(440+a))/3.0f;//sin((t)*(440+a))/4.0+sin((t)*(220+a))/4.0;//tan(t);//sin(t*220);//tan(t*(FREQUENCY)); 
					// ** scale x to a range of 0-maxb (signed char) for 8 bit sound reproduction **
					Data[i] = (char)(127*x+128);
				}
				if(b==1)
					for (i=0; i < 11025; i++)
					{        
						// ** Generate the sound wave based on FREQUENCY define
						// ** x will have a range of -1 to +1
						t=t2*i;
						x = 0;//sin(t*(441+a));
						x = ((rnd(0,15))*sin(t*(3+a)))/128.0;//sin((t)*(440+a))/4.0+sin((t)*(220+a))/4.0;//tan(t);//sin(t*220);//tan(t*(FREQUENCY)); 
						//         //if(x>4)x=4;if(x<-4)x=-4;
						if(i<512)(x=x*i/512);
						if(i>10769)(x=x*(1.0-(i-10769)/256.0));
						// ** scale x to a range of 0-maxb (signed char) for 8 bit sound reproduction **
						Data[i] = (char)(127*x+128);
					}
					if(b==0)
						for (i=0; i < BUFFERSIZE; i++)
						{
							t=t2*i;
							x=0;//sin(sin(t*220)+tan(t));
							Data[i] = (char)(127*x+128);
						}
						// ** Create the wave header for our sound buffer **
						WaveHeader.lpData=Data;
						WaveHeader.dwBufferLength=BUFFERSIZE;
						WaveHeader.dwFlags=0;
						WaveHeader.dwLoops=0;
	}

	void Prepare()
	{
		//waveOutPause(hWaveOut);
		waveOutReset(hWaveOut);
		waveOutPrepareHeader(hWaveOut,&WaveHeader,sizeof(WaveHeader));
		waveOutWrite(hWaveOut,&WaveHeader,sizeof(WaveHeader));
	}

	void Foot()
	{
		Govbuf(1,1);
		Prepare();
	}
	void Pickup()
	{
		Govbuf(8,1);
		Prepare();
	}
	void Beep()
	{
		Govbuf(0,4);
		Prepare();
	}
	void Explode()
	{
		Govbuf(0.5,1);
		Prepare();
	}
	void Dead()
	{
		Govbuf(0,2);
		Prepare();
	}
};


class Sound
{
public:
	char Click[64];
	Sound::Sound()
	{
	}

	SoundPlay Play;

	void InitWave()
	{
		// ** Initialize the sound format we will request from sound card **    
		WaveFormat.wFormatTag = WAVE_FORMAT_PCM;     // Uncompressed sound format
		WaveFormat.wBitsPerSample = 8;               // Bits per sample per channel
		WaveFormat.nSamplesPerSec = 44100;//11025;           // Sample Per Second
		WaveFormat.nBlockAlign = WaveFormat.nChannels * WaveFormat.wBitsPerSample / 8;
		WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;    
		WaveFormat.cbSize = 0;
		// ** Create our "Sound is Done" event **
		Done = CreateEvent (0, FALSE, FALSE, 0);
	}

}Sound;

bool InWater()
{
	return ypos<WATER_HEIGHT;
}

bool IsGrass(worldgrid a)
{
	if(a.type>=GRASS1 && a.type<=GRASS3)
		return true;
	return false;
}


////////////////////////////////////////////////////////// S O U N D /////////////////////////////////////////////////////


void Smooth(){
	coord a,x,y,x2,z2;
	coord b=4*chunkx*chunkz*chunksize*chunksize;
	for(x=0;x<terx/2;x++)
	{
		for(y=0;y<terz/2;y++)
		{
			x2=x*2,z2=y*2;
			scene[x2][z2].h=(scene[x2+1][z2+1].h+scene[x2+1][z2].h+scene[x2][z2+1].h+scene[x2][z2].h+scene[x2-1][z2-1].h+scene[x2-1][z2].h+scene[x2][z2-1].h+scene[x2+1][z2-1].h+scene[x2-1][z2+1].h)/9.0f;
		}
	}
	for(x=0;x<terx/2;x++)
	{
		for(y=0;y<terz/2;y++)
		{
			x2=x*2+1,z2=y*2+1;
			scene[x2][z2].h=(scene[x2+1][z2+1].h+scene[x2+1][z2].h+scene[x2][z2+1].h+scene[x2][z2].h+scene[x2-1][z2-1].h+scene[x2-1][z2].h+scene[x2][z2-1].h+scene[x2+1][z2-1].h+scene[x2-1][z2+1].h)/9.0f;
		}
	}
}
void MakeItBig()
{
	coord a,x,z;
	coord xsize=chunkx*chunksize;
	coord ysize=chunkz*chunksize;
	for(x=0;x<xsize;x++){
		for(z=0;z<ysize;z++)
		{
			scene[x][z].h=pow(scene[x][z].h,2.0f)/16.0f;
		}
	}
}

struct WeapTextures{
	GLuint A,B,C;
};

struct Weaponary{
	GLubyte Ammo;
	bool Own;
}Weapon[8];

struct Textures{
	WeapTextures Weapon[10];
	GLuint Cube[16];
	GLuint Zombie[6];
	GLuint Clouds[4];
}Textures;

GLuint TextureID,TexA,TexB;


void BuildTexture(GLuint &tex, coord side, bool smooth, GLuint type, coord r_param1=0,coord g_param1=0,coord b_param1=0, coord r_param2=1,coord g_param2=1,coord b_param2=1){
	glEnable(GL_TEXTURE_2D);
	coord sideh=side/2;
	ubyte surface[256*256*3];
	glGenTextures(1, &tex);

#define b2 (side*(y)+x)*3
#define border (x==0 || x==side-1 || y==0 || y==side-1)

	switch(type)
	{
	case CLOUDS:

		break;

	case RAW_NOISE:
		for(int x=0;x<side;x++){for(int y=0;y<side;y++){
			int b=b2;//(side*(y)+x)*3;
			surface[b]=r_param1+rnd(0,r_param2);
			surface[b+1]=g_param1+rnd(0,g_param2);
			surface[b+2]=b_param1+rnd(0,b_param2);
		}}
		break;

	case CONCRETE:
		//R
		for(int x=0;x<side;x++){for(int y=0;y<side;y++){
			int b=b2;//(side*(y)+x)*3;
			surface[b]=100+rnd(0,15);
			surface[b+1]=100+rnd(0,15);
			surface[b+2]=100+rnd(0,15);
			if(border)//(x==0 || x==side-1 || y==0 || y==side-1)
			{surface[b]+=16;surface[b+1]+=16;surface[b+2]+=16;}
		}}

		break;

	case CRATE:
		//R
		for(int x=0;x<side;x++){for(int y=0;y<side;y++){
			int b=b2;//(side*(y)+x)*3;
			surface[b]=x^y; 
			surface[b+1]=surface[b]+80;
			surface[b+2]=surface[b];
			surface[b+2]+=32;
			surface[b]+=96;
			if(border)//(x==0 || x==side-1 || y==0 || y==side-1)
			{surface[b]-=16;surface[b+1]-=16;surface[b+2]-=16;}
		}}
		//B
		break;

	case DEBUG:
		//R
		for(int x=0;x<side;x++){for(int y=0;y<side;y++){
			int b=b2;//(side*(y)+x)*3;
			surface[b]=128;
			surface[b+1]=128;
			surface[b+2]=128;
			if(border)//(x==0 || x==side-1 || y==0 || y==side-1)
			{surface[b]=maxb;surface[b+1]=maxb;surface[b+2]=maxb;}
		}}

		break;


	case CRATEDEBUG:
		//R
		for(int x=0;x<side;x++){for(int y=0;y<side;y++){
			int b=b2;//(side*(y)+x)*3;

			surface[b]=32;

			surface[b+1]=64;

			surface[b+2]=96;
			if(border)//(x==0 || x==side-1 || y==0 || y==side-1)
			{surface[b+2]=128;surface[b+1]=maxb;surface[b]=128;}

		}}
		break;

	}

#undef border
#undef b2

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, side, side, 0, GL_RGB, GL_UNSIGNED_BYTE, surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if(!smooth)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glDisable(GL_TEXTURE_2D);
}


class Vector2D
{
public:

	float scalar;
	float angle;

	Vector2D::Vector2D()
	{
		scalar=0;
		angle=0;
	}

	bool NotNull()
	{
		if(scalar)return true;
	}

	void MakeFromXY(float x,float y)
	{
		if(x!=0)
		angle=atan(y/x);
		scalar=sqrt(y*y+x*x);

		//if(x<0 && y>0)angle=PI-angle;
		//if(x<0 && y<0)angle=PI+angle;

		if(x<0)scalar=-scalar;

	}

	void Normalize()
	{
		if(scalar==0)return;
		if(scalar<0)
		scalar=-1.0f;
		else
		scalar=1.0f;
	}

	float GetX()
	{
		return cos(angle)*scalar;
	}

	float GetY()
	{
		return sin(angle)*scalar;
	}

	Vector2D Rotate(float rotangle)
	{
		Vector2D a;
		a.angle=(angle+rotangle);
		a.scalar=scalar;
		return a;
	}

	//float GetZ2()
	//{
	//	return cos((angle-PI/2))*scalar;
	//}

	void Multiply(Vector2D v2)
	{
		MakeFromXY(GetX()+v2.GetX(),GetY()+v2.GetY());
	}

};


void LoadImageA(){  

	BuildTexture(Textures.Weapon[HANDS].A,64,true, RAW_NOISE, 189, 158, 157, 16,16,16 );
	BuildTexture(Textures.Weapon[SPADE].A,64,true, RAW_NOISE, 80, 60, 45, 16,16,16 );
	BuildTexture(Textures.Weapon[BAZOOKA].A,4,true, RAW_NOISE, 100, 110, 105, 16,16,16 );
	BuildTexture(Textures.Weapon[BAZOOKA].B,8,true, RAW_NOISE, 120, 90, 75, 24,24,24 );
	BuildTexture(Textures.Weapon[BAZOOKA].C,16,false, RAW_NOISE, 90, 110, 80, 16,16,16 );
	BuildTexture(Textures.Weapon[MACHINEGUN].A,4,true, RAW_NOISE, 90, 100, 95, 16,16,16 );
	BuildTexture(Textures.Weapon[MACHINEGUN].B,8,true, RAW_NOISE, 36, 24, 30, 16,16,16 );
	BuildTexture(Textures.Weapon[MACHINEGUN].C,32,true, RAW_NOISE, 100, 110, 105, 16,16,16 );
	//BuildNoiseTexture(Textures.Cube[CONCRETE],32,true, 110, 110, 110, 16,16,12 );

	BuildTexture(Textures.Zombie[0], 8, false, 12);
	BuildTexture(Textures.Cube[CONCRETE], 32, true, CONCRETE);
	BuildTexture(Textures.Cube[CRATE], 32, true, CRATE);
	BuildTexture(Textures.Cube[DEBUG], 8, false, DEBUG);
	BuildTexture(Textures.Cube[CRATEDEBUG], 8, false, CRATEDEBUG);
	BuildTexture(Textures.Clouds[0], 32, false, CLOUDS);

	glDisable(GL_TEXTURE_2D);

}

class Terrain
{
public:
	coord a,x,z;
	float buffer[chunkx*chunksize][chunkz*chunksize];
	void JustMakeItBigger()
	{
		//coord a,x,z;
		coord xsize=chunkx*chunksize;
		coord zsize=chunkz*chunksize;
		for(x=0;x<xsize;x++){
			for(z=0;z<zsize;z++)
			{
				scene[x][z].h+=2;
			}
		}
	}
	void JustMakeItSmaller()
	{

		coord xsize=chunkx*chunksize;
		coord zsize=chunkz*chunksize;
		for(x=0;x<xsize;x++){
			for(z=0;z<zsize;z++)
			{
				scene[x][z].h-=2;
			}
		}
	}
	void ClearAllDaShit()
	{
		for(z=0;z<EngineLevelSizeZ;z++)
		{
			for(x=0;x<EngineLevelSizeX;x++)
			{
				buffer[x][z]=0;
				scene[x][z].h=0;
				scene[x][z].type=0;
				clr0rgb[x][z].r=0;
				clr0rgb[x][z].g=0;
				clr0rgb[x][z].b=0;
			}
		}
		for(x=0;x<chunkx;x++)for(z=0;z<chunkz;z++)cubenum[x][z]=0;

		memset(cubescene,0,terxh*128*terzh*sizeof(cubegrid));

		for(int a=0; a<botsnum; a++)
			GroundBot[a].Kill();
	}

	void SmoothBuffer(coord iteration)
	{
		coord a,x2,z2,b;
		for(a=0;a<iteration;a++)
		{
			for(x=0;x<terx;x++)
			{
				for(z=0;z<terz;z++)
				{
					//coord x2=x,y2=z;
					buffer[x][z]=(buffer[x][z-1]+buffer[x][z+1]+buffer[x][z])/3.0f;
				}
			}
			for(z=0;z<terz;z++)
			{
				for(x=0;x<terx;x++)
				{
					//coord x2=x,y2=z;
					buffer[x][z]=(buffer[x-1][z]+buffer[x+1][z]+buffer[x][z])/3.0f;
				}
			}
		}
	}
	void ApplyBufferToScene(coord times)
	{
		coord x,z;
		if(times==1)
			for(x=0;x<terx;x++)
			{
				for(z=0;z<terz;z++)
				{
					scene[x][z].h+=buffer[x][z];
				}
			}
		else
			for(x=0;x<terx;x++)
			{
				for(z=0;z<terz;z++)
				{
					scene[x][z].h+=times*buffer[x][z];
				}
			}
	}

	float points[256][256];
	float points8[8][8];

	void Interpolate(int i)
	{

		int size=landsize/i;

		for(int pointx=0; pointx<i; pointx++)
			for(int pointz=0; pointz<i; pointz++)
			{
				float ya=points[pointx][pointz];
				float yb=points[pointx+1][pointz];
				int xa=0;
				int xb=size;

				float k=(ya-yb)/(xa-xb);

				for(int n=0; n<size;n++)
				{
					float y=k*n+ya;
					buffer[pointx*size+n][pointz*size]=y;
				}
			}


			for(int x=0; x<landsize; x++)
				for(int pointz=0; pointz<i; pointz++)
				{
					float ya=buffer[x][pointz*size];
					float yb=buffer[x][(pointz+1)*size];
					int xa=0;
					int xb=size;

					float k=(ya-yb)/(xa-xb);

					for(int n=0; n<size;n++)
					{
						float z=k*n+ya;
						buffer[x][pointz*size+n]=z;
					}
				}
	}

	void Points(int i,float height)
	{
		for(int x=0; x<i; x++)
			for(int z=0; z<i; z++)
				points[x][z]=height*(rand()%256)/256.0f;
	}

	void Add(int xoffset=0, int yoffset=0)
	{
		for(int x=0; x<landsize; x++)
			for(int z=0; z<landsize; z++)
				scene[x][z].h+=buffer[x+xoffset][z+yoffset];	
	}

	void Rise(float a)
	{
		for(int x=0; x<landsize; x++)
			for(int z=0; z<landsize; z++)
				scene[x][z].h+=a;//+rndf()/4.0f;	
	}

	void Generate()
	{

		int a=landsize/128.0f/2.0f/2.0f;

		Points(a,384);


		//Interpolate(a);
		//Add();


		//for(int x=0; x<a; x++)
		//	for(int y=0; y<a; y++)
		//		points[x][y]=128*(rand()%256)/256.0f;


		//a*=2;
		//Points(a,256);
		//Interpolate(a);
		//Add();

		a*=2;
		Points(a,128);

		//for(int m=0;m<2;m++)
		//{
		//	x1=rnd(0,a);
		//	z1=rnd(0,a);
		//	for(int n=0;n<512;n++)
		//	{
		//		x1+=rnd(-1,1);
		//		z1+=rnd(-1,1);
		//	points[x1][z1]=-128.0f;
		//	}
		//}
		//
		Interpolate(a);
		Add();

		a*=2;
		Points(a,64);


		for(int m=0;m<4;m++)
		{

			int x1=rnd(0,a);
			int z1=rnd(0,a);
			for(int n=0;n<64;n++)
			{
				x1+=rnd(-1,1);
				z1+=rnd(-1,1);
				points[x1][z1]+=64.0f;
			}
		}


		for(int m=0;m<4;m++)
		{

			int x1=rnd(0,a);
			int z1=rnd(0,a);
			for(int n=0;n<64;n++)
			{
				x1+=rnd(-1,1);
				z1+=rnd(-1,1);
				points[x1][z1]-=64.0f;
			}
		}

		Interpolate(a);
		Add(1,0);


		a*=2;
		Points(a,32);
		Interpolate(a);
		Add(0,1);

		a*=2;
		Points(a,16);
		Interpolate(a);
		Add(-1,0);

		a*=2;
		Points(a,8);
		Interpolate(a);
		Add(0,-1);

		a*=2;
		Points(a,4);
		Interpolate(a);
		Add(1,1);

		a*=2;
		Points(a,2);
		Interpolate(a);
		Add(-1,1);

		a*=2;
		Points(a,0.5);
		Interpolate(a);
		Add(1,-1);

		Rise(rnd(-96,-64));


	}
	void PlaceCrates()
	{
		for(coord a3=0;a3<cratenum;a3++)
		{
			coord ranx=rand()%terx,ranz=rand()%terz;
			coord h = scene[ranx][ranz].h;
			if(h>WATER_HEIGHT)
				AddCube(ranx,h+2,ranz,CRATE,false);
		}
	}

	void BuildCastles()
	{
		for(coord a=0;a<buildnum;a++){

			coord size=(int)((16+rand()%4*4)/2)*2;

			coord x1=(int)(
				(rand()%(terx-2*size)+size)/2)*2;

			float y1=abs(scene[x1+size/2][z1+size/2].h);

			if(y1<10.0f){Castles[a].Enabled=false;}
			else{Castles[a].Enabled=true;

			coord floors=(rnd(1,5));

			coord texture=rand()%32;
			int clrg=rand()%4*16-32,clrb=rand()%4*16-32;
			coord clradd=32+rand()%4*32;


			coord floorheight=(5);//rand()%6+8;

			coord flh=floorheight*2;

			Castles[a].x=x1+size/2;
			Castles[a].z=z1+size/2;

			Castles[a].h=floors*flh;
			Castles[a].y=y1+Castles[a].h/2;

			Castles[a].w=size;
			Castles[a].l=size;


			for(x=x1;x<x1+size;x++)
			{
				for(z=z1;z<z1+size;z++)
				{
					coord y2 = (int)(y1/2.0f);
					scene[x][z].h = y2*2+maxf ;

				}
			}

			for(x=x1;x<x1+size;x++)
			{
				coord xl=x-x1;
				for(int z=z1;z<z1+size;z++)
				{
					coord yl=z-z1;
					scene[x][z].type=CONCRETE;
					if(xl!=0 && yl!=0 && xl!=size && yl!=size)
					{
						clr0rgb[x][z].r=((xl*xl+yl*yl)*texture)%256/4+clradd;
						clr0rgb[x][z].g=clr0rgb[x][z].r+clrg;
						clr0rgb[x][z].b=clr0rgb[x][z].r+clrb;
					}
					else
					{
						clr0rgb[x][z].r=clradd;
						clr0rgb[x][z].g=clr0rgb[x][z].r;
						clr0rgb[x][z].b=clr0rgb[x][z].r;
					}
				}
			}

			coord xd=0,zd=0,xd2=0,zd2=0;

			xd=x1+rand()%(size-6)-3;

			zd=z1+rand()%(size-6)-3;


			for(coord floor=0;floor<floors;floor++)
			{

				for(coord h2=0;h2<flh;h2+=2)
				{

					for(x=x1;x<x1+size;x+=2)
					{
						if(floor==0 && ((x<xd-2 || x>xd+2)) || floor>0)	
						{AddCube(x,y1+h2+floor*flh,z1,CONCRETE,false);
						AddCube(x,y1+h2+floor*flh,z1+size,CONCRETE,false);}

					}
					for(coord z=z1;z<z1+size;z+=2)
					{
						if(floor==0 && ((z<zd-2 || z>zd+2)) || floor>0)	
						{AddCube(x1,y1+h2+floor*flh,z,CONCRETE,false);
						AddCube(x1+size,y1+h2+floor*flh,z,CONCRETE,false);}
					}
				}

				//ÏÎË ÍÀ ÄÐÓÃÈÕ ÝÒÀÆÀÕ

				if(floor>0)
					for(x=x1;x<x1+size;x+=2)
					{
						for(int z=z1;z<z1+size;z+=2)
						{
							AddCube(x,y1+floor*flh,z,CONCRETE,false);
						}
					}

			}

			//ÑÓÍÄÓÊÈ

			coord crts=rnd(1,4);
			for(coord r=0;r<crts;r++)
				AddCube(x1+rand()%size,y1+(rand()%floors)*flh+2,z1+rand()%size,CRATE,false);
		} 
}
	}

	void Fractal()
	{
		float GlobalAdd;
		TICKWIN=GetTickCount();
		srand(TICKWIN);
		//GlobalAdd=rnd(0,3)*16+rnd(0,15);
		//if(rnd(0,2)==0)
		//	GlobalAdd=0;
		//GlobalAdd+=ADD_HEIGHT;

		Generate();

		float k=EdgeDampingK;

		int dist;
		coord max=terxh-EdgeDampingMaxDist;
		float finaddx=0.0f,finaddy=0.0f;
		for(x=0;x<terx;x++)
		{
			dist=abs((int)x-(int)terxh);
			if(dist>max)
			{
				finaddx=((float)dist-(float)max)/k;
			}
			else
			{
				finaddx=0;
			}
			for(z=0;z<terz;z++)
			{

				dist=abs((int)z-(int)terzh);
				if(dist>max)
				{
					finaddy=((float)dist-(float)max)/k;
				}
				else
				{
					finaddy=0;
				}

				scene[x][z].h-=17.0f;//13.0f;
				if(scene[x][z].h<6.0f){scene[x][z].h=scene[x][z].h/1.5f-2.0f+rndf();}

				scene[x][z].h+=finaddx+finaddy;
				if(!rnd(0,7) && objectsnum<MAX_Objs && scene[x][z].h>36.0f && scene[x][z].h<80.0f)// && scene[x][z]<60.0f && scene[x][z]>30.0f)
				{
					scene[x][z].type=GRASS1+rnd(0,2);
				}

				if(scene[x][z].h<0.0f){scene[x][z].h=0+rndf();}

			}
		}



		// / C A S T L E S ; 
		//for(coord b=0;b<buildnum;b++){
			BuildCastles();
		//}

		PlaceCrates();
	}
}Terrain;


void  BuildScene(bool rebuildgeo, bool rebuildclr){

	objectsnum=0;
	if(rebuildgeo)
	{
		Terrain.ClearAllDaShit();Terrain.Fractal();
	}

	for(coord n=0;n<botsnum;n++)
		GroundBot[n].Spawn();

	if(Cheat)
	{
		Weapon[SPADE].Own=true;
		Weapon[BAZOOKA].Own=true;
		Weapon[MACHINEGUN].Own=true;
		Weapon[SPADE].Ammo=1;
		Weapon[BAZOOKA].Ammo=8;
		Weapon[MACHINEGUN].Ammo=64;
	}
	Weapon[HANDS].Own=true;

	Weapon[HANDS].Ammo=0;	

	Weapon[BLOCKS].Own=true;

	Weapon[BLOCKS].Ammo=0;		

	if(rebuildclr)
		for(int x=0;x<EngineLevelSizeX;x++){for(int z=0;z<EngineLevelSizeZ;z++){
			float h=scene[x][z].h;
			coord num=x*z+z;
			if(scene[x][z].type!=CONCRETE)
			{
				if(!Debug)
				{
					coord rg=rnd(0,15);
					if(x==0 || z==0 || x>EngineLevelSizeX || z>EngineLevelSizeZ) 
					{
						clr0rgb[x][z].r=0;
						clr0rgb[x][z].g=clr0rgb[x][z].r/1.8;
						clr0rgb[x][z].b=clr0rgb[x][z].r/3.2;
					}
					else
					{
						////	$	WILD WEST	$
						/*SHORE*/				if(h<32.0f)
						{
							clr0rgb[x][z].r=rnd(175,180)+h;///2.0f;
							clr0rgb[x][z].g=rnd(145,155)+h;///2.0f;
							clr0rgb[x][z].b=rnd(105,110)+h;///2.0f;

							if(!IsGrass(scene[x][z]))scene[x][z].type=SAND;
						}
						/*GRASS*/				if(h>=32.0f && h<128.0f)
						{
							if(scene[x+1][z].h<80.0f && scene[x-1][z].h<80.0f && scene[x][z+1].h<80.0f && scene[x][z-1].h<80.0f && scene[x-2][z].h<80.0f && scene[x][z-2].h<80.0f)
							{
								clr0rgb[x][z].r=rand()%8+80+h-12.0f+0.5*(h-12.0f);
								clr0rgb[x][z].g=rand()%8+114+h-12.0f;
								clr0rgb[x][z].b=rand()%4+50+h-12.0f;
							
								if(!IsGrass(scene[x][z]))scene[x][z].type=GRASS;
							}
							/*ROCK1*/					else
							{
								clr0rgb[x][z].r=rand()%16+100;
								clr0rgb[x][z].g=rand()%16+104;
								clr0rgb[x][z].b=rand()%8+110;
							}
						}
						/*ROCK2*/				if(h>=128.0f && h<192.0f)
						{
							clr0rgb[x][z].r=rand()%6+240;
							clr0rgb[x][z].g=rand()%6+240;
							clr0rgb[x][z].b=rand()%8+245;
						}
						/*SNOW*/				if(h>=192.0f)
						{
							clr0rgb[x][z].r=rand()%6+240;
							clr0rgb[x][z].g=rand()%6+240;
							clr0rgb[x][z].b=rand()%8+245;
						}
						coord a;
					}
				}	
				else	
				{
					clr0rgb[x][z].r=h*16.0;
					clr0rgb[x][z].g=h*16.0;
					clr0rgb[x][z].b=h*16.0;
				}
			}
		}	
		}
}

float randclr()
{
	srand(time(NULL)*64);
	float a=rndf();
	return a;
}


// A	B	C	D	E	F	G	H	I	J	K	L	M	N	O	P	Q	R	S	T	U	V	W	X	Y	Z






class InventoryAndHUDAndSoOn{
public:

	int menuselected;

	ubyte slot;
	InventoryAndHUDAndSoOn::InventoryAndHUDAndSoOn()
	{
		menuselected=0;
		slot=1;
	}

	void Line2D(coord x, coord y, coord x2, coord y2, coord width)
	{			
		GLfloat v[] = {x,y};
		GLfloat v2[] = {x2,y2};

		glLineWidth(width);

		glBegin(GL_LINES);
		glVertex2fv(v);
		glVertex2fv(v2);
		glEnd();
	}
	//         ASCII !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! http://www.asciitable.com/index/asciifull.gif
	enum symboltype
	{ 
		number=48,  //48-57
		letter=65,  //65-90
		symbol1=33, //33-47
		symbol2=58, //58-64
		symbol3=91, //91-96
		number2=97	//97-122
	};

	bool GetFontQuad(char st, coord x, coord y)
	{
		if(st==number){return font_number[x][y];}else
			if(st==letter){return font_letter[x][y];}

	}

	void Symbol(char chr, coord xpos, coord ypos, coord size, bool Italics)
	{			
		//GLfloat v[] = {x,y};
		char chrb;

		if(chr>=symbol1 && chr<number)
			chrb=symbol1;
		else
			if(chr>=number && chr<symbol2)
				chrb=number;
			else
				if(chr>=symbol2 && chr<letter)
					chrb=symbol2;
				else
					if(chr>=letter && chr<symbol3)
						chrb=letter;
					else
						if(chr>=symbol3 && chr<number2)
							chrb=symbol3;
						else
							if(chr>=number2 && chr<=122)
							{chr=chr-number2+number;chrb=number;}
							else
								return;

		char chrc=chr-chrb;
		chrc*=4;

		for(coord x=0;x<4;x++)
		{
			coord xs=x*size;
			coord x2=xpos+xs;
			//if(Italics)x2+=y;

			for(coord y=0;y<6;y++)
			{
				coord ys=y*size;
				coord y2=ypos+ys;
				if(Italics)y2+=x;

				if(GetFontQuad(chrb, y,chrc+x))
				{
					glVertex2f(x2,y2);
					glVertex2f(x2-size,y2);
					glVertex2f(x2-size,y2+size);
					glVertex2f(x2,y2+size);
				}
			}
		}

	}


	void Line(std::string st, coord xpos, coord ypos, coord size, bool Italics)
	{
		coord xx=0, space;
		space=size*(4+2);
		for(coord a=0;a<st.length();a++)
		{
			Symbol(st[a],xpos+xx,ypos,size,Italics);
			xx+=space;
		}
	}

	void SwitchMenu()
	{
		if(GAME.mode==GAME.MODES.menu)
			return;
		//Menu=true;
		Menu=!Menu;
		Stop=Menu;
		//else
		//	Stop=false;
	}


	void StartGame()
	{
		Menu=0;
		Stop=0;
		GAME.mode=GAME.MODES.game;
	}


	void Inva()
	{
		static ubyte r[7],g[7],b[7];
		r[0]=0;
		g[0]=0;
		b[0]=0;
		r[HANDS]=160;
		g[HANDS]=130;
		b[HANDS]=110;
		r[BLOCKS]=110;
		g[BLOCKS]=100;
		b[BLOCKS]=80;
		r[SPADE]=80;
		g[SPADE]=60;
		b[SPADE]=20;
		r[MACHINEGUN]=140;
		g[MACHINEGUN]=120;
		b[MACHINEGUN]=170;
		r[BAZOOKA]=90;
		g[BAZOOKA]=150;
		b[BAZOOKA]=70;
		r[6]=0;
		g[6]=0;
		b[6]=0;

		for(coord a=1;a<6;a++)
		{
			bool HasIt=(slot==a && Weapon[a].Own);

			glColor3ub(r[a]*Weapon[a].Own,g[a]*Weapon[a].Own,b[a]*Weapon[a].Own);
			glBegin(GL_QUADS);

			Symbol('0'+a,HSCREEN_WIDTH-224+a*64,SCREEN_HEIGHT-192,4,HasIt);

			glEnd();
		}
	}


	void DrawMenu()
	{

		glLoadIdentity();
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1 );

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);

		glColor4f(0.2f,0.2f,0.2f,0.4f);
		Line2D(HSCREEN_WIDTH-208,HSCREEN_HEIGHT-16,HSCREEN_WIDTH+304,HSCREEN_HEIGHT-16,16.0f);

		glColor4f(maxf,maxf,0.5f,maxf);
		glBegin(GL_QUADS);

		Line(gamename,HSCREEN_WIDTH-192,HSCREEN_HEIGHT-64,8,true);

		glColor4f(maxf,maxf,0.5f,0.5f);

		if(menuselected<0)menuselected=0;
		if(menuselected>3)menuselected=3;

		if(keys[VK_DOWN].Hit){menuselected++;Sound.Play.Beep();}
		if(keys[VK_UP].Hit){menuselected--;Sound.Play.Beep();}

		if(keys[VK_RETURN].Hit){

			switch(menuselected){
			case 0://START
				StartGame();
				break;
			case 1://SETTINGS
				break;
			case 2://NEW MAP
				BuildScene(true,true);
				break;
			case 3://EXIT
				done=true;
				break;
			}

			Sound.Play.Explode();
		}

		const std::string Menus[4]={
			"START",
			"SETTINGS",
			"NEW MAP",
			"EXIT"};

			const GLfloat menuclr[] = {0.5f,maxf,0.5f};
			const int size=5;

			for(coord slot=0; slot<4;slot++)
			{
				bool sel=(slot==menuselected);
				glColor4f(menuclr[0],menuclr[1],menuclr[2],sel/2.0f+0.5f);
				Line(Menus[slot],HSCREEN_WIDTH-128,HSCREEN_HEIGHT+7*size*slot,size,sel);
			}
			glEnd();
			glDisable(GL_BLEND);
	}

	void DrawHUD()
	{
		if(Weapon[slot].Ammo<0 || Weapon[slot].Ammo>MaxAmmo[slot]+1)
			Weapon[slot].Own=false;
		if(Weapon[slot].Own==false)
			slot-=1;
		glLoadIdentity();
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1 );

		glDisable( GL_DEPTH_TEST);

		glDisable(GL_TEXTURE_2D);

		//glColor4f(maxf,maxf,maxf,maxf);

		glEnable(GL_BLEND);

		//HEALTH

		glColor4f(0.7f,0.0f,0.0f,0.5f);
		Line2D(96+HP,SCREEN_HEIGHT-288,256+95,SCREEN_HEIGHT-288,16.0f);

		glColor4f(0.0f,maxf,0.0f,0.3f);
		Line2D(96,SCREEN_HEIGHT-288,HP+96,SCREEN_HEIGHT-288,16.0f);

		//AMMO

		glColor4f(0.6f,0.4f,0.0f,0.4f);
		Line2D(96,SCREEN_HEIGHT-236,96+Weapon[Inv.slot].Ammo*256/(MaxAmmo[Inv.slot]+1),SCREEN_HEIGHT-236,16.0f);

		glBegin(GL_QUADS);
		glColor4f(maxf,maxf,maxf,maxf);
		Line("HEALTH",104,SCREEN_HEIGHT-313,2.0f,false);
		if(Weapon[Inv.slot].Ammo>0)Line("AMMO",104,SCREEN_HEIGHT-261,2.0f,false);
		glEnd();

		glColor4f(0.2f,0.2f,0.2f,0.2f);

		glBegin(GL_QUADS);
		glVertex2f(HSCREEN_WIDTH-188,SCREEN_HEIGHT-164); // top left
		glVertex2f(HSCREEN_WIDTH+148,SCREEN_HEIGHT-164); // bottom left
		glVertex2f(HSCREEN_WIDTH+148,SCREEN_HEIGHT-196); // bottom right
		glVertex2f(HSCREEN_WIDTH-188,SCREEN_HEIGHT-196); // top right
		glEnd();

		glDisable(GL_BLEND);

		glColor3f(maxf,maxf,0.5f);
		glBegin(GL_QUADS);

		glEnd();

		Inva();

		glEnable( GL_DEPTH_TEST);
	}
} Inv;

								
class Draw
{
public:
	bool CubeIsInit;
	bool DrawCube;
	ubyte drawtype;

	void Line(GLfloat x,GLfloat y,GLfloat z,GLfloat x2,GLfloat y2,GLfloat z2, ubyte r,ubyte g,ubyte b, ubyte a, GLfloat width, bool machinegun)
	{
		GLfloat v[] = {x,y,z};
		GLfloat v2[] = {x2,y2,z2};
		float R=r/256.0f,G=g/256.0f,B=b/256.0f,A=a/256.0f;
		if(machinegun)
		{
			glLineStipple(16,0x1C47);
			if(rnd(0,3)==0)
			{
				glLineWidth(width);
				glEnable(GL_BLEND);
				glColor4f(R, G, B, A);
				glBegin(GL_LINES);

				glVertex3fv(v2);
				glVertex3fv(v);
				glEnd();
				glDisable(GL_BLEND);
				glLineStipple(1,0);
				glColor3f(maxf, maxf, maxf);
			}
		}
	}


	void CubeFaces(GLfloat v0a[],GLfloat v1a[],GLfloat v2a[],GLfloat v3a[],GLfloat v0b[],GLfloat v1b[],GLfloat v2b[],GLfloat v3b[])
	{
		const float one=maxf;
		const float zer=0.0f;

		glTexCoord2f(zer, zer);
		glVertex3fv (v1b);
		glTexCoord2f(one, zer);
		glVertex3fv (v2b);
		glTexCoord2f(one, one);
		glVertex3fv (v3b);
		glTexCoord2f(zer, one);
		glVertex3fv (v0b);

		glTexCoord2f(zer, zer);
		glVertex3fv (v1a);	glTexCoord2f(one, zer);
		glVertex3fv (v0a);	glTexCoord2f(one, one);
		glVertex3fv (v3a);	glTexCoord2f(zer, one);
		glVertex3fv (v2a);

		glTexCoord2f(zer, zer);
		glVertex3fv (v2b);	glTexCoord2f(one, zer);
		glVertex3fv (v2a);	glTexCoord2f(one, one);
		glVertex3fv (v3a);	glTexCoord2f(zer, one);
		glVertex3fv (v3b);

		glTexCoord2f(zer, zer);
		glVertex3fv (v1b);	glTexCoord2f(one, zer);
		glVertex3fv (v0b);	glTexCoord2f(one, one);
		glVertex3fv (v0a);	glTexCoord2f(zer, one);
		glVertex3fv (v1a);

		glTexCoord2f(zer, zer);
		glVertex3fv (v2a);	glTexCoord2f(one, zer);
		glVertex3fv (v2b);	glTexCoord2f(one, one);
		glVertex3fv (v1b);	glTexCoord2f(zer, one);
		glVertex3fv (v1a);

		glTexCoord2f(zer, zer);
		glVertex3fv (v3a);	glTexCoord2f(one, zer);
		glVertex3fv (v0a);	glTexCoord2f(one, one);
		glVertex3fv (v0b);	glTexCoord2f(zer, one);
		glVertex3fv (v3b);		
	}


	void ShortCubeFaces(GLfloat W, GLfloat H, GLfloat L)
	{

		GLfloat v3a[] = {-W,-H,-L};
		GLfloat v2a[] = {-W,H,-L};
		GLfloat v1a[] = {W,H,-L};
		GLfloat v0a[] = {W,-H,-L};
		GLfloat v3b[] = {-W,-H,L};
		GLfloat v2b[] = {-W,H,L};
		GLfloat v1b[] = {W,H,L};
		GLfloat v0b[] = {W,-H,L};

		CubeFaces(v0a,v1a,v2a,v3a,v0b,v1b,v2b,v3b);
	}

	void Projectile(GLfloat x,GLfloat y,GLfloat z, GLuint TEXTURE)
	{

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TEXTURE);
		glColor4f(maxf, maxf, maxf, maxf);
		float ang=rand()%1024/16.0;
		glPushMatrix();
		glTranslatef(x,y,z);

		ShortCubeFaces(projectile_half_side,projectile_half_side,projectile_half_side);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

	}

	void Cube(GLfloat x,GLfloat y,GLfloat z, GLfloat pitch, GLfloat yaw, GLfloat roll, GLfloat h, GLubyte type)
	{
		//GLfloat h=cubeside/2;
		GLfloat v3a[] = {-h+x,-h+y,-h+z};
		GLfloat v2a[] = {-h+x,h+y,-h+z};
		GLfloat v1a[] = {h+x,h+y,-h+z};
		GLfloat v0a[] = {h+x,-h+y,-h+z};
		GLfloat v3b[] = {-h+x,-h+y,h+z};
		GLfloat v2b[] = {-h+x,h+y,h+z};
		GLfloat v1b[] = {h+x,h+y,h+z};
		GLfloat v0b[] = {h+x,-h+y,h+z};

		if(type!=drawtype)
		{
			//glEnd();

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Textures.Cube[type]);
			drawtype=type;

			glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glColor4f(1.0, 1.0, 1.0, 1.0);

			if(type==DEBUG || type==CRATEDEBUG)
			{
				glEnable(GL_BLEND);
				glDisable(GL_CULL_FACE);
				glColor4f(1.0, 1.0, 1.0, 0.25);
			}

		}

		glBegin (GL_QUADS);
		CubeFaces(v0a,v1a,v2a,v3a,v0b,v1b,v2b,v3b);
		glEnd();

	}


	void Box(GLfloat x,GLfloat y,GLfloat z, GLfloat W, GLfloat H, GLfloat L, GLfloat pitch, GLfloat yaw, GLfloat roll, GLuint TEXTURE)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TEXTURE);

		glPushMatrix();

		glTranslatef(x,y,z);
		Rotate(pitch,yaw,roll);

		glBegin (GL_QUADS);
		ShortCubeFaces(W/2,H/2,L/2);
		glEnd();

		glPopMatrix();
	}

	void Hands()
	{	
		glDisable(GL_DEPTH_TEST);
		if(InWater())
			glColor4f(0.7f, 0.7f, maxf, maxf);
		switch(Inv.slot)
		{
		case HANDS:

			if(LMB.press && RMB.press)
			{
				Box(maxf,-2.75f,-0.75f,maxf,maxf,4.0f,60.0f+rndf(),-0.0f+rndf(),-0+rndf(),Textures.Weapon[HANDS].A);
				Box(-maxf,-2.75f,-0.75f,maxf,maxf,4.0f,60.0f+rndf(),-0.0f+rndf(),-0+rndf(),Textures.Weapon[HANDS].A);
			}
			else
			{
				if(LMB.press)
					Box(-1.5f,-2.75f,-1.5f,maxf,maxf,4.0f,45.0f+rndf(),-0.0f+rndf(),-0+rndf(),Textures.Weapon[HANDS].A);
				else
					Box(-2.0f,-3.5f,-1.5f,maxf,maxf,4.0f,45.0f,0.0f,0.0f,Textures.Weapon[HANDS].A);
				if(RMB.press)		
					Box(1.5f,-2.75f,-1.5f,maxf,maxf,4.0f,45.0f+rndf(),-0.0f+rndf(),-0+rndf(),Textures.Weapon[HANDS].A);
				else
					Box(2.0f,-3.5f,-1.5f,maxf,maxf,4.0f,45.0f,0.0f,0.0f,Textures.Weapon[HANDS].A);
			}
			break;
		case SPADE:
			if(LMB.press)
			{
				Box(maxf,-2.25f,-1.5f,-0.75f,0.75f,4.0f,45.0f+rndf(),0.0f+rndf(),0.0f+rndf(),Textures.Weapon[SPADE].A);
			}
			else
			{
				if(RMB.press)
				{
					Box(maxf,-2.25f,1.5f,-0.75f,0.75f,4.0f,20.0f+rndf(),0.0f+rndf(),0.0f+rndf(),Textures.Weapon[SPADE].A);
				}
				else
				{
					Box(1.25f,-1.75f,-1.75f,-0.75f,0.75f,4.0f,60.0f,-0.0f,0.0f,Textures.Weapon[SPADE].A);
				}
			}
			break;

		case BLOCKS:
			if(LMB.press)
			{
				Box(-1.5f,-2.75f,-2.5f,maxf,maxf,4.0f,45.0f,-0.0f,-0,Textures.Weapon[HANDS].A);
				Box(1.5f,-2.75f,-2.5f,maxf,maxf,4.0f,45.0f,-0.0f,-0,Textures.Weapon[HANDS].A);
			}
			else	
				if(RMB.press)
				{
					Box(1.5f,-2.75f,-0.5f,maxf,maxf,4.0f,45.0f,-0.0f,-0,Textures.Weapon[HANDS].A);
					Box(-1.5f,-2.75f,-0.5f,maxf,maxf,4.0f,45.0f,-0.0f,-0,Textures.Weapon[HANDS].A);
				}
				else
				{
					Box(-maxf,-3.5f,-1.5f,maxf,maxf,4.0f,45.0f,0.0f,0.0f,Textures.Weapon[HANDS].A);
					Box(maxf,-3.5f,-1.5f,maxf,maxf,4.0f,45.0f,0.0f,0.0f,Textures.Weapon[HANDS].A);
				}
				break;

		case MACHINEGUN:
			if(LMB.press)
			{
				float rnd1=rndf()/4.0f, rnd2=rndf()*2.0f;

				glEnable(GL_DEPTH_TEST);
				Line(0.25f,-0.25f,-0.5f, 0.15f+rnd(-4,3),-0.25f+rnd(-4,3),-0.5f-256.0f+rnd1, rand()%30+200,rand()%50+206,150,100, rand()%16/8.0f, true); 
				Line(0.25f,-0.25f,-0.5f, 0.15f+rnd(-4,3),-0.25f+rnd(-4,3),-0.5f-256.0f+rnd1, rand()%30+200,rand()%50+206,150,100, rand()%16/8.0f, true);
				glDisable(GL_DEPTH_TEST);
				coord b=rnd(24,39);
				for(int a=0;a<b;a++)
					Line(0.25f,-0.25f,-0.75f+rnd1, 0.25f+rand()%8/32.0f-0.125,-0.25f+rand()%8/32.0f-0.125f,-1.25f+rnd1, rand()%26+230,rand()%30+160,120,120, 2.0f+rand()%8/2.0f, true); 

				Box(2.0f,-3.25f,-5.0f+rnd1, 0.5f,maxf,0.25f, 0.0f+rnd2,0.0f,0.0f, Textures.Weapon[MACHINEGUN].B);
				Box(2.0f,-2.0f,-4.0f+rnd1, 0.75f,1.5f,3.0f, 0.0f+rnd2,0.0f,0.0f, Textures.Weapon[MACHINEGUN].C);
				Box(2.0f,-maxf,-5.0f+rnd1, 0.25f,0.5f,0.15f, 0.0f+rnd2,0.0f,0.0f, Textures.Weapon[MACHINEGUN].A);
				Box(2.0f,-1.5f,-2.0f+rnd1, 0.5f,0.75f,0.75f, 45.0f+rnd2,0.0f,0.0f, Textures.Weapon[MACHINEGUN].B);
			}
			else
			{
				Box(2.0f,-3.25f,-5.0f, 0.5f,maxf,0.25f, 0.0f,0.0f,0.0f, Textures.Weapon[MACHINEGUN].B);
				Box(2.0f,-2.0f,-4.0f, 0.75f,1.5f,3.0f, 0.0f,0.0f,0.0f, Textures.Weapon[MACHINEGUN].C);
				Box(2.0f,-maxf,-5.0f, 0.25f,0.5f,0.15f, 0.0f,0.0f,0.0f, Textures.Weapon[MACHINEGUN].A);
				Box(2.0f,-1.5f,-2.0f, 0.5f,0.75f,0.75f, 45.0f,0.0f,0.0f, Textures.Weapon[MACHINEGUN].B);
			}
			break;
		case SHOTGUN:
			break;
		case BAZOOKA:
			if(LMB.press)
			{
				float rnd1=0.5f,rnd2=16.0f;//float rnd1=rand()%16/64.0f, rnd2=//rand()%16/8.0f;
				Box(2.0f,-3.75f,-5.0f+rnd1, 1.5f,1.5f,0.5f, 0.0f+rnd2,0.0f,0.0f, Textures.Weapon[BAZOOKA].B);
				Box(2.0f,-2.0f,-4.0f+rnd1, 2.0f,2.0f,4.0f, 0.0f+rnd2,0.0f,0.0f, Textures.Weapon[BAZOOKA].C);
				Box(2.0f,-0.75f,-5.0f+rnd1, 0.5f,0.5f,0.15f, 0.0f+rnd2,0.0f,0.0f, Textures.Weapon[BAZOOKA].A);
			}
			else
			{
				Box(2.0f,-3.75f,-5.0f, 1.5f,1.5f,0.5f, 0.0f,0.0f,0.0f, Textures.Weapon[BAZOOKA].B);
				Box(2.0f,-2.0f,-4.0f, 2.0f,2.0f,4.0f, 0.0f,0.0f,0.0f, Textures.Weapon[BAZOOKA].C);
				Box(2.0f,-0.75f,-5.0f, 0.5f,0.5f,0.15f, 0.0f,0.0f,0.0f, Textures.Weapon[BAZOOKA].A);
			}
			break;
		}
		glColor4f(maxf, maxf, maxf, maxf);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glDisable(GL_BLEND);
		glDisable(GL_FOG);
		glLoadIdentity( );
		glEnable(GL_DEPTH_TEST);
	}


	void Clouds()
	{
		glDisable(GL_FOG);
		float b=64.0f;
		float h=256.0f;
		glColor4f(0.8,0.8,0.8,1.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int size=3584;
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Textures.Weapon[1].A);		

		for(int a=0;a<4;a++)
		{
			int x=((a==3)||(a==2))*size;
			int z=((a==1)||(a==2))*size;

			GLfloat v0b[] = { -size+x+landsizeh, h, -size+z+landsizeh };
			GLfloat v1b[] = { 0+x+landsizeh, h, -size+z+landsizeh };
			GLfloat v2b[] = { 0+x+landsizeh, h, z+landsizeh };
			GLfloat v3b[] = { -size+x+landsizeh, h, z+landsizeh };

			glBegin( GL_QUADS );
			glTexCoord2f(b,0.0);	
			glVertex3fv( v0b );
			glTexCoord2f(b,b);
			glVertex3fv( v1b );
			glTexCoord2f(0.0,b);
			glVertex3fv( v2b );
			glTexCoord2f(0.0,0.0);
			glVertex3fv( v3b );
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);

		glDisable(GL_BLEND);	
		glPopMatrix();
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glEnable(GL_FOG);
	}

	void Water(bool GroundBase)
	{
		float h;
		if(GroundBase)
		{
			h=-0.25f;
			glColor4fv(groundclr);
			glDisable(GL_BLEND);
		}
		else
		{
			h=WATER_HEIGHT;
			glColor4fv(waterclr);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		int size=6584;
		glPushMatrix();

		for(int a=0;a<4;a++)
		{

			int x=((a==3)||(a==2))*size;
			int z=((a==1)||(a==2))*size;

			GLfloat v0b[] = { -size+x+landsizeh, h, -size+z+landsizeh };
			GLfloat v1b[] = { x+landsizeh, h, -size+z+landsizeh };
			GLfloat v2b[] = { x+landsizeh, h, z+landsizeh };
			GLfloat v3b[] = { -size+x+landsizeh, h, z+landsizeh };

			glBegin( GL_TRIANGLES );

			glVertex3fv( v0b );

			glVertex3fv( v1b );

			glVertex3fv( v2b );

			glVertex3fv( v0b );

			glVertex3fv( v2b );

			glVertex3fv( v3b );
			glEnd();

		}

		glDisable(GL_BLEND);	
		glPopMatrix();
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}


	void Sky()
	{
		glDisable( GL_DEPTH_TEST );
		glShadeModel(GL_SMOOTH);
		GLfloat v0b[] = { -2.0f, -maxf, -maxf };
		GLfloat v1b[] = {  2.0f, -maxf, -maxf };
		GLfloat v2b[] = {  2.0f,  maxf, -maxf };
		GLfloat v3b[] = { -2.0f,  maxf, -maxf };

		glBegin( GL_TRIANGLES );
		glColor4ubv( skyclrdown );
		glVertex3fv( v0b );
		glColor4ubv( skyclrdown );
		glVertex3fv( v1b );
		glColor4ubv( skyclrup );
		glVertex3fv( v2b );
		glColor4ubv( skyclrdown );
		glVertex3fv( v0b );
		glColor4ubv( skyclrup );
		glVertex3fv( v2b );
		glColor4ubv( skyclrup );
		glVertex3fv( v3b );
		glEnd();
		glEnable( GL_DEPTH_TEST );
	}

	void SeaFilter()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable( GL_DEPTH_TEST );
		glShadeModel(GL_SMOOTH);
		GLfloat v0b[] = { -2.0f, -maxf, -maxf };
		GLfloat v1b[] = {  2.0f, -maxf, -maxf };
		GLfloat v2b[] = {  2.0f,  maxf, -maxf };
		GLfloat v3b[] = { -2.0f,  maxf, -maxf };
		GLubyte clr0[]    = { 100,200,maxb,maxb };
		GLubyte clr1[]    = { 100,180,maxb,maxb };
		GLubyte clr2[]    = { 132,22,maxb,maxb };
		GLubyte clr3[]    = { 132,22,maxb,maxb };

		glBegin( GL_TRIANGLES );
		glColor4f(0.0, 0.4, 1.0, 0.5);
		glVertex3fv( v0b );
		glVertex3fv( v1b );
		glVertex3fv( v2b );
		glVertex3fv( v0b );
		glVertex3fv( v2b );
		glVertex3fv( v3b );
		glEnd();

		glEnable( GL_DEPTH_TEST );
		glDisable(GL_BLEND);


	}  

	void BloodFilter()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable( GL_DEPTH_TEST );
		glShadeModel(GL_SMOOTH);
		GLfloat v0b[] = { -2.0f, -maxf, -maxf };
		GLfloat v1b[] = {  2.0f, -maxf, -maxf };
		GLfloat v2b[] = {  2.0f,  maxf, -maxf };
		GLfloat v3b[] = { -2.0f,  maxf, -maxf };
		GLubyte clr0[]    = { 100,200,maxb,maxb };
		GLubyte clr1[]    = { 100,180,maxb,maxb };
		GLubyte clr2[]    = { 132,22,maxb,maxb };
		GLubyte clr3[]    = { 132,22,maxb,maxb };

		glBegin( GL_TRIANGLES );
		glColor4f(0.7, 0.2, 0.5, 0.5);
		glVertex3fv( v0b );
		glVertex3fv( v1b );
		glVertex3fv( v2b );
		glVertex3fv( v0b );
		glVertex3fv( v2b );
		glVertex3fv( v3b );
		glEnd();

		glEnable( GL_DEPTH_TEST );
		glDisable(GL_BLEND);
	}

}Draw;

void GroundBot::Kill()
{
	HPb=0;
	Exists=false;
}

bool GroundBot::NewPosition()
{
	randyr=rand()%360;
	for(int tries=0;tries<16;tries++)
	{
		x=rand()%terx;
		z=rand()%terz;		
		if(scene[(int)x][(int)z].h>WATER_HEIGHT)
			return true;
	}
	return false;
}

bool GroundBot::Spawn()
{
	if(NewPosition())
	{Exists=true;
	HPb=100+rnd(0,155);
	return true;}
	return false;
}
void GroundBot::draw(ubyte color)
{
	if(Dist3D(x,y,z,xpos,ypos,zpos)>768.0f)
		return;

	float cl=0.5f+color/512.0f;
	glDisable(GL_BLEND);

	glColor4f(maxf,cl,cl,maxf);

	float yr2=yr*180/PI;
	Draw.Box(x,y,z,2.5f,5.5f,1.25f,0.0f,yr2,0.0f,Textures.Weapon[BAZOOKA].B);

	glColor4f(maxf,maxf,maxf,maxf);

	{
		Vector2D a;

		a.MakeFromXY(0.15,0.0);
		a=a.Rotate(yr);
		Draw.Box(x+a.GetY(),y+0.5f,z+a.GetX(),1.8f,0.2f,1.5f,0.0f,yr2,0.0f,Textures.Weapon[MACHINEGUN].B);

		a.MakeFromXY(0.5,0.45);
		a=a.Rotate(yr);
		Draw.Box(x+a.GetY(),y+1.5f,z+a.GetX(),0.2f,0.2f,0.5f,0.0f,yr2,0.0f,Textures.Weapon[MACHINEGUN].B);

		a.MakeFromXY(0.5,-0.45);
		a=a.Rotate(yr);
		Draw.Box(x+a.GetY(),y+1.5f,z+a.GetX(),0.2f,0.2f,0.5f,0.0f,yr2,0.0f,Textures.Weapon[MACHINEGUN].B);
	}
	glColor4f(maxf,maxf,maxf,maxf);

}

void GroundBot::BreakWalls()
{
	coord xc=(int)(x/2)+rnd(-1,1),yc=(int)(y/2)+rnd(-1,1),zc=(int)(z/2)+rnd(-1,1);
	if(cubescene[xc][yc][zc].type!=0)
	{
		cubescene[xc][yc][zc].type=0;
		freezetime=16;
	}
}

void GroundBot::Process()
{
	bool Attack=false;
	if(Exists)
	{
		draw(HPb);

		if(Stop)return;

		if(freezetime>0)
		{
			freezetime--;
			return;
		}
		if(x<1)x=2;
		if(z<1)z=2;
		if(x>terx)x=terx-2;
		if(z>terz)z=terz-2;


		y=scene[(int)x][(int)z].h+3.0f;

		if(HPb<0)
		{
			Exists=false;
			Spawn();
		}

		float sped;

		float x3=sin(yr);
		float z3=cos(yr);

		if(Dist3D(x,y,z,xpos,ypos,zpos)<64.0f && GAME.mode!=GAME.MODES.menu)
		{
			randyr+=(rnd(-3,3))/256.0f;
			if(!Found)
			{
				randyr=0;
				Found=true;
			}
			yr=atan((x-xpos)/(z-zpos))+randyr;

			if(z>zpos){
				yr+=PI;}

			sped=3.0f;
			sped+=abs(scene[(int)x][(int)z].h-scene[(int)(x+x3)][(int)(z+z3)].h)*8;

			if(Dist3D(xpos,ypos,zpos,x,y,z)<4.0f)
				Attack=true;
			if(Attack)
			{
				sped+=128.0f;
				if(!Cheat && HP>0)
				{HP-=1;
				Sound.Play.Pickup();
				}
			}
		}
		else
		{
			randyr+=rnd(-3,3)/64.0f;
			yr=randyr;

			if(y<WATER_HEIGHT)
			{
				x3*=-4;
				z3*=-4;	
				randyr+=PI;
			}
			else
			{
				x3=sin(yr);
				z3=cos(yr);
			}

			sped=8.0f;
			sped+=abs(scene[(int)x][(int)z].h-scene[(int)(x+x3)][(int)(z+z3)].h)*8;

		}
		BreakWalls();
		x+=x3*T/sped;
		z+=z3*T/sped;
	}
}



HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application
//bool	keys[256];			// Array Used For The Keyboard Routine

GLfloat	rtri;				// Angle For The Triangle ( NEW )
GLfloat	rquad;				// Angle For The Quad ( NEW )
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
	glViewport(0,0,width,height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(FOV, ratio,nearclip,farclip);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}
GLfloat xtestx=0,ytesty=0,ztestz=0;
void projectile::MakeProj(float x, float y, float z, float vecx, float vecy, float vecz, float dmg,float dmgrad2)
{
	ystart=y;
	//if(!IsExists)
	//{
	mx=vecx;my=vecy;mz=vecz;
	px=x;py=y;pz=z;
	IsExists=true;
	dmgrad=dmgrad2;
	accel=0.0f;
	TICK=0;

}
void projectile::DrawProj()
{
	if(IsExists)
	{
		Draw.Projectile(px,py,pz,Textures.Weapon[BAZOOKA].B);
		Move();
	}
}
void projectile::Explode()
{
	int x2=(int)px,y2=(int)py,z2=(int)pz;
	int x4,y4,z4;

	for(coord a=0;a<16;a++)
	{
		float dist=Dist3D(GroundBot[a].x,GroundBot[a].y,GroundBot[a].z,px,py,pz);
		if(dist<32.0f)			  
		{
			GroundBot[a].HPb-=256;
			for(int ran=-4;ran<4;ran++)
			{
				int xra=+rnd(-2,2),zra=+rnd(-2,2);
				clr0rgb[(int)GroundBot[a].x+xra][(int)GroundBot[a].z+zra].r=140+rnd(0,15);
				clr0rgb[(int)GroundBot[a].x+xra][(int)GroundBot[a].z+zra].g=80;
				clr0rgb[(int)GroundBot[a].x+xra][(int)GroundBot[a].z+zra].b=30;
			}
		}	
	}

	if(OnLevel(x2,y2,z2))
	{
		Sound.Play.Foot();
		for(int x=-dmgrad;x<dmgrad;x++)
		{
			for(int z=-dmgrad;z<dmgrad;z++)
			{
				coord x3=x2+x, y3=y2+y, z3=z2+z;
				float damage2=dmgrad/(abs(Dist3D(0,y2,0,x,scene[x3][z3].h,z))+0.5f);
				scene[x3][z3].h-=damage2;
				if(clr0rgb[x3][z3].r>48)
					clr0rgb[x3][z3].r-=4*damage2;
				if(clr0rgb[x3][z3].g>48)
					clr0rgb[x3][z3].g-=4*damage2;
				if(damage2>2.0 && IsGrass(scene[x3][z3]))
					scene[x3][z3].type=GRASS;
			}
		}
		dmgrad/=2;
		for(int x=-dmgrad;x<dmgrad;x++)
		{
			for(int y=-dmgrad;y<dmgrad;y++)
			{
				for(int z=-dmgrad;z<dmgrad;z++)
				{
					coord x3=(px+x)/2, y3=(py+y)/2, z3=(pz+z)/2;
					cubescene[x3][y3][z3].type=0;
				}
			}
		}
	}
	IsExists=false;
}
void projectile::Move()
{
	TICK+=T;

	if(TICK>200)
		Explode();
	accel+=0.01f;
	px+=mx*T;
	py=-GRAVITY*TICK*TICK/2+my*TICK+ystart;    //(my-accel)*T;
	pz+=mz*T;
	int x2=(int)px,y2=(int)py,z2=(int)pz;

	float rounding=2.0f;

	for(int x3=-1;x3<1;x3++)
		for(int z3=-1;z3<1;z3++)
		{
			if((int)(scene[x2+x3][z2+z3].h/rounding)==(int)(y2/rounding))
			{Explode();return;}
		}
		if(cubescene[x2/2][y2/2][z2/2].type!=0)
		{Explode();return;}
}

bool MakeRocket(int a, float x, float y, float z, float vecx, float vecy, float vecz, float dmg, float dmgrad)
{
	for(coord b=0;b<10;b++)
	{
		if(!proj[b].IsExists)
		{
			proj[b].MakeProj(x,y,z,vecx,vecy,vecz,dmg,dmgrad);
			return true;
		}
	}
	return false;
}

void AddAmmo(ubyte slot, int ammo)
{
	if(slot==2)
	{
		if(HP<64)
			HP+=192;
		else
		{
			HP=maxb;
		}
		return;
	}
	if(slot==3 && Weapon[3].Own)
	{
		slot=rnd(4,5);
		ammo=MaxAmmo[slot]/4;
	}
	Weapon[slot].Own=true;
	int ammo2=Weapon[slot].Ammo;
	ammo2+=ammo;
	if(ammo2>MaxAmmo[slot])
		Weapon[slot].Ammo=MaxAmmo[slot]-1;
	else
	{
		Weapon[slot].Ammo=ammo2;
	}
}


class Input
{
public:
	int xdeb, ydeb, zdeb, ydebspec;
	bool Deadonce;
	coord MyTick;

	enum statenum
	{ 
		walking,
		cube,
		inair
	};

	bool wlock,alock,slock,dlock;

	Vector2D move;
	Vector2D finalmove;

	float Accel;

	void ClearKeys()
	{
		coord a;
		for(a=0;a<256;a++)
		{
			keys[a].Hit=false;
			keys[a].Rel=false;
		}
		LMB.Hit=false;
		LMB.Rel=false;

		keys[VK_SPACE].TICK++;

		LMB.TICK++;

		SetCursorPos(SCREEN_CENTER_X/2,SCREEN_CENTER_Y/2);
	}


	bool Hit(float dist, float precision, int damage, bool destroyblocks, float pushforce, float maxdist, bool blood)
	{
		float shit=dist*precision;
		static GLfloat clr;
		static GLfloat x3,y3,z3,z2,x2,y2,z,a;
		static GLint oldposx,oldposz,xi,yi,zi;
		clr0rgb[oldposx][oldposz].r=clr;
		x3=sin(yrot*PIover180)*cos(xrot*PIover180);
		y3=-sin(xrot*PIover180);
		z3=-cos(yrot*PIover180)*cos(xrot*PIover180);
		for(a=0;a<shit;a++)
		{
			x2=x3*(a/precision)+xpos;
			y2=y3*(a/precision)+ypos;
			z2=z3*(a/precision)+zpos;
			xi=(int)(x2);
			yi=(int)(y2);
			zi=(int)(z2);

			coord xs=(int)(x2/2.0f+0.25),ys=(int)(y2/2.0f+0.25),zs=(int)(z2/2.0f+0.25);
			if(destroyblocks && OnLevel(xs,ys,zs) && cubescene[xs][ys][zs].type!=0)
			{

				cubescene[xs][ys][zs].type=0;
				return true;
			}
			for(coord b=0; b<botsnum; b++)
			{
				if(Dist3D(GroundBot[b].x,GroundBot[b].y,GroundBot[b].z,x2,y2,z2)<maxdist)//(int)(GroundBot[b].x/2.0f)==(int)(x2/2.0f) && (int)(GroundBot[b].y/4.0f)==(int)(y2/4.0f) && (int)(GroundBot[b].z/2.0f)==(int)(-z2/2.0f))
				{
					GroundBot[b].HPb-=damage;
					GroundBot[b].x+=x3*pushforce;

					GroundBot[b].z+=z3*pushforce;

					if(blood)
					{
						clr0rgb[(int)GroundBot[b].x][(int)GroundBot[b].z].r=140+rnd(0,15);clr0rgb[(int)GroundBot[b].x][(int)GroundBot[b].z].g=80;clr0rgb[(int)GroundBot[b].x][(int)GroundBot[b].z].b=30;
					}

					GroundBot[b].randyr+=(rnd(-3,3))/16.0f;
					return true;
				}
			}
		}
		return false;
	}

	bool raytrace(int &xout,int &zout)
	{

		static GLfloat clr;
		static GLfloat x3,y3,z3,z2,x2,y2,z,a;
		static GLint oldposx,oldposz,xi,yi,zi;
		clr0rgb[oldposx][oldposz].r=clr;
		x3=sin(yrot*PIover180)*cos(xrot*PIover180);
		y3=-sin(xrot*PIover180);
		z3=-cos(yrot*PIover180)*cos(xrot*PIover180);
		for(a=0;a<512;a++)
		{
			x2=x3*(a/4.0)+xpos;
			y2=y3*(a/4.0)+ypos;
			z2=z3*(a/4.0)+zpos;
			xi=(int)(x2);
			yi=(int)(y2);
			zi=(int)(z2);

			if(xi<terx && zi<terz && xi>0 && zi>0 && ((int)(scene[xi][zi].h)==yi || cubescene[xi/2][(int)(yi/2)][zi/2].type!=0))
			{
				xout=xi;
				zout=zi;
				xtestx=x2;
				ytesty=y2;
				ztestz=z2;
				return true;
			}
		}
		return false;
	}

	void dig(float level,bool addcube, bool debug, bool weak=1)
	{
		int xo,zo;

		  


		if(raytrace(xo,zo))
		{
			if(!debug)
			{
				if(Dist3D(xpos,ypos,zpos,xo,scene[xo][zo].h,zo)<10.0f)
				{
					if(!weak || scene[xo][zo].type==GRASS || scene[xo][zo].type==SAND)
					{
					scene[xo][zo].h+=level;
					scene[xo+1][zo].h+=level;
					scene[xo][zo-1].h+=level;
					scene[xo][zo+1].h+=level;
					scene[xo-1][zo].h+=level;
					}

					if(IsGrass(scene[xo][zo]))		scene[xo][zo].type=GRASS;
					if(IsGrass(scene[xo+1][zo]))	scene[xo+1][zo].type=GRASS;
					if(IsGrass(scene[xo][zo-1]))		scene[xo][zo-1].type=GRASS;
					if(IsGrass(scene[xo][zo+1]))	scene[xo][zo+1].type=GRASS;
					if(IsGrass(scene[xo-1][zo]))		scene[xo-1][zo].type=GRASS;
				}
			}

			coord h=(int)((scene[xo][zo].h+scene[xo][zo+1].h+scene[xo+1][zo+1].h+scene[xo+1][zo].h)/8.0)+1;
			if(ypos-CAM_HEIGHT-h*2>4.0f)
			{
				h=(int)((ypos-CAM_HEIGHT)/2.0f);
			}

			if(Dist3D(xpos,ypos,zpos,xo,h*2.0f,zo)<10.0f)
			{
				xo=(xo+1)/2;
				zo=(zo+1)/2;
				if(xdeb!=0 && addcube )
				{
					if(!AddCube(xo,h,zo,CONCRETE,true))
						if(!AddCube(xo,h+1,zo,CONCRETE,true))
							if(!AddCube(xo,h+2,zo,CONCRETE,true))
								AddCube(xo,h+3,zo,CONCRETE,true);
				}
				if(debug)
				{

					for(coord h2=0;h2<4;h++)
					{
						if(cubescene[xo][h+h2][zo].type==0)
						{
							xdeb=xo;ydeb=h+h2;zdeb=zo;
						}
					}
					if(cubescene[xo][ydeb-1][zo].type==CRATE)
					{
						if(LMB.Hit)
						{
							cubescene[xo][ydeb-1][zo].type=0;
							coord b=rnd(2,5);
							AddAmmo(b,MaxAmmo[b]/4);
							Sound.Play.Beep();
							xdeb=0;ydeb=0;zdeb=0;
							return;
						}
						ydebspec=ydeb-1;
					}
					else
						ydebspec=0;
				}

			}
			if(Inv.slot!=BLOCKS)
			{
				xdeb=0;ydeb=0;zdeb=0;
			}
		}
	}
	int smgtick;
	void smgshoot(float damage)
	{
		int xo,zo;
		smgtick++;
		if(smgtick%5==0)
		{
			Weapon[MACHINEGUN].Ammo-=1;
			Sound.Play.Pickup();
			Hit(64.0f,4.0f,16, true, maxf, 2.0f, true);
		}

		if(raytrace(xo,zo))
		{
			if(Dist3D(xpos,ypos,zpos,xo,scene[xo][zo].h,zo)<128.0f)
			{
				scene[xo][zo].h+=rndf()-0.5f;
				if(clr0rgb[xo][zo].r>32.0f)clr0rgb[xo][zo].r-=2.0f+2*rndf();
				if(clr0rgb[xo][zo].g>32.0f)clr0rgb[xo][zo].g-=2.0f+2*rndf();
				if(clr0rgb[xo][zo].b>32.0f)clr0rgb[xo][zo].b-=2.0f+2*rndf();
				if(scene[xo][zo].type>=GRASS1 && scene[xo][zo].type<=GRASS3)
					scene[xo][zo].type=GRASS;
			}
			coord h=(int)((scene[xo][zo].h+scene[xo][zo+1].h+scene[xo+1][zo+1].h+scene[xo+1][zo].h)/8.0)+1;
			if(Dist3D(xpos,ypos,zpos,xo,scene[xo][zo].h,zo)<10.0f)
			{
				xo/=2;
				zo/=2;
			}
		}
	}
	void jump()
	{
		Accel+=0.5/(InWater()+1);
		state=inair;
	}
	void gravity()
	{
		if (state==inair)
		{
			int xx2=(int)(xpos/2.0+0.5);
			int zz2=(int)(zpos/2.0+0.5);
			int yy2=(int)((ypos-CAM_HEIGHT)/2.0+0.5);

			Accel-=GRAVITY/(InWater()+1);

			if(Accel<0.0)
			{
				if((scene[(int)(xpos)][(int)(zpos)].h-ypos)>=-CAM_HEIGHT && Accel<0.0)
				{
					Accel=0; state=walking;
				}
			}

			ypos+=Accel;
		}
	}
	worldgrid SafeScene(int xpos,int zpos)
	{
		if(xpos>0 && zpos>0 && xpos<terx && zpos<terz)
			return scene[xpos][zpos];
		else
			return scene[0][0];
	}


	void Collision()
	{
		int finxmove=finalmove.GetX();
		int finzmove=finalmove.GetY();

		//int x=int(xpos);

			switch(state)
			{
			case walking:

				
				if(cubescene[int(xpos)/2][int(ypos)/2][int(zpos)/2].type==CONCRETE)
				{
				finxmove=0;
				zpos+=move.GetY();
				}

				break;

			case inair:

				break;
			}

			xpos+=finxmove;
			zpos+=finzmove;


	}


	void SmoothCamera()
	{
		static float yrotating;
		realcamera[1]=SafeScene((int)(xpos),(int)(zpos)).h+CAM_HEIGHT;
		if(xpos>0 && xpos<LevelSizeX && zpos>0 && zpos<LevelSizeZ && GAME.mode==GAME.MODES.game){
			float zpos2=cos(yrot*PIover180)*3,xpos2=sin(yrot*PIover180)*3;
			float dist=realcamera[1]-fincamera[1];
			float bobdist=dist;

			int xx2=(int)(xpos/2.0+0.5);
			int zz2=(int)(zpos/2.0+0.5);
			int yy2=(int)((ypos-CAM_HEIGHT)/2.0+0.5);
			float distcol=8.0f;

			wlock=false;
			alock=false;
			slock=false;
			dlock=false;	

			switch(state)
			{
			case walking:
				if(fincamera[1]-scene[(int)(xpos+xpos2)][(int)(zpos+zpos2)].h-CAM_HEIGHT<-3.0f){

					wlock=true;
				}

				if(bobdist<0.5)bobdist=0;
				if(keys['W'].press && !wlock)
				{					  
					yrotating+=0.2;
				}

				if(dist<-0.2f)
					fincamera[1]+=(dist)/8.0f;
				else
					fincamera[1]+=(dist)/4.0f;
				ypos=fincamera[1];
				movespeed=0.2;
				break;

			case inair:
				//if(ypos-scene[(int)(xpos+xpos2)][(int)(-zpos-zpos2)].h<-CAM_HEIGHT){wlock=true; xpos-=xpos2; zpos+=zpos2;}else{wlock=false;}
				//fincamera[1]=realcamera[1];
				break;
			}
		}else{GAME.mode=GAME.MODES.spectate;movespeed=4;}

		Collision();
	}


	void Check()
	{

		if(GAMEOVER && !Deadonce)Sound.Play.Dead();

		if(HP<20 || HP>maxb)
		{GAMEOVER=true;Sound.Play.Dead();}
		if(!GAMEOVER)
		{
			float x3,y3,z3,z3h,y3h,x3h;
			if(GAME.mode==GAME.MODES.game)
			{
				dig(0.0f,false,true);

				if(keys[VK_UP].Hit && Cheat){
					BuildScene(true,true);}
				if(keys[VK_DOWN].Hit && Cheat){
					big--;
					BuildScene(true,true);}
				if(keys[VK_SPACE].Hit && keys[VK_SPACE].TICK>20){
					keys[VK_SPACE].TICK=0;
					if(GAME.mode==GAME.MODES.spectate && Cheat)
						should_rotate = !should_rotate;
					if(GAME.mode==GAME.MODES.game)
					{jump();}}
				if(keys[VK_TAB].press && Cheat){
					Smooth();
					BuildScene(false,true);}

				if(keys['0'].Hit)
					Inv.slot=0;
				if(keys['1'].Hit)
				{Inv.slot=1;Sound.Play.Beep();}
				if(keys['2'].Hit)
				{Inv.slot=2;Sound.Play.Beep();}
				if(keys['3'].Hit && Weapon[3].Own)
				{Inv.slot=3;Sound.Play.Beep();}
				if(keys['4'].Hit && Weapon[4].Own)
				{Inv.slot=4;Sound.Play.Beep();}
				if(keys['5'].Hit && Weapon[5].Own)
				{Inv.slot=5;Sound.Play.Beep();}
				if(keys['6'].Hit)
					Inv.slot=6;
				if(keys['7'].Hit)
					Inv.slot=7;
				if(keys['8'].Hit)
					Inv.slot=8;
				if(keys['9'].Hit)
					Inv.slot=9;
				//if(keys['R'].press && Cheat){
				//	Terrain.JustMakeItBigger();
				//	BuildScene(false,true);}
				//if(keys['T'].press && Cheat){
				//	Terrain.JustMakeItSmaller();
				//	BuildScene(false,true);}
				if(LMB.press)
				{
					switch(Inv.slot)
					{
					case HANDS:
						break;
					case SPADE:
						dig(0.1f,false,false,true);
						break;
					case MACHINEGUN:
						smgshoot(maxf);
						break;
					case BLOCKS:
						break;
					}
				}
				if(RMB.press)
				{
					switch(Inv.slot)
					{
					case HANDS:
						break;
					case SPADE:
						dig(-0.1f,false,false,true);
						break;
					case BLOCKS:
						break;
					}
				}
				if(LMB.Hit)
				{

					LMB.Hit=false;
					switch(Inv.slot)
					{
					case HANDS:
						dig(0.0f,false,false);

						x3=sin(yrot*PIover180)*cos(xrot*PIover180);
						y3=-sin(xrot*PIover180);
						z3=cos(yrot*PIover180)*cos(xrot*PIover180);
						if(LMB.TICK>16)
						{
							LMB.TICK=0;
							if(Hit(2.0f,16.0f,32, false, 2.0f, 4.0f, false))
								Sound.Play.Foot();
						}
						break;
					case SPADE:

						if(LMB.TICK>16)
						{
							LMB.TICK=0;
							if(Hit(4.0f,16.0f,16, true, 4.0f, 4.0f, false))
								Sound.Play.Foot();
						}
						break;
					case BAZOOKA:
						x3=sin(yrot*PIover180)*cos(xrot*PIover180);
						y3=-sin(xrot*PIover180);
						z3=-cos(yrot*PIover180)*cos(xrot*PIover180);
						x3h=sin((yrot+90.0f)*PIover180)*cos((xrot)*PIover180);
						y3h=-sin((xrot)*PIover180);
						z3h=cos((yrot+90.0f)*PIover180)*cos((xrot)*PIover180);

						MakeRocket(0,xpos+x3*4,ypos+y3*4,zpos+z3*4,x3,y3,z3,16.0f,8.0f);
						Weapon[BAZOOKA].Ammo-=1;
						break;
					case BLOCKS:
						dig(0.0f,true,false);
						break;
					}
				}
				if(RMB.Hit)
				{
					RMB.Hit=false;
					switch(Inv.slot)
					{
					case HANDS:

						if(LMB.TICK>16)
						{
							LMB.TICK=0;
							if(Hit(2.0f,16.0f,32, false, 2.0f, 4.0f, false))
								Sound.Play.Foot();

						}
						break;
					case SPADE:
						if(LMB.TICK>16)
						{
							if(Hit(4.0f,16.0f,16, true, 4.0f, 4.0f, false))
								Sound.Play.Foot();
						}
						break;
					case BLOCKS:
						break;
					}
				}

				if(keys['G'].Hit && Cheat){
					Debug=!Debug;
					BuildScene(false,true);}
				bool pressedy=false;
				if(keys['Y'].Hit && Cheat){
					GAME.mode++;
					if (GAME.mode>3) GAME.mode=0;
					pressedy=true;	}
				if(keys[VK_ESCAPE].Hit)
				{}

				if(keys['F'].press && Cheat)
				{
					physup=true;
				}else physup=false;
				if(keys['H'].press)
				{
					int b=0;
					for(int a=0;a<1024*1024*128;a++)
					{b++;}
				};
			}

			move.angle=yrot*PIover180;
			move.scalar=1.0f;

			float forwardx=move.GetY(),//*cos(xrot*PIover180),
				forwardy=-movespeed,//*sin(xrot*PIover180),
				forwardz=-move.GetX(),//*cos(xrot*PIover180),
				leftx=-move.Rotate(PI/2).GetY(),
				leftz=move.Rotate(PI/2).GetX();
			
			float finxmove=0,finymove=0,finzmove=0;

 					/*finxmove=move.GetY()*cos(xrot*PIover180);
					finzmove=
					finymove=*/

			if(keys['W'].press)
			{
				slock=false;
				finymove+=forwardy;

				if(!wlock)
				{
					finxmove+=forwardx;
					finzmove+=forwardz;
				}
			}

			if(keys['S'].press)
			{
				wlock=false;
				finymove-=forwardy;
				
				if(!slock)
				{
					finxmove-=forwardx;
					finzmove-=forwardz;
				}
			}
			if(keys['A'].press)
			{
				dlock=false;
				if(!alock)
				{
					finxmove+=leftx;
					finzmove+=leftz;
				}
			}
			if(keys['D'].press)
			{
				alock=false;
				if(!dlock)
				{
					finxmove-=leftx;
					finzmove-=leftz;
				}
			}

			

			finalmove.MakeFromXY(finxmove,finzmove);
			
			
			finalmove.Normalize();


			xpos+=finalmove.GetX()*movespeed*T;
			//ypos+=finymove;
			zpos+=finalmove.GetY()*movespeed*T;
			

			int localx,localy;
			POINT ptCursor;
			GetCursorPos( &ptCursor );		

			localx=ptCursor.x;
			localy=ptCursor.y;		
			int deltax=(localx-(signed int)(SCREEN_CENTER_X/2));
			int deltay=(localy-(signed int)(SCREEN_CENTER_Y/2));
			yrot+=deltax/32.0;
			xrot+=deltay/32.0;

			if(xrot>90.0f)xrot=90;
			if(xrot<-90.0f)xrot=-90;

			if(GAME.mode!=GAME.MODES.menu)
			{
				SmoothCamera();
				gravity();
			}
			else
			{
				float h = scene[(int)(xpos)][(int)(zpos)].h+maxf;
				if(h-ypos>0.0f){ypos=h;}
			}

			ClearKeys();
		}
	}
}Input;


//*************************
//INITGL START STARTGAME GO FIRST LABEL
//*************************
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	BuildScene(true,true);

	GAME.mode=GAME.MODES.menu;//game;
	Menu=true;

	xpos=landsizeh;
	ypos=scene[(int)xpos][(int)zpos].h+32;
	yrot=121;

	Draw.DrawCube=true;
	glShadeModel( GL_SMOOTH  );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( FOV, ratio, nearclip, farclip );
	// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(maxf);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return TRUE;										// Initialization Went OK
}

	//GLubyte red[]    = { maxb,   0,   0, maxb };
	//GLubyte green[]  = {   0, maxb,   0, maxb };
	//GLubyte blue[]   = {   0,   0, maxb, maxb };
	//GLubyte white[]  = { maxb, maxb, maxb, maxb };
	//GLubyte yellow[] = {   0, maxb, maxb, maxb };
	//GLubyte black[]  = {   0,   0,   0, maxb };
	//GLubyte orange[] = { maxb, maxb,   0, maxb };
	//GLubyte purple[] = { maxb,   0, maxb,   0 };




class render
{
private:
	void Clear()
	{
 	glLoadIdentity( );
	gluPerspective( FOV, ratio, nearclip, farclip );

	if(!OnLevel(xpos,ypos,zpos) && GAME.mode==GAME.MODES.game)
		GAMEOVER=true;
	if(GAME.mode==GAME.MODES.spectate)
		GAME.mode=GAME.MODES.game;
	static float angle = 0.0f;

	xscene=-xpos;
	yscene=-ypos;
	zscene=-zpos;
	coord a;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity( );
	}

	void Sky()
	{
	

	if(!Debug && !InWater())
		Draw.Sky();

	glLoadIdentity( );
	}

	void TransformScene()
	{
	Rotate(xrot,yrot,zrot);
	glTranslatef( xscene, yscene, zscene);
	}

	void Fog()
	{
	glEnable(GL_DEPTH_TEST);
	if(Debug)
		glDisable(GL_FOG);
	else glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);        // Fog Mode
	glFogfv(GL_FOG_COLOR, fogclr);            // Set Fog Color
												//glFogf(GL_FOG_DENSITY, 0.35f);              // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST);          // Fog Hint Value
	if(ypos>WATER_HEIGHT)
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
	glEnable( GL_CULL_FACE );
	glFrontFace( GL_CW );
	}

	void Terrain()
	{
	for(int x2=0;x2<chunkx;x2++){for(int z2=0;z2<chunkz;z2++){
		bLOD=0;
		float dist = sqrt(pow(x2*chunksize+chunksize/2.0-xpos,2.0)+pow(z2*chunksize+chunksize/2.0-zpos,2.0));

		if(!Debug){
			if(dist > 64.0f) bLOD++;
			if(dist > 128.0f) bLOD++;
			//if(dist > 512.0f) LOD=16;
			if(dist > 512.0f) bLOD++;
			//if(dist > 768.0f) bLOD++;
			//if(dist > 1024.0f) bLOD++;

			LOD=(int)pow(2.0f,bLOD);

			if(LOD<2)glShadeModel(GL_FLAT);else glShadeModel(GL_SMOOTH);
		}
		else{
			glShadeModel(GL_SMOOTH);
		}
		coord ChunkdivLOD=chunksize/LOD;

		if(LOD==1)
		{
			for(int x=0+chunksize*x2;x<chunksize+chunksize*x2;x++){
				glBegin( GL_QUAD_STRIP );				
				for(int z=chunksize*z2;z<chunksize+chunksize*z2+1;z++){
					coord LODx2=(x+1), LODx3=(x+2);//LODz2=(z+1), LODz3=(z+2);
					GLfloat v0b[] = {  x,  scene[x][z].h,  z };
					GLfloat v3b[] = {  LODx2,  scene[LODx2][z].h,  z };
					GLubyte clr0[]    = { clr0rgb[x][z].r,clr0rgb[x][z].g,clr0rgb[x][z].b,maxb };
					GLubyte clr3[]    = { clr0rgb[LODx2][z].r,clr0rgb[LODx2][z].g,clr0rgb[LODx2][z].b,maxb };

					glColor4ubv( clr0 );// 0 0

					glVertex3fv( v3b );

				}		
				glEnd();
			}
		}
		else{
			glDisable(GL_TEXTURE_2D);

			if(LOD<3)glShadeModel(GL_FLAT);else glShadeModel(GL_SMOOTH);

			for(int x=0+chunksize*x2/LOD;x<chunksize/LOD+chunksize*x2/LOD;x++){
				glBegin( GL_QUAD_STRIP );				
				for(int z=chunksize*z2/LOD;z<chunksize/LOD+chunksize*z2/LOD+2;z++){
					coord LODx=(x)*LOD, LODz=(z)*LOD, LODx2=(x+1)*LOD;//, LODx3=(x+2)*LOD;// LODy2=(z+1)*LOD;//, LODy3=(z+2)*LOD;
					float addx=0,addx2=0,addz=0,addz2=0;
					float megalod;
					if(LOD>1)
					{
						float add=-bLOD*2+1;//-0.0-sqrt(LOD*1.0);//-LOD*2.0;
						if(x==x2*chunksize/LOD){addx=add;}
						if(z==z2*chunksize/LOD){addz=add;}
						if(x==((x2+1)*chunksize/LOD-1)){addx2=add;}
						if(z==((z2+1)*chunksize/LOD-1)){addz2=add;}
					}
					GLfloat v0b[] = {  LODx,  scene[LODx][LODz].h+addx+addz,  LODz };
					GLfloat v3b[] = {  LODx2,  scene[LODx2][LODz].h+addx2+addz,  LODz };
					GLubyte clr0[]    = { clr0rgb[LODx][LODz].r,clr0rgb[LODx][LODz].g,clr0rgb[LODx][LODz].b,maxb };
					GLubyte clr3[]    = { clr0rgb[LODx2][LODz].r,clr0rgb[LODx2][LODz].g,clr0rgb[LODx2][LODz].b,maxb };

					glColor4ubv( clr0 );// 0 0
					glVertex3fv( v0b );
					glColor4ubv( clr3 );// 1 0
					glVertex3fv( v3b );

				}		
				glEnd();
			}		}
	}}
	}

	void Grass(){

		float cosa=cos(yrot*PIover180);
	float sina=sin(yrot*PIover180);

	int currentchunkx=(int)(xpos/chunksize);
	int currentchunkz=(int)(zpos/chunksize);

	int fullnum=5;

	for(int x2=currentchunkx-fullnum; x2<currentchunkx+fullnum; x2++)
		for(int z2=currentchunkz-fullnum; z2<currentchunkz+fullnum; z2++)
		{
			if(x2>0 && z2>0)
			{

				for(int x=x2*chunksize; x<x2*chunksize+chunksize; x++)
				for(int z=z2*chunksize; z<z2*chunksize+chunksize; z++)
				{
					if(IsGrass(scene[x][z]))
					{
					float width=0.2f;

					float addheight=0.0f,height=(scene[x][z].type-GRASS1)/2.0f+maxf;//-dist/48.0f;
					GLubyte clr[] = { 20,140,20,maxb };
					GLubyte clr0[]    = { clr0rgb[x][z].r,clr0rgb[x][z].g,clr0rgb[x][z].b,maxb };

					GLfloat vert1[] = { x, scene[x][z].h-0.5f, z };
					GLfloat vert2[] = { x, scene[x][z].h+addheight+height, z };
					GLfloat vert3[] = { x-cosa*width, scene[x][z].h-0.5f+addheight, z-sina*width };
					GLfloat vert4[] = { x-cosa*width, scene[x][z].h+height+addheight, z-sina*width };

					glBegin( GL_QUADS );

					glColor4ubv( clr );
					//glTexCoord2f(8, 0);
					glVertex3fv( vert2 );
					//glTexCoord2f(8, 8);
					glVertex3fv( vert1 );
					//glTexCoord2f(0, 8);
					glVertex3fv( vert3 );
					//glTexCoord2f(0, 0);
					glVertex3fv( vert4 );
					glEnd();
					}
				}
			}
		}

	//for(int x=xpos-GRASS_DIST;x<xpos+GRASS_DIST;x++)
	//{
	//	if(LimitCoord(x,0,LevelSizeX))
	//		for(int z=zpos-GRASS_DIST;z<zpos+GRASS_DIST;z++)
	//		{

	//			if(LimitCoord(z,0,LevelSizeZ) && IsGrass(scene[x][z]))
	//			{
	//				float width=0.2f;

	//				float addheight=0.0f,height=(scene[x][z].type-GRASS1)/2.0f+maxf;//-dist/48.0f;
	//				GLubyte clr[] = { 20,140,20,maxb };
	//				GLubyte clr0[]    = { clr0rgb[x][z].r,clr0rgb[x][z].g,clr0rgb[x][z].b,maxb };

	//				GLfloat vert1[] = { x, scene[x][z].h-0.5f, z };
	//				GLfloat vert2[] = { x, scene[x][z].h+addheight+height, z };
	//				GLfloat vert3[] = { x-cosa*width, scene[x][z].h-0.5f+addheight, z-sina*width };
	//				GLfloat vert4[] = { x-cosa*width, scene[x][z].h+height+addheight, z-sina*width };

	//				glBegin( GL_QUADS );

	//				glColor4ubv( clr );
	//				//glTexCoord2f(8, 0);
	//				glVertex3fv( vert2 );
	//				//glTexCoord2f(8, 8);
	//				glVertex3fv( vert1 );
	//				//glTexCoord2f(0, 8);
	//				glVertex3fv( vert3 );
	//				//glTexCoord2f(0, 0);
	//				glVertex3fv( vert4 );
	//				glEnd();
	//			}
	//		}
	//}



	}

	void SpecialObjects()
	{
	glFrontFace( GL_CCW );

	for(coord a=0;a<10;a++)
		proj[a].DrawProj();
	}

	void Cubes()
	{

	int xposh=xpos/2;
	int zposh=zpos/2;

		for(int x4=0;x4<chunkx;x4++)
		for(int z4=0;z4<chunkz;z4++)
		{
			if((abs((float)(x4*chunksize-chunksizeh)-xpos)>64 || abs((float)(z4*chunksize-chunksizeh)-zpos)>64))
				for(coord cn=0;cn<cubenum[x4][z4];cn++)
				{
					cube a=cubes[cn][x4][z4];

					if(cubescene[a.x][a.y][a.z].type!=0)
						if(abs((int)a.x-xposh)<48)
							if(abs((int)a.z-zposh)<48)
								Draw.Cube(a.x*2,a.y*2,a.z*2,0,0,0,maxf,cubescene[a.x][a.y][a.z].type);
				}
		}
	}

	void BuildingLODS()
	{
		for(coord cn=0;cn<buildnum;cn++)
		{
			if(Castles[cn].Enabled && (abs((float)Castles[cn].x-xpos)>64 || abs((float)Castles[cn].z-zpos)>64))
				Draw.Box(Castles[cn].x, Castles[cn].y-AdditionalCastleLODHeight, Castles[cn].z, Castles[cn].w, Castles[cn].h+AdditionalCastleLODHeight*2, Castles[cn].l,0.0f,0.0f,0.0f,Textures.Cube[CONCRETE]);

		}
	}

	void GameOver()
	{
		if(GAMEOVER)
		{
			glLoadIdentity( );
			Draw.BloodFilter();
			return;
		}
	}

	void DebugCube()
	{
			glColor4f(1.0, 1.0, 1.0, 1.0);

			if(Input.ydebspec==0)
				Draw.Cube(Input.xdeb*2,Input.ydeb*2,Input.zdeb*2,0,0,0,1,DEBUG);
			else
			{
				Draw.Cube(Input.xdeb*2,Input.ydebspec*2,Input.zdeb*2,0,0,0,1.1,CRATEDEBUG);
			}
	}

	void Water()
	{
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glColor4f(1.0, 1.0, 1.0, 1.0);			

			Draw.Water(true);
			Draw.Water(false);

			

			if(InWater())	
			{
				HP-=0.25f;
				glLoadIdentity( );
				Draw.SeaFilter();
			}
	}

	void DMenu()
	{

			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 1.0);

			glEnable(GL_CULL_FACE);

			glDisable(GL_BLEND);
			glDisable(GL_FOG);

			glLoadIdentity( );
			glColor4f(1.0, 1.0, 1.0, 1.0);

		if(Menu)
		{
			Inv.DrawMenu();
		}
	}

	void Hands()
	{	
		if(!Menu)
		Draw.Hands();	
	}

	void HUD()
	{	
		if(!Menu)
		Inv.DrawHUD();	
	}


public:

	void Cycle( void )
	{

		Clear();

		Sky();

		TransformScene();

		Fog();
	
		Terrain();

		Grass();

		SpecialObjects();

		for(coord n=0; n<botsnum; n++)
		{
			GroundBot[n].Process();
		}

		Cubes();

		BuildingLODS();

		GameOver();

		DebugCube();

		Water();
		
		//Draw.Clouds();

		DMenu();

		Hands();

		HUD();

	}


}Render;



GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}
	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}
	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}
	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}
/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height
	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag
	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name
	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","JarGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}
	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right-WindowRect.left,	// Calculate Window Width
		WindowRect.bottom-WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		4, 8, 16, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 1, 0,									// Accumulation Bits Ignored
		4,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 4										// Layer Masks Ignored
	};
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen
	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	//GAME.mode=GAME.MODES.game;//.spectate;
	should_rotate=false;
	BuildScene(false,false);
	//	setup_opengl( width, height );
	LoadImage();

	return TRUE;									// Success
}
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			// LoWord Can Be WA_INACTIVE, WA_ACTIVE, WA_CLICKACTIVE,
			// The High-Order Word Specifies The Minimized State Of The Window Being Activated Or Deactivated.
			// A NonZero Value Indicates The Window Is Minimized.
			if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)))
				active=TRUE;						// Program Is Active
			else
				active=FALSE;						// Program Is No Longer Active
			return 0;								// Return To The Message Loop
		}
	case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
			case SC_SCREENSAVE:					// Screensaver Trying To Start?
			case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}
	case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
	case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam].press = TRUE;					// If So, Mark It As TRUE
			keys[wParam].Hit = TRUE;
			return 0;								// Jump Back
		}
	case WM_LBUTTONDOWN:
		{
			LMB.press = TRUE;
			LMB.Hit = TRUE;
			return 0;
		}
	case WM_LBUTTONUP:
		{
			LMB.press = FALSE;
			LMB.Rel = TRUE;
			return 0;
		}
	case WM_RBUTTONDOWN:
		{
			RMB.press = TRUE;
			RMB.Hit = TRUE;
			return 0;
		}
	case WM_RBUTTONUP:
		{
			RMB.press = FALSE;
			RMB.Rel = TRUE;
			return 0;
		}
		//case MK_MBUTTON:
		//{
		//	 MMB.press = TRUE;
		//	 MMB.Hit = TRUE;
		//}
		//case MK_MBUTTON:
		//{
		//	 MMB.press = FALSE;
		//	 MMB.Rel = TRUE;
		//}
	case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam].press = FALSE;					// If So, Mark It As FALSE
			keys[wParam].Rel = TRUE;
			return 0;								// Jump Back
		}
	case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}							 
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	//BOOL	done=FALSE;								// Bool Variable To Exit Loop
	static DWORD timer,DeltaT;						  
	int t=0;

	DWORD tickpre=GetTickCount();
	DWORD tickpost=GetTickCount();

	bool SKIPWAITING=false;
	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "64kb KONKYRZ IGOR",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}
	// Create Our OpenGL Window
	if (!CreateGLWindow("64kb ZShooter",SCREEN_W,SCREEN_H,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	Sound.InitWave();

	if (waveOutOpen(&hWaveOut,0,&WaveFormat,(DWORD) Done,0,CALLBACK_EVENT) != MMSYSERR_NOERROR) 
	{        
		Message("Sound card cannot be opened.");
		return TRUE;
	}
	ResetEvent(Done);

	//DWORD tickwhile;


	while(!done)									// Loop That Runs While done=FALSE
	{

		

		TICKWIN=GetTickCount();
		TICKFRM--;
		if(GetTickCount()-timer>1486-DeltaT)
		{
			timer=GetTickCount();
			Sound.Play.Govbuf(t,0);
			t++;
			waveOutPause(hWaveOut);
			waveOutReset(hWaveOut);
			if (waveOutPrepareHeader(hWaveOut,&WaveHeader,sizeof(WaveHeader)) != MMSYSERR_NOERROR)
			{
				Message("Error preparing Header!");
				return TRUE;
			}
			waveOutWrite(hWaveOut,&WaveHeader,sizeof(WaveHeader));

		}
		DeltaT=GetTickCount()-timer;

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			

			if (keys[VK_ESCAPE].Hit)	// Active?  Was There A Quit Received?  && !DrawGLScene()     active
			{
				if(Cheat)done=TRUE;							// ESC or DrawGLScene Signalled A Quit
				else
					Inv.SwitchMenu();
			}

			tickpre=GetTickCount();

			Render.Cycle();

			

			

			

			if(!Stop)Input.Check();
			Input.ClearKeys();
			//}			
			//DWORD tickwhile2=GetTickCount();

			

			tickpost=GetTickCount();
			
			int rendertime=tickpost-tickpre;
			

			if(rendertime<=DELTA)
			{
				Sleep(DELTA-rendertime);
				T=1.0f;
			}
			else
			{
				T=(rendertime)/DELTA;
			}
		}
		if (keys[VK_F1].press)						// Is F1 Being Pressed?
		{
			keys[VK_F1].press=FALSE;					// If So Make Key FALSE
			KillGLWindow();						// Kill Our Current Window
			fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
			// Recreate Our OpenGL Window
			if (!CreateGLWindow("NeHe's Solid Object Tutorial",SCREEN_W,SCREEN_H,16,fullscreen))
			{
				return 0;						// Quit If Window Was Not Created
			}
		}

		

	}
	waveOutPause(hWaveOut);
	waveOutReset(hWaveOut);
	// ** Unprepare our wav header **
	if (waveOutUnprepareHeader(hWaveOut,&WaveHeader,sizeof(WaveHeader)) != MMSYSERR_NOERROR)
	{
		Message("Error unpreparing header!");
		return TRUE;
	}
	// ** close the wav device **
	if (waveOutClose(hWaveOut) != MMSYSERR_NOERROR)
	{
		Message("Sound card cannot be closed!");
		return TRUE;
	} 
	// ** Release our event handle **
	CloseHandle(Done);
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
