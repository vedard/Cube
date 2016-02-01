#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <string>
#include "define.h"
#include "textureatlas.h"
#include "item.h"

class BlockInfo 
{
public:
	BlockInfo();
	BlockInfo(BlockType type, const std::string& nom);
	~BlockInfo();

	void Init(BlockType type, const std::string& nom);

	BlockType GetType() const;
	std::string GetName() const;

	void SetDurabilite(int durabilite);
	int GetDurabilite() const;

	void SetHauteur(float hauteur);
	float GetHauteur() const;

	void Afficher() const;

	
	friend std::ostream& operator<<(std::ostream& o, const BlockInfo& v);

private:
	//Type
	BlockType m_type;
	std::string m_nom;
	int m_durabilite;
	float m_hauteur;

public:
	//Position texture dans l'atlas
	float u, v, w, h;


};

#endif // BLOCKINFO_H__
