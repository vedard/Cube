#ifndef EXPERIENCE_H__
#define EXPERIENCE_H__

#define EXPERIENCE_LEVEL 1
#define EXPERIENCE_XP 0
#define EXPERIENCE_FILE_NAME "experience.xp"

class Experience
{
	private:
		int m_level = EXPERIENCE_LEVEL;
		int m_xp = EXPERIENCE_XP;
		int m_maxXp;
		int m_xpGain;
	
		void Load();
		void Save();

		void CalculateMaxXp();
		void CalculateXpGain();
	public:
		const int& GetLevel() { return m_level; }
		const int& GetXp() { return m_xp; }
		const int& GetMaxXp() { return m_maxXp; }
		const int& GetXpGain() { return m_xpGain; }

		void GainXp(const int& xpGain);

		Experience();
		~Experience();
};

#endif
