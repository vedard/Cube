#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "player.h"

class Engine : public OpenglContext
{
public:
    Engine();
    virtual ~Engine();
    virtual void Init();
    virtual void DeInit();
    virtual void LoadResource();
    virtual void UnloadResource();
    virtual void Render(float elapsedTime);
    virtual void KeyPressEvent(unsigned char key);
    virtual void KeyReleaseEvent(unsigned char key);
    virtual void MouseMoveEvent(int x, int y);
    virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
    virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

private:
    bool m_wireframe;

	Player m_player;

	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier

    Texture m_textureFloor;
	Texture m_textureWall;
};

#endif // ENGINE_H__
