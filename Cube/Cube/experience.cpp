#include "experience.h"
#include "openglcontext.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

// private:

void Experience::Load()
{
	std::ifstream ifs(EXPERIENCE_FILE_NAME);
	std::istringstream iss;
	std::string key, value;
	std::map<std::string, std::string> m_data;
	
	if(ifs.good())
	{
		m_data.clear();
		m_data["level"] = "";
		m_data["xp"] = "";

		for(std::string line; std::getline(ifs, line); )
		{
			iss.str(line);
			iss >> key >> value;
			m_data[key] = value;
			iss.clear();	
		}

		m_level = stoi(m_data["level"]);
		CalculateMaxXp();
		CalculateXpGain();
		GainXp(stoi(m_data["xp"]));
	}
	ifs.close();
	Save();
}
void Experience::Save()
{
	std::ofstream ofs(EXPERIENCE_FILE_NAME);

	ofs << "level " << m_level << "\n";
	ofs << "xp " << m_xp << "\n";

	ofs.close();
}

void Experience::CalculateMaxXp()
{
	m_maxXp = ceil(pow(m_level + 1, 3));
}
void Experience::CalculateXpGain()
{
	m_xpGain = ceil(pow(m_level, 1.5));
}

// public:

void Experience::GainXp(const int& xpGain)
{
	m_xp += xpGain;

	while(m_xp >= m_maxXp)
	{
		m_xp -= m_maxXp;
		m_level++;
		CalculateXpGain();
		CalculateMaxXp();
	}
}

Experience::Experience()
{
	Load();
}
Experience::~Experience()
{
	Save();
}
