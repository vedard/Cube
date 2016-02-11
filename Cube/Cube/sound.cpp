#include "sound.h"

sf::SoundBuffer * Sound::m_SoundBuffers = new sf::SoundBuffer[400];
sf::Sound * Sound::m_Sound = new sf::Sound[200];
int * Sound::m_listbuffer = new int[200];



void Sound::AddSound(int soundbuffer, std::string path)
{
	if (!m_SoundBuffers[soundbuffer].loadFromFile(path))
		std::cout << "Could not load: " << path << std::endl;
}

void Sound::Play(int soundbuffer, int volume, const Vector3<float> pos)
{

	Parametre& m_settings = Parametre::GetInstance();
	int volumeSound = m_settings.m_soundvolume;
	if (volume > m_settings.m_soundvolume)
		volumeSound = volume;
	for (int i = 0; i < 200; i++)
	{
		if (m_Sound[i].getStatus() == sf::Sound::Status::Stopped)
		{
			m_Sound[i].setBuffer(m_SoundBuffers[soundbuffer]);
			m_Sound[i].setVolume((float)volumeSound);
			m_Sound[i].setPosition(pos.x, pos.y, pos.z);
			m_Sound[i].play();
			m_listbuffer[i] = soundbuffer;
			break;
		}
	}
}
void Sound::PlayOnce(int soundbuffer)
{
	Parametre& m_settings = Parametre::GetInstance();
	bool alreadyplaying = false;
	for (int i = 0; i < 200; i++)
	{
		if (m_listbuffer[i] == soundbuffer && m_Sound[i].getStatus() == sf::Sound::Status::Playing)
		{
			alreadyplaying = true;
			break;
		}
	}
	if (!alreadyplaying)
	{
		for (int i = 0; i < 200; i++)
		{
			if (m_Sound[i].getStatus() == sf::Sound::Status::Stopped)
			{
				m_Sound[i].setBuffer(m_SoundBuffers[soundbuffer]);
				m_Sound[i].setVolume((float)m_settings.m_soundvolume);
				m_Sound[i].play();
				m_listbuffer[i] = soundbuffer;
				break;
			}
		}
	}
}

void Sound::DeInit()
{
	delete[] m_Sound;
	delete[] m_SoundBuffers;
	delete[] m_listbuffer;
}
