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
#define NUMBER_OF_BLOCK 28
#define MAX_MONSTER 0
#define MAX_COW 30
#define PI 3.14159265358f
#define MAX_BULLET 500
#define TEXTURE_PATH	"../Cube/media/textures/"
#define SHADER_PATH	"../Cube/media/shaders/"
#define AUDIO_PATH	"../Cube/media/audio/"
#define MODEL_PATH	"../Cube/media/model/"
#define TICK_DELAY .5f
#define TICK_DELAY_WATER .2f

#define INVENTORY_SIZE 15			//Must be larger than FAST_INVENTORY_SIZE
#define FAST_INVENTORY_SIZE 3		//If changed, modification must be made into the render and the keybinding to apply the changes
#define IS_INVENTORY_CREATIVE true	//If the inventory is in creative mode(infinite everything)
#define FIRST_FAST_INVENTORY_KEY 	sf::Keyboard::Z
#define SECOND_FAST_INVENTORY_KEY 	sf::Keyboard::X
#define THIRD_FAST_INVENTORY_KEY 	sf::Keyboard::C
#define OPEN_CLOSE_INVENTORY_KEY	sf::Keyboard::I

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
	BTYPE_RWATER1,
	BTYPE_RWATER2,
	BTYPE_RWATER3,
	BTYPE_FWATER,
	BTYPE_RLAVA1,
	BTYPE_RLAVA2,
	BTYPE_RLAVA3,
	BTYPE_FLAVA,
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

// Pour savoir sur quel menu nous nous situons
enum SOUS_MENU {
	SM_PRINCIPAL,
	SM_CONTROLS,
	SM_SETTINGS,
	SM_SETTING_CHOICES
};

// Buttons/states du menu principal
enum MENU_PRINCIPAL {
	MP_CONTROLS,
	MP_SETTINGS,
	MP_EXIT_GAME
};

enum MENU_SETTINGS {
	MS_FULLSCREEN,
	MS_WIDTH,
	MS_HEIGHT,
	MS_ANTI_ALIASING,
	MS_VSYNC,
	MS_RENDER_DISTANCE,
	MS_CROSSCOLOR_R,
	MS_CROSSCOLOR_G,
	MS_CROSSCOLOR_B,
	MS_MOUSE_SENSITIVITY
};

enum GAME_WIDTH {
	GW_1920,
	GW_1600,
	GW_1440,
	GW_1366,
	GW_1280
};

enum GAME_HEIGHT {
	GH_1080,
	GH_1024,
	GH_960,
	GH_900,
	GH_768
};

#endif // DEFINE_H__
