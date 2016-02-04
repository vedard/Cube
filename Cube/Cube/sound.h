#ifndef SOUND_H__
#define SOUND_H__
#include "define.h"
#include "vector3.h"
#include "parametre.h"

class Sound
{
public:
	typedef int32 SoundBuffer;
	enum { M9_FIRE, MP5K_FIRE, AK47_FIRE, GUN_DRAW, FLESH_IMPACT, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6, MUSIC1, DROWNING, GASPING};

	static void DeInit();
	static void AddSound(int soundbuffer, std::string path);
	static void Play(int soundbuffer, int volume = 15, const Vector3<float> pos = Vector3<float>(0, 0, 0));
	static void PlayOnce(int soundbuffer);
	
private:
	static sf::SoundBuffer * m_SoundBuffers;
	static sf::Sound * m_Sound;
	static int * m_listbuffer;
};
#endif
