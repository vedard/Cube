#include "blockinfo.h"
#include <iostream>

BlockInfo::BlockInfo(BlockType type, const std::string& nom) : m_type(type), m_nom(nom), m_durabilite(1)
{
}

BlockInfo::~BlockInfo()
{
}

BlockType BlockInfo::GetType() const
{
    return m_type;
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


