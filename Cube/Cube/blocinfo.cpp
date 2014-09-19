#include "blocinfo.h"

BlocInfo::BlocInfo(BlocType type, const std::string & nom) : m_type(type), m_nom(nom)
{
	m_durabilite = 3;
}

BlocInfo::~BlocInfo()
{
}

BlocType BlocInfo::GetType() const
{
	return m_type;
}

void BlocInfo::SetDurabilite(int durabilite)
{
	m_durabilite = durabilite;
}

int BlocInfo::GetDurabilite() const
{
	return m_durabilite;
}

void BlocInfo::Afficher() const
{
	std::cout
		<< "Nom: " << m_nom
		<< " Type: " << m_type
		<< " Durabilite: " << m_durabilite << std::endl;
}