#include "engine.h"



Engine::Engine() :
m_wireframe(false),
m_player(WORLD_SIZE / 2 * CHUNK_SIZE_X, 100, WORLD_SIZE / 2 * CHUNK_SIZE_X, 0, 0),
m_shader01(),
m_textureAtlas(7),
m_world()
{
	//Initialisation des touches
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		m_keyboard[i] = false;
	}

	//Creation du tableau de block info
	m_bInfo = new BlockInfo[256];
}

Engine::~Engine()
{
	delete[] m_bInfo;
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
	gluPerspective(75.0f, (float)Width() / (float)Height(), 0.001f, 1000.0f);
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

}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
	//Load texture qui ne sont pas dans l'atlas
	LoadTexture(m_textureSky, TEXTURE_PATH "sky.jpg");
	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");
	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");

	//Load texture dans l'atlas
	m_bInfo[BTYPE_GRASS].Init(BTYPE_GRASS, "Grass");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_grass.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_GRASS].u, m_bInfo[BTYPE_GRASS].v, m_bInfo[BTYPE_GRASS].w, m_bInfo[BTYPE_GRASS].h);

	m_bInfo[BTYPE_TEST].Init(BTYPE_TEST, "Test");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_test.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_TEST].u, m_bInfo[BTYPE_TEST].v, m_bInfo[BTYPE_TEST].w, m_bInfo[BTYPE_TEST].h);

	m_bInfo[BTYPE_STONE].Init(BTYPE_STONE, "Stone");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_stone.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_STONE].u, m_bInfo[BTYPE_STONE].v, m_bInfo[BTYPE_STONE].w, m_bInfo[BTYPE_STONE].h);

	m_bInfo[BTYPE_WOOD_PLANK].Init(BTYPE_WOOD_PLANK, "Wood Plank");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_wood_plank.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_WOOD_PLANK].u, m_bInfo[BTYPE_WOOD_PLANK].v, m_bInfo[BTYPE_WOOD_PLANK].w, m_bInfo[BTYPE_WOOD_PLANK].h);

	m_bInfo[BTYPE_CHEST].Init(BTYPE_CHEST, "Chest");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_chest.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_CHEST].u, m_bInfo[BTYPE_CHEST].v, m_bInfo[BTYPE_CHEST].w, m_bInfo[BTYPE_CHEST].h);

	m_bInfo[BTYPE_BED_ROCK].Init(BTYPE_BED_ROCK, "Bed Rock");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_bed_rock.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_BED_ROCK].u, m_bInfo[BTYPE_BED_ROCK].v, m_bInfo[BTYPE_BED_ROCK].w, m_bInfo[BTYPE_BED_ROCK].h);

	m_bInfo[BTYPE_DIRT].Init(BTYPE_DIRT, "Dirt");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_dirt.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_DIRT].u, m_bInfo[BTYPE_DIRT].v, m_bInfo[BTYPE_DIRT].w, m_bInfo[BTYPE_DIRT].h);


	if (!m_textureAtlas.Generate(128, false))
	{
		std::cout << " Unable to generate texture atlas ..." << std::endl;
		abort();
	}

	std::cout << " Loading and compiling shaders ..." << std::endl;
	if (!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
	{
		std::cout << " Failed to load shader " << std::endl;
		exit(1);
	}

	//Load la map
	m_world.InitMap(8,5,100,15);
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
	GetBlocAtCursor();



	static float gameTime = elapsedTime;

	gameTime += elapsedTime;

	//On met a jour le fps
	if ((int)(gameTime * 100) % 10 == 0)
		m_fps = round(1 / elapsedTime);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Update le player
	m_player.Move(m_keyboard[sf::Keyboard::W], m_keyboard[sf::Keyboard::S], m_keyboard[sf::Keyboard::A], m_keyboard[sf::Keyboard::D], elapsedTime, m_world);
	m_player.ApplyRotation();
	m_player.ApplyTranslation();


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

	glTexCoord2f(0.75, 0.50);		glVertex3f(-512, -512, 512);
	glTexCoord2f(0.75, 0.25);		glVertex3f(-512, 512, 512);
	glTexCoord2f(0.5, 0.25);		glVertex3f(512, 512, 512);
	glTexCoord2f(0.5, 0.50);		glVertex3f(512, -512, 512);

	glTexCoord2f(0.75, 0.25);		glVertex3f(-512, 512, 512);
	glTexCoord2f(0.75, 0.50);		glVertex3f(-512, -512, 512);
	glTexCoord2f(1, 0.50);			glVertex3f(-512, -512, -512);
	glTexCoord2f(1, 0.25);			glVertex3f(-512, 512, -512);

	glTexCoord2f(0.5, 0);			glVertex3f(-512, 512, 512);
	glTexCoord2f(0.25, 0);			glVertex3f(-512, 512, -512);
	glTexCoord2f(0.25, 0.25);		glVertex3f(512, 512, -512);
	glTexCoord2f(0.5, 0.25);		glVertex3f(512, 512, 512);
	glEnd();


	glPopMatrix();

	////Render des chunk

	m_textureAtlas.Bind();

	for (int i = 0; i < WORLD_SIZE; i++)
	{
		for (int j = 0; j < WORLD_SIZE; j++)	//Parcour les chunk
		{
			if (m_world.ChunkAt(i, j).IsDirty())
				m_world.ChunkAt(i, j).Update(m_bInfo);
			m_shader01.Use();
			m_world.ChunkAt(i, j).Render();
			Shader::Disable();
		}
	}

	//Render le hui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DrawHud();
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




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

	//Lctr -> Sneak
	else if (m_keyboard[sf::Keyboard::LControl])
		m_player.SetSneak(true);

	//LSHIFT -> RUN
	else if (m_keyboard[sf::Keyboard::LShift])
		m_player.SetRunning(true);

	//space -> jump
	if (m_keyboard[sf::Keyboard::Space])
		m_player.Jump();

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

	//end sneak
	if (!m_keyboard[sf::Keyboard::LControl])
		m_player.SetSneak(false);

	//end run
	if (!m_keyboard[sf::Keyboard::LShift])
		m_player.SetRunning(false);

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
	if (button == 1 && m_currentBlock.x != -1)
	{
		Vector3<float> chunkPos(floor(m_currentBlock.x / CHUNK_SIZE_X), 0, floor(m_currentBlock.z / CHUNK_SIZE_Z));
		m_world.ChunkAt(chunkPos.x, chunkPos.z).RemoveBloc(m_currentBlock.x - (chunkPos.x * CHUNK_SIZE_X), m_currentBlock.y, m_currentBlock.z - (chunkPos.z * CHUNK_SIZE_X));

	}
	else if (button == 4 && m_currentBlock.x != -1)
	{
		Vector3<float> playerFootPos((int)m_player.Position().x, (int)m_player.Position().y, (int)m_player.Position().z); //Positio des pieds
		Vector3<float> playerEyePos((int)m_player.Position().x, (int)(m_player.Position().y +m_player.GetDimension().y), (int)m_player.Position().z); //Position des yeux
		Vector3<float> newBlocPos(m_currentBlock.x + m_currentFaceNormal.x, m_currentBlock.y + m_currentFaceNormal.y, m_currentBlock.z + m_currentFaceNormal.z); //Position du nouveau block

		//Si il y a pas de collision avec le player
		if (playerFootPos != newBlocPos && playerEyePos != newBlocPos)
		{
			Vector3<float> chunkPos(floor((newBlocPos.x) / CHUNK_SIZE_X), 0, floor((newBlocPos.z) / CHUNK_SIZE_Z));
			m_world.ChunkAt(chunkPos.x, chunkPos.z).PlaceBlock(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X), newBlocPos.y, newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X), BTYPE_WOOD_PLANK);

		}
	}
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

