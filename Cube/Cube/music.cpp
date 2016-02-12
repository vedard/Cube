#include "music.h"

Music Music::m_instance = Music();

Music::Music()
{

}

Music::~Music()
{
}

Music& Music::GetInstance()
{
	return m_instance;
}

void Music::PlayNext()
{
	if (m_music.getStatus() == sf::Sound::Status::Playing)
	{

	}
	else
	{
		if (!m_music.openFromFile(MUSIC_PATH "music" + std::to_string(m_index + 1) + ".wav")) {
			m_index = 0;
			PlayNext();
		}
		m_music.setLoop(false);
		m_music.setVolume(m_settings.m_musicvolume);
		m_music.play();
		m_index++;
	}
}

void Music::setVolume(int volume)
{
	m_music.setVolume(volume);
}

void Music::getMusic()
{

}
