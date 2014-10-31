#include "engine.h"
#include <iostream>
#include <algorithm>
#include <cmath>



Engine::Engine() : m_wireframe(false), m_player(0, 1.7, 0, 0, 0), m_testChunk(), m_shader01()
{
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		m_keyboard[i] = false;
	}
}

Engine::~Engine()
{
}

void Engine::Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << " Error while initializing glew .. abording (" << glewGetErrorString(err) << ")" << std::endl;
		abort();
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0f, (float)Width() / (float)Height(), 0.1f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);


	glEnable(GL_CULL_FACE);

	// Light
	GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f };
	GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

	CenterMouse();
	HideCursor();

	

	//Creation des chunk

	for (int x = 0; x < CHUNK_SIZE_X; ++x)
	{
		for (int z = 0; z < CHUNK_SIZE_Z; ++z)
		{
			for (int y = 0; y < 32; ++y)
			{
				if (x % 2 == 0 && y % 2 == 0 && z % 2 == 0)
					m_testChunk.SetBlock(x, y, z, BTYPE_DIRT);

				
			}
		}
	}
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
	LoadTexture(m_textureGrass, TEXTURE_PATH "block_grass.bmp");
	LoadTexture(m_textureChecker, TEXTURE_PATH "checker.bmp");
	LoadTexture(m_textureTest, TEXTURE_PATH "block_test.bmp");
	LoadTexture(m_textureSky, TEXTURE_PATH "sky.jpg");

	std::cout << " Loading and compiling shaders ..." << std::endl;
	if (!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
	{
		std::cout << " Failed to load shader " << std::endl;
		exit(1);
	}
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
	m_player.Move(m_keyboard[sf::Keyboard::W], m_keyboard[sf::Keyboard::S], m_keyboard[sf::Keyboard::A], m_keyboard[sf::Keyboard::D], m_keyboard[sf::Keyboard::LShift], elapsedTime);
	m_player.ApplyRotation();
	m_player.ApplyTranslation();

	// Plancher
	m_textureChecker.Bind();
	float nbRep = 130.f;
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


	//Ciel
	glPushMatrix();
	glRotatef(gameTime * 1.2, 0, 1, 0);
	m_textureSky.Bind();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0.50);			glVertex3f(-512, -512, -512);
	glTexCoord2f(0.25, 0.50);		glVertex3f(512, -512, -512);
	glTexCoord2f(0.25, 0.25);		glVertex3f(512, 512, -512);
	glTexCoord2f(0, 0.25);			glVertex3f(-512, 512, -512);

	glTexCoord2f(0.25, 0.25);		glVertex3f(512, 512, -512);
	glTexCoord2f(0.25, 0.5);		glVertex3f(512, -512, -512);
	glTexCoord2f(0.5, 0.5);			glVertex3f(512, -512, 512);
	glTexCoord2f(0.5, 0.25);		glVertex3f(512, 512, 512);

	glTexCoord2f(0.75, 0.50);			glVertex3f(-512, -512, 512);
	glTexCoord2f(0.75, 0.25);			glVertex3f(-512, 512, 512);
	glTexCoord2f(0.5, 0.25);		glVertex3f(512, 512, 512);
	glTexCoord2f(0.5, 0.50);		glVertex3f(512, -512, 512);

	glTexCoord2f(0.75, 0.25);		glVertex3f(-512, 512, 512);
	glTexCoord2f(0.75, 0.50);		glVertex3f(-512, -512, 512);
	glTexCoord2f(1, 0.50);			glVertex3f(-512, -512, -512);
	glTexCoord2f(1, 0.25);		glVertex3f(-512, 512, -512);

	glTexCoord2f(0.5, 0);		glVertex3f(-512, 512, 512);
	glTexCoord2f(0.25, 0);		glVertex3f(-512, 512, -512);
	glTexCoord2f(0.25, 0.25);			glVertex3f(512, 512, -512);
	glTexCoord2f(0.5, 0.25);		glVertex3f(512, 512, 512);
	glEnd();


	glPopMatrix();



	m_textureGrass.Bind();
	for (int i = 0; i < 2; i++)
	{
		if (m_testChunk.IsDirty())
			m_testChunk.Update();
		m_shader01.Use();
		m_testChunk.Render();
		Shader::Disable();
	}

}

void Engine::KeyPressEvent(unsigned char key)
{
	//update le teableau
	m_keyboard[key] = true;

	//Esc -> Arrete le programme
	if (m_keyboard[36])
		Stop();

	//f10 -> toggle fulscreen mode
	else if (m_keyboard[94])
		SetFullscreen(!IsFullscreen());
	//V -> toogle noclip mode
	else if (m_keyboard[sf::Keyboard::V])
		m_player.ToggleNoClip();

	//y -> toggle wireframe mode
	else if (m_keyboard[24])
	{
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

void Engine::KeyReleaseEvent(unsigned char key)
{
	//update le teableau
	m_keyboard[key] = false;

}

void Engine::MouseMoveEvent(int x, int y)
{
	// Centrer la souris seulement si elle n'est pas déjà centrée
	// Il est nécessaire de faire la vérification pour éviter de tomber
	// dans une boucle infinie où l'appel à CenterMouse génère un
	// MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre 
	// MouseMoveEvent, etc
	if (x == (Width() / 2) && y == (Height() / 2))
		return;
	CenterMouse();

	float relativeX = x - Width() / 2;
	float relativeY = y - Height() / 2;

	m_player.TurnLeftRight(relativeX * MOUSE_SENSIBILITY);
	m_player.TurnTopBottom(relativeY * MOUSE_SENSIBILITY);

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
	if (!texture.IsValid())
	{
		std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
		if (stopOnError)
			Stop();

		return false;
	}

	return true;
}

