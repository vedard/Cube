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
#define WORLD_SIZE 45
#define DAY_TIME 13
#define EDITING_DISTANCE 7
#define NUMBER_OF_BLOCK 31
#define NUMBER_OF_BLOCK 31
#define MAX_SPRINTER 3
#define MAX_CREEPER 15
#define MAX_BULLET 500
#define MAX_COW 8
#define MAX_BEAR 3
#define MAX_CHICKEN 30
#define MAX_DRAGON 1
#define MAX_BIRD 28
#define MONSTER_MULTIPLIER 5
#define BLOODMOON_PROBABILITY 1 // Plus haut c'est, moins t'as de chances. à 1 t'as 100%, a 2 t'as 50%. etc
#define BLOODMOON_DURATION 2500 // Doit toujours rester 2500#define PI 3.14159265358f
#define INVULNERABILITY_PLAYER_TIME 1
#define MAX_TRAMPOLINE_JUMP 10
#define PI 3.14159265359

#define TEXTURE_PATH	"../Cube/media/textures/"
#define EFFECTS_PATH	"../Cube/media/effects/"
#define SHADER_PATH	"../Cube/media/shaders/"
#define AUDIO_PATH	"../Cube/media/audio/"
#define MUSIC_PATH	"../Cube/media/audio/music/"
#define WEAPONS_PATH	"../Cube/media/audio/weapons/"
#define WALK_PATH	"../Cube/media/audio/walk/"
#define HURT_PATH	"../Cube/media/audio/hurt/"
#define DEATH_PATH  "../Cube/media/audio/death/"
#define LEAVE_PATH	"../Cube/media/audio/leave/"
#define EFFECT_PATH	"../Cube/media/audio/effect/"
#define SPRINTER_PATH	"../Cube/media/audio/sprinter/"


#define MODEL_PATH	"../Cube/media/model/"
#define TICK_DELAY .100f


#define HURT_TIME 20
#define IS_DYING_LENGTH 3
#define GUN_NUMBER 5


#define FAST_INVENTORY_SIZE 3		//If changed, modification must be made into the render and the keybinding to apply the changes
#define INVENTORY_SIZE 15
#define IS_INVENTORY_CREATIVE false	//If the inventory is in creative mode(infinite everything)
#define FIRST_FAST_INVENTORY_KEY 	sf::Keyboard::Z
#define SECOND_FAST_INVENTORY_KEY 	sf::Keyboard::X
#define THIRD_FAST_INVENTORY_KEY 	sf::Keyboard::C
#define OPEN_CLOSE_INVENTORY_KEY	sf::Keyboard::I
#define INVENTORY_MOVE_ITEM_KEY		sf::Keyboard::B
#define INVENTORY_DELETE_ITEM_KEY	sf::Keyboard::E

#define MIN_WIDTH 800  // Valeurs subjectives pour ne pas pouvoir mettre le jeu trop petit dans le menu et ne plus pouvoir rien faire
#define MIN_HEIGHT 600
#define MAX_VOLUME 100 // Valeurs du sf::Sound
#define MIN_VOLUME 0


#define KEY_ALREADY_BOUND "This key is already bound"
#define KEY_BINDED_SUCCESSFULLY "Key was binded succesfully"

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
	BTYPE_NETHEREACK,
	BTYPE_TRAMPOLINE,
	BTYPE_TAPIS,
	BTYPE_TRAP
};

enum WEAPONS {
	W_BLOCK,
	W_PISTOL,
	W_SUBMACHINE_GUN,
	W_ASSAULT_RIFLE,
	W_SNIPER,
	W_SHOTGUN
};

// Pour savoir sur quel menu nous nous situons
enum SOUS_MENU {
	SM_PRINCIPAL,
	SM_CONTROLS,
	SM_SETTINGS,
	SM_SETTING_SELECTED,
	SM_CONTROL_SELECTED,
	SM_MULTIPLAYER
};

// Buttons/states du menu principal
enum MENU_PRINCIPAL {
	MP_CONTROLS,
	MP_SETTINGS,
	MP_MULTIPLAYER,
	MP_EXIT_GAME
};

enum MENU_SETTINGS {
	MS_FULLSCREEN,
	MS_WIDTH,
	MS_HEIGHT,
	MS_ANTI_ALIASING,
	MS_VSYNC,
	MS_RENDER_DISTANCE,
	MS_SOUND_VOLUME,
	MS_MUSIC_VOLUME,
	MS_CROSSCOLOR_R,
	MS_CROSSCOLOR_G,
	MS_CROSSCOLOR_B,
	MS_MOUSE_SENSITIVITY
};
enum ANIMAL_TYPE
{
	A_BEAR,
	A_COW,
	A_CHICKEN,
	A_DRAGON,
	A_BIRD
};

enum MENU_CONTROLS {
	MC_AVANCER,
	MC_GAUCHE,
	MC_RECULER,
	MC_DROITE,
	MC_FULLSCREEN,
	MC_INFO,
	MC_CROUCH,
	MC_RUN,
	MC_JUMP,
	MC_NOCLIP,
	MC_INVENTORY1,
	MC_INVENTORY2,
	MC_INVENTORY3,
	MC_INVENTORY4,
	MC_INVENTORY,
	MC_SPAWNMONSTER,
	MC_WIREFRAME
};

enum MONSTER_TYPE {
	M_CREEPER,
	M_SPRINTER
};

#endif // DEFINE_H__
