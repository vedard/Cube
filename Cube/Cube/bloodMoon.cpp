#include "bloodMoon.h"
#include <iostream>

BloodMoon::BloodMoon() : m_isActive(false), m_duration(BLOODMOON_DURATION), m_currTime(0), m_isStarted(false) { }

BloodMoon::~BloodMoon()
{

}

void BloodMoon::Activate() { m_isActive = true; }
void BloodMoon::Deactivate() { m_isActive = false; }
void BloodMoon::Start() { m_isStarted = true; }
void BloodMoon::Stop() { m_isActive = false; }
bool BloodMoon::GetActiveState() const { return m_isActive; }
bool BloodMoon::GetStartedState() const { return m_isStarted; }

void BloodMoon::AddElapsedUnit()
{
	m_currTime++;
}

bool BloodMoon::GetCompletionState()
{
	if (m_currTime >= m_duration) {
		m_isActive = false;
		m_isStarted = false;
		return true;
	}
	else
		return false;
	return false;
}



