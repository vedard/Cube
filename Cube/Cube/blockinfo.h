#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <string>
#include "define.h"

class BlockInfo
{
    public:
    BlockInfo(BlockType type, const std::string& nom);
    ~BlockInfo();

    BlockType GetType() const;

    void SetDurabilite(int durabilite);
    int GetDurabilite() const;

    void Afficher() const;

    private:
    BlockType m_type;
    std::string m_nom;
    int m_durabilite;

};

#endif // BLOCKINFO_H__
