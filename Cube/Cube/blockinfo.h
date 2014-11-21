#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <string>
#include "define.h"
#include "textureatlas.h"

class BlockInfo
{
public:
	BlockInfo();
	BlockInfo(BlockType type, const std::string& nom);
	~BlockInfo();

	void Init(BlockType type, const std::string& nom);

	BlockType GetType() const;

	void SetDurabilite(int durabilite);
	int GetDurabilite() const;

	void Afficher() const;

private:
	//Type
	BlockType m_type;
	std::string m_nom;
	int m_durabilite;

public:
	//Position texture dans l'atlas
	float u, v, w, h;


};

#endif // BLOCKINFO_H__
