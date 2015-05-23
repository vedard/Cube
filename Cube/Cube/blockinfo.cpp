#include "blockinfo.h"
#include <iostream>

BlockInfo::BlockInfo()
{
	Init(BTYPE_TEST, "Test");
}

BlockInfo::BlockInfo(BlockType type, const std::string& nom) : m_type(type), m_nom(nom), m_durabilite(1)
{
}

BlockInfo::~BlockInfo()
{
	
}

void BlockInfo::Init(BlockType type, const std::string& nom)
{
	m_type = type;
	m_nom = nom;
	m_durabilite = 1;
}

BlockType BlockInfo::GetType() const
{
    return m_type;
}
std::string BlockInfo::GetName() const
{
	return m_nom;
}

void BlockInfo::SetDurabilite(int durabilite)
{
    m_durabilite = durabilite;
}

int BlockInfo::GetDurabilite() const
{
    return m_durabilite;
}

void BlockInfo::Afficher() const
{
    std::cout << "Type: " << m_type << std::endl;
    std::cout << "Nom: " << m_nom << std::endl;
    std::cout << "Durabilite: " << m_durabilite << std::endl;
}

std::ostream& operator<<(std::ostream& o, const BlockInfo& v)
{
	return o << v.m_nom;
}