void Engine::DrawHud()
{
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Bind de la texture pour le font
	m_textureFont.Bind();

	std::ostringstream ss;

	ss << "Fps: " << m_fps;
	PrintText(10, Height() - 25, 16, ss.str());


	ss.str("");
	ss << "Position "<< m_player.Position();
	PrintText(10, 30, 16, ss.str());

	ss.str("");
	ss << m_currentBlock;
	PrintText(10, 10, 16, ss.str());

	ss.str("");
	ss << "Health:";
	//Pour chaque 10 point de vie on met un carre sinon un espace
	for (int i = 0; i < m_player.GetHP() / 5; i++)
	{
		ss << (char)254; // Le carractere ■
	}
	for (int i = 0; i < 100 / 5 - m_player.GetHP() / 5; i++)
	{
		ss << " ";
	}
	PrintText((Width() - ss.str().length() * 12) - 10, Height() - 25, 16, ss.str());

	// Affichage du crosshair
	m_textureCrosshair.Bind();
	static const int crossSize = 32;
	glLoadIdentity();
	glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(crossSize, 0);
	glTexCoord2f(1, 1);
	glVertex2i(crossSize, crossSize);
	glTexCoord2f(0, 1);
	glVertex2i(0, crossSize);
	glEnd();

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::PrintText(unsigned int x, unsigned int y, int size, const std::string & t)
{
	glLoadIdentity();
	glTranslated(x, y, 0);
	for (unsigned int i = 0; i < t.length(); ++i)
	{
		float left = (float)((t[i] - 32) % 16) / 16.0f;
		float top = (float)((t[i] - 175) / 16) / 16.0f;

		top += 0.5f;
		glBegin(GL_QUADS);
		glTexCoord2f(left, 1.0f - top - 0.0625f);
		glVertex2f(0, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
		glVertex2f(size, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top);
		glVertex2f(size, size);
		glTexCoord2f(left, 1.0f - top);
		glVertex2f(0, size);
		glEnd();
		glTranslated(size - (size / 4), 0, 0);
	}
}

void Engine::GetBlocAtCursor()
{
	int x = Width() / 2;
	int y = Height() / 2;

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	// Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
	// de valeurs negatives
	int px = (int)(posX);
	int py = (int)(posY);
	int pz = (int)(posZ);

	bool found = false;

	if ((m_player.Position() - Vector3<float>(posX, posY, posZ)).Length() < EDITING_DISTANCE)
	{
		// Apres avoir determine la position du bloc en utilisant la partie entiere du hit
		// point retourne par opengl, on doit verifier de chaque cote du bloc trouve pour trouver
		// le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
		// nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
		// 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
		// A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
		// position 15 par exemple nous retourne plutot la position 15.0001
		for (int x = px - 1; !found && x <= px + 1; ++x)
		{
			for (int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
			{
				for (int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z)
				{
					if (z >= 0)
					{
						BlockType bt = m_world.BlockAt(x, y, z);

						if (bt == BTYPE_AIR)
							continue;

						m_currentBlock.x = x;
						m_currentBlock.y = y;
						m_currentBlock.z = z;

						if (Tool::InRangeWithEpsilon<float>(posX, x, x + 1, 0.05) && Tool::InRangeWithEpsilon<float>(posY, y, y + 1, 0.05) && Tool::InRangeWithEpsilon<float>(posZ, z, z + 1, 0.05))
						{
							found = true;
						}
					}
				}
			}
		}
	}

	if (!found)
	{
		m_currentBlock.x = -1;
	}
	else
	{
		// Find on which face of the bloc we got an hit
		m_currentFaceNormal.Zero();

		// Front et back:
		if (Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z, 0.005f))
			m_currentFaceNormal.z = -1;
		else if (Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z + 1, 0.005f))
			m_currentFaceNormal.z = 1;
		else if (Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x, 0.005f))
			m_currentFaceNormal.x = -1;
		else if (Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x + 1, 0.005f))
			m_currentFaceNormal.x = 1;
		else if (Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y, 0.005f))
			m_currentFaceNormal.y = -1;
		else if (Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y + 1, 0.005f))
			m_currentFaceNormal.y = 1;
	}
}

