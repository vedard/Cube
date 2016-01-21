#ifndef DEFINE_H__
#define DEFINE_H__

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <GL/glut.h>
#include <unistd.h>

#endif



typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;


#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16
#define WORLD_SIZE 70
#define EDITING_DISTANCE 7
#define NUMBER_OF_BLOCK 20
#define MAX_MONSTER 10
#define MAX_COW 30
#define PI 3.14159265358f
#define MAX_BULLET 500
#define TEXTURE_PATH	"../Cube/media/textures/"
#define SHADER_PATH	"../Cube/media/shaders/"
#define AUDIO_PATH	"../Cube/media/audio/"
#define MODEL_PATH	"../Cube/media/model/"

#define INVENTORY_SIZE 15		//Must be larger than FAST_INVENTORY_SIZE
#define FAST_INVENTORY_SIZE 3	//If changed, modification must be made into the render and the keybinding to apply the changes


typedef uint8 BlockType;
enum BLOCK_TYPE { 
	BTYPE_AIR, 
	BTYPE_GRASS, 
	BTYPE_TEST, 
	BTYPE_STONE, 
	BTYPE_CHEST, 
	BTYPE_WOOD_PLANK, 
	BTYPE_BED_ROCK, 
	BTYPE_DIRT, 
	BTYPE_IRON, 
	BTYPE_COAL, 
	BTYPE_DIAMOND, 
	BTYPE_GOLD, 
	BTYPE_REDSTONE, 
	BTYPE_LAPIS_LAZULI,
	BTYPE_WOOD,
	BTYPE_LEAVE,
	BTYPE_WATER,
	BTYPE_LAVA,
	BTYPE_SAND,
	BTYPE_NETHEREACK
};

enum WEAPONS {
	W_BLOCK,
	W_PISTOL,
	W_SUBMACHINE_GUN,
	W_ASSAULT_RIFLE
};




#endif // DEFINE_H__
