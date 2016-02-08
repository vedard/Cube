#ifndef SOUND_H__
#define SOUND_H__
#include "define.h"
#include "vector3.h"
#include "parametre.h"

class Sound
{
public:
	typedef int32 SoundBuffer;
	enum { M9_FIRE, MP5K_FIRE, AK47_FIRE, GUN_DRAW, FLESH_IMPACT, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6, MUSIC1, DROWNING, GASPING, WATERSTEP1, WATERSTEP2,WATERSTEP3, WATERSTEP4
	, DEATH1,DEATH2, DEATH3, DEATH4, DEATH5, DEATH6, DEATH7, DEATH8, DEATH9, LEAVE1,LEAVE2,LEAVE3,LEAVE4,LEAVE5
	};

	static void DeInit();
	static void AddSound(int soundbuffer, std::string path);
	static void Play(int soundbuffer, int volume = 15, const Vector3<float> pos = Vector3<float>(0, 0, 0));
	static void PlayOnce(int soundbuffer);
	static void PlayAndWait(int soundbuffer);
	
private:
	static sf::SoundBuffer * m_SoundBuffers;
	static sf::Sound * m_Sound;
	static int * m_listbuffer;
};
#endif
