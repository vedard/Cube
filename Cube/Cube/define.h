#ifndef DEFINE_H__
#define DEFINE_H__

#include <SFML/Window.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include <gl/GL.h>
#include <gl/GLU.h>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;


#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define PI 3.14159265358

enum BlockType {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS};


#define TEXTURE_PATH        "../Cube/media/textures/"
#define VIEW_DISTANCE       128
#define MOUSE_SENSIBILITY 1/20

#endif // DEFINE_H__
