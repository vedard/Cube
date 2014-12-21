#ifndef DEFINE_H__
#define DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>


#ifdef _WIN32
#include <windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#endif

#ifndef _WIN32
#include <GL/glut.h>
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;


#define CHUNK_SIZE_X 8
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 8
#define WORLD_SIZE 20
#define RENDER_DISTANCE 14
#define EDITING_DISTANCE 7
#define NUMBER_OF_BLOCK 18
#define PI 3.14159265358f


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
	BTYPE_SAND
};


#define TEXTURE_PATH	"../Cube/media/textures/"
#define SHADER_PATH		"../Cube/media/shaders/"
#define MOUSE_SENSIBILITY 1/4

#endif // DEFINE_H__
