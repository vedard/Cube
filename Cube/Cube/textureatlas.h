#ifndef TEXTUREATLAS_H__
#define TEXTUREATLAS_H__
#include "define.h"
#include <string>
#include <map>
#include <IL/ilu.h>

class TextureAtlas
{
public:
    typedef unsigned int TextureIndex;

public:
    TextureAtlas(unsigned int nbTexture);
    ~TextureAtlas();
    TextureIndex AddTexture(const std::string& fname);
    bool Generate(int textureSize, bool mipmap);

    bool IsValid() const;
    void Bind() const;

    void TextureIndexToCoord(TextureIndex idx, float& u, float& v, float& w, float& h) const;

private:
    bool IsPowerOfTwo(unsigned int x)
    {
        return ((x != 0) && ((x & (~x + 1)) == x));
    }

private:
    struct TextureInfo
    {
        ILuint texId;
        TextureIndex texIdx;

        TextureInfo(ILuint _texId, unsigned int _texIdx) : texId(_texId), texIdx(_texIdx) {}
    };
    // On utilise un std::map pour avoir des valeurs uniques
    typedef std::map<std::string, TextureInfo> TextureList;
    TextureList m_textureList;

    TextureIndex m_currentTextureIndex;
    GLuint  m_textureId;
    bool    m_isValid;
    unsigned int m_nbTexturePerSide;

};

#endif // TEXTUREATLAS_H__
