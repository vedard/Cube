#include "engine.h"
#include <iostream>
#include <algorithm>
#include <cmath>



Engine::Engine() : m_wireframe(false), m_player(0, 1.8, 0, 0, 0)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable( GL_TEXTURE_2D );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0f, (float)Width() / (float)Height(), 0.1f, 1000.0f);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable (GL_LINE_SMOOTH);

	glEnable(GL_CULL_FACE);

    // Light
    GLfloat light0Pos[4]  = {0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f};
    GLfloat light0Amb[4]  = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat light0Diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0Spec[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

    CenterMouse();
    HideCursor();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
	LoadTexture(m_textureFloor, TEXTURE_PATH "gazon.jpg");
	LoadTexture(m_textureWall, TEXTURE_PATH "wall.jpg");
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
    static float gameTime = elapsedTime;

    gameTime += elapsedTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations initiales
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//Update le player
	m_player.Move(0,0,0,0, elapsedTime);
	m_player.ApplyRotation();
	m_player.ApplyTranslation();

	//Cube
	glPushMatrix();
	glTranslatef(0, 0, -10);
	glRotatef(gameTime * 100.f, 0, 1, 0);
	glTranslatef(-0.5, 0, 0.5);

	m_textureWall.Bind();
	glBegin(GL_QUADS);

		//face
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3f(1, 0, 0);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(0, 1, 0);
		//droite
		glTexCoord2f(0, 0);
		glVertex3f(1, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3f(1, 0, -1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, -1);
		glTexCoord2f(0, 1);
		glVertex3f(1, 1, 0);

		//gauche
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(0, 1);
		glVertex3f(0, 1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(0, 1, -1);
		glTexCoord2f(1, 0);
		glVertex3f(0, 0, -1);

		//deriere
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, -1);
		glTexCoord2f(0, 1);
		glVertex3f(0, 1, -1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, -1);
		glTexCoord2f(1, 0);
		glVertex3f(1, 0, -1);

		//Haut
		glTexCoord2f(0, 0);
		glVertex3f(0, 1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(1, 1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, -1);
		glTexCoord2f(0, 1);
		glVertex3f(0, 1, -1);

		//Bas
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2f(0, 1);
		glVertex3f(0, 0, -1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 0, -1);
		glTexCoord2f(1, 0);
		glVertex3f(1, 0, 0);

	glEnd();
	glPopMatrix();

    // Plancher
    m_textureFloor.Bind();
    float nbRep = 50.f;
    glBegin(GL_QUADS);
	glNormal3f(0.f, 1.f, 0.f);            // Normal vector
	glTexCoord2f(0.f, 0.f);
    glVertex3f(-100.f, 0.f, 100.f);
    glTexCoord2f(nbRep, 0);
    glVertex3f(100.f, 0.f, 100.f);
    glTexCoord2f(nbRep, nbRep);
    glVertex3f(100.f, 0.f, -100.f);
    glTexCoord2f(0, nbRep);
    glVertex3f(-100.f, 0.f, -100.f);
    glEnd();
}

void Engine::KeyPressEvent(unsigned char key)
{
    switch(key)
    {
    case 36:	// ESC
        Stop();
        break;
    case 94:	// F10
        SetFullscreen(!IsFullscreen());
        break;
    default:
        std::cout << "Unhandled key: " << (int)key << std::endl;
    }

}

void Engine::KeyReleaseEvent(unsigned char key)
{
    switch(key)
    {
    case 24:       // Y
        m_wireframe = !m_wireframe;
        if(m_wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }
}

void Engine::MouseMoveEvent(int x, int y)
{
	// Centrer la souris seulement si elle n'est pas déjà centrée
	// Il est nécessaire de faire la vérification pour éviter de tomber
	// dans une boucle infinie où l'appel à CenterMouse génère un
	// MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre 
	// MouseMoveEvent, etc
	if(x == (Width() / 2) && y == (Height() / 2))
		return;
    CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
    texture.Load(filename);
    if(!texture.IsValid())
    {
        std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
        if(stopOnError)
            Stop();

        return false;
    }

    return true;
}

