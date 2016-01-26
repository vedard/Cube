#include "sound.h"

sf::SoundBuffer * Sound::m_SoundBuffers = new sf::SoundBuffer[400];
sf::Sound * Sound::m_Sound = new sf::Sound[200];


void Sound::AddSound(int soundbuffer, std::string path)
{
	if (!m_SoundBuffers[soundbuffer].loadFromFile(path))
		std::cout << "Could not load: " << path << std::endl;
}

void Sound::Play(int soundbuffer, int volume, const Vector3<float> pos)
{
	for (int i = 0; i < 256; i++)
	{
		
		if (m_Sound[i].getStatus() == sf::Sound::Status::Stopped)
		{
			m_Sound[i].setBuffer(m_SoundBuffers[soundbuffer]);
			m_Sound[i].setVolume((float)volume);
			m_Sound[i].setPosition(pos.x, pos.y, pos.z);
			m_Sound[i].play();
			break;
		}
	}
}

void Sound::DeInit()
{
	delete[] m_Sound;
	delete[] m_SoundBuffers;
}
