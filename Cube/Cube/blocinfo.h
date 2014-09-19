#ifndef BLOCKINFO_H
#define BLOCKINFO_H

#include <iostream>
#include <string>
#include "define.h"

class BlocInfo
{
public:
	BlocInfo(BlocType type, const std::string & nom);
	~BlocInfo();
	BlocType GetType() const;
	void SetDurabilite(int durabilite);
	int GetDurabilite() const;
	void Afficher() const;

private:
	int m_durabilite;
	std::string m_nom;
	BlocType m_type;

};

#endif
