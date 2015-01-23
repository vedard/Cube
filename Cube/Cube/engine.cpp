#include "engine.h"
#define NBR_MONSTER 0


Engine::Engine() :
m_wireframe(false),
m_player(),
m_shader01(),
m_textureAtlas(NUMBER_OF_BLOCK - 1),
m_world(),
m_currentBlock(-1, -1, -1),
displayInfo(false)
{
	//Initialisation des touches
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		m_keyboard[i] = false;
	}

	//Creation du tableau de block info
	m_bInfo = new BlockInfo[256];

	m_monster = new Monster[NBR_MONSTER];
}

Engine::~Engine()
{
	m_world.SaveMap("map.sav");
	delete[] m_bInfo;
	delete[] m_monster;
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
	gluPerspective(75.0f, (float)Width() / (float)Height(), 0.01f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


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

	m_bInfo[BTYPE_IRON].Init(BTYPE_IRON, "Iron");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_iron.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_IRON].u, m_bInfo[BTYPE_IRON].v, m_bInfo[BTYPE_IRON].w, m_bInfo[BTYPE_IRON].h);

	m_bInfo[BTYPE_COAL].Init(BTYPE_COAL, "Coal");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_coal.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_COAL].u, m_bInfo[BTYPE_COAL].v, m_bInfo[BTYPE_COAL].w, m_bInfo[BTYPE_COAL].h);

	m_bInfo[BTYPE_DIAMOND].Init(BTYPE_DIAMOND, "Diamond");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_diamond.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_DIAMOND].u, m_bInfo[BTYPE_DIAMOND].v, m_bInfo[BTYPE_DIAMOND].w, m_bInfo[BTYPE_DIAMOND].h);

	m_bInfo[BTYPE_GOLD].Init(BTYPE_GOLD, "Gold");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_gold.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_GOLD].u, m_bInfo[BTYPE_GOLD].v, m_bInfo[BTYPE_GOLD].w, m_bInfo[BTYPE_GOLD].h);

	m_bInfo[BTYPE_REDSTONE].Init(BTYPE_REDSTONE, "Redstone");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_redstone.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_REDSTONE].u, m_bInfo[BTYPE_REDSTONE].v, m_bInfo[BTYPE_REDSTONE].w, m_bInfo[BTYPE_REDSTONE].h);

	m_bInfo[BTYPE_LAPIS_LAZULI].Init(BTYPE_LAPIS_LAZULI, "Lapis Lazuli");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_lapis_lazuli.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_LAPIS_LAZULI].u, m_bInfo[BTYPE_LAPIS_LAZULI].v, m_bInfo[BTYPE_LAPIS_LAZULI].w, m_bInfo[BTYPE_LAPIS_LAZULI].h);

	m_bInfo[BTYPE_WOOD].Init(BTYPE_WOOD, "Wood");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_wood.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_WOOD].u, m_bInfo[BTYPE_WOOD].v, m_bInfo[BTYPE_WOOD].w, m_bInfo[BTYPE_WOOD].h);

	m_bInfo[BTYPE_LEAVE].Init(BTYPE_LEAVE, "Leave");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_leave.png");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_LEAVE].u, m_bInfo[BTYPE_LEAVE].v, m_bInfo[BTYPE_LEAVE].w, m_bInfo[BTYPE_LEAVE].h);

	m_bInfo[BTYPE_WATER].Init(BTYPE_WATER, "Water");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_water.png");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_WATER].u, m_bInfo[BTYPE_WATER].v, m_bInfo[BTYPE_WATER].w, m_bInfo[BTYPE_WATER].h);

	m_bInfo[BTYPE_SAND].Init(BTYPE_SAND, "Sand");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_sand.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_SAND].u, m_bInfo[BTYPE_SAND].v, m_bInfo[BTYPE_SAND].w, m_bInfo[BTYPE_SAND].h);

	m_bInfo[BTYPE_NETHEREACK].Init(BTYPE_NETHEREACK, "Netherrack");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_netherrack.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_NETHEREACK].u, m_bInfo[BTYPE_NETHEREACK].v, m_bInfo[BTYPE_NETHEREACK].w, m_bInfo[BTYPE_NETHEREACK].h);

	m_bInfo[BTYPE_LAVA].Init(BTYPE_LAVA, "Lava");
	m_texBlockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "block_lava.bmp");
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[BTYPE_LAVA].u, m_bInfo[BTYPE_LAVA].v, m_bInfo[BTYPE_LAVA].w, m_bInfo[BTYPE_LAVA].h);

	if (!m_textureAtlas.Generate(64, false))
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
	m_world.LoadMap("map.sav", m_bInfo);
	m_world.SetUpdateDistance(m_renderDistance);
	m_world.InitChunks(WORLD_SIZE / 2, WORLD_SIZE / 2);
	

	m_player.SetName("Player 1");
	m_player.Spawn(m_world, WORLD_SIZE*CHUNK_SIZE_X / 2, WORLD_SIZE*CHUNK_SIZE_X / 2);

	for (int i = 0; i < NBR_MONSTER; i++)
	{
		m_monster[i].SetName("Monster " + std::to_string(i + 1));
		m_monster[i].Spawn(m_world, (WORLD_SIZE*CHUNK_SIZE_X / 2) - 50 + rand() % 100, (WORLD_SIZE*CHUNK_SIZE_X / 2) - 50 + rand() % 100);
		m_monster[i].SetTarget(&m_player);
	}

}

void Engine::UnloadResource()
{

}

void Engine::Render(float elapsedTime)
{

	static float gameTime = elapsedTime;
	static float nextGameUpdate = gameTime;

	GetBlocAtCursor();

	gameTime += elapsedTime;

	//On met a jour le fps
	if ((int)(gameTime * 100) % 10 == 0)
		m_fps = (int)round(1.f / elapsedTime);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int loops = 0;

	//Lock les mouvements a 50 fps
	while (gameTime > nextGameUpdate && loops < 10)
	{
		//Update le player
		m_player.Move(m_keyboard[sf::Keyboard::W], m_keyboard[sf::Keyboard::S], m_keyboard[sf::Keyboard::A], m_keyboard[sf::Keyboard::D], m_world);

		for (int i = 0; i < NBR_MONSTER; i++)
			m_monster[i].Move(m_world);


		//1 / 0.02 = 50 fps
		nextGameUpdate += 0.02f;
		loops++;
	}

	//Place le joueur au centre du monde
	m_player.ApplyRotation();
	m_player.ApplyTranslation();

	//Activation des shaders
	m_shader01.Use();
	glUniform1f(glGetUniformLocation(m_shader01.m_program, "gameTime"), gameTime);
	glUniform1f(glGetUniformLocation(m_shader01.m_program, "underwater"), m_player.Underwater());

	//Ciel
	if (m_player.GetPosition().y > 64)
	{
		glPushMatrix();
		glTranslatef(m_player.GetPosition().x, 0, m_player.GetPosition().z);

		glRotatef(gameTime * 1.1f, 0.f, 1.f, 0.f);

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
	}

	////Chunk
	m_textureAtlas.Bind();

	//Chunk du joueur
	Vector3<int> playerPos((int)m_player.GetPosition().x / CHUNK_SIZE_X, 0, (int)m_player.GetPosition().z / CHUNK_SIZE_Z);

	//m_world.InitChunks(playerPos.x, playerPos.z);
	std::thread t(&World::InitChunks, &m_world, playerPos.x, playerPos.z);
	t.detach();

	//Update les chunk autour du joueur si il sont dirty
	m_world.Update(playerPos.x, playerPos.z, m_bInfo);

	//std::thread a(&World::Update, &m_world, playerPos.x, playerPos.z, m_bInfo);
	//a.join();

	m_chunkToUpdate = m_world.ChunkNotUpdated(playerPos.x, playerPos.z);
	m_world.Render(playerPos.x, playerPos.z, m_shader01.m_program);

	//Monstre
	for (int i = 0; i < NBR_MONSTER; i++)
		m_monster[i].Draw(false);



	//Block focused
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(m_currentBlock.x , m_currentBlock.y , m_currentBlock.z );


	glBegin(GL_LINE_LOOP);
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glNormal3f(0.f, 1.f, 0.f);

	if (m_currentFaceNormal.z == -1)
	{
		glVertex3f(0, 0, 0 - 0.01);
		glVertex3f(0, 0.99, 0 - 0.01);
		glVertex3f(0.99, 0.99, 0 - 0.01);
		glVertex3f(0.99, 0, 0 - 0.01);
	}
	else if (m_currentFaceNormal.x == 1)
	{
		glVertex3f(1 + 0.01, 0.99, 0);
		glVertex3f(1 + 0.01, 0.99, 0.99);
		glVertex3f(1 + 0.01, 0, 0.99);
		glVertex3f(1 + 0.01, 0, 0);
	}
	else if (m_currentFaceNormal.z == 1)
	{
		glVertex3f(0, 0, 1 + 0.01 + 0.01);
		glVertex3f(0.99, 0, 1 + 0.01 + 0.01);
		glVertex3f(0.99, 0.99, 1 + 0.01 + 0.01);
		glVertex3f(0, 0.99, 1 + 0.01 + 0.01);
	}
	else if (m_currentFaceNormal.x == -1)
	{
		glVertex3f(0 - 0.01, 0.99, 0.99);
		glVertex3f(0 - 0.01, 0.99, 0);
		glVertex3f(0 - 0.01, 0, 0);
		glVertex3f(0 - 0.01, 0, 0.99);
	}
	else if (m_currentFaceNormal.y == 1)
	{
		glVertex3f(0, 1 + 0.01, 0.99);
		glVertex3f(0.99, 1 + 0.01, 0.99);
		glVertex3f(0.99, 1 + 0.01, 0);
		glVertex3f(0, 1 + 0.01, 0);
	}
	else if (m_currentFaceNormal.y == -1)
	{
		glVertex3f(0, 0 - 0.01, 0.99);
		glVertex3f(0, 0 - 0.01, 0);
		glVertex3f(0.99, 0 - 0.01, 0);
		glVertex3f(0.99, 0 - 0.01, 0.99);
		
	}
	glEnd();
	glPopMatrix();

	Shader::Disable();

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
	{
		SetFullscreen(!IsFullscreen());

	}
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
	//F3 -> toggle info
	else if (m_keyboard[sf::Keyboard::F3])
	{
		displayInfo = !displayInfo;

	}
	//Lshift + F5 -> delete Cache
	else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::F5])
	{
		for (int i = 0; i < WORLD_SIZE; i++)
			for (int j = 0; j < WORLD_SIZE; j++)
				m_world.ChunkAt(i, j)->DeleteCache();

	}
	//Lshift + O -> open map
	else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::O])
	{
		//m_world.LoadMap("map.sav", m_bInfo);
		std::thread t(std::bind(&World::LoadMap, &m_world, "map.sav", m_bInfo));
		t.detach();
		//m_player.Spawn(m_world);
	}
	//Lshift + W -> Write map
	else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::W])
	{
		//m_world.SaveMap("map.sav");
		std::thread t(&World::SaveMap, &m_world, "map.sav");
		t.detach();
	}

	//Lshift + R -> Random map
	else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::R])
	{
		//m_world.InitMap(time(NULL));
		std::thread t(&World::InitMap, &m_world, time(NULL));
		t.detach();
		//m_player.Spawn(m_world);
	}

	//Lshift + F -> Flat map
	else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::F])
	{
		//m_world.InitMap(0);
		std::thread t(&World::InitMap, &m_world, 0);
		t.detach();
		//m_player.Spawn(m_world);
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

	//Left Click
	if (button == 1 && m_currentBlock.x != -1)
	{
		Vector3<int> chunkPos(m_currentBlock.x / CHUNK_SIZE_X, 0, m_currentBlock.z / CHUNK_SIZE_Z);
		m_world.ChunkAt(chunkPos.x, chunkPos.z)->RemoveBloc(m_currentBlock.x - (chunkPos.x * CHUNK_SIZE_X), m_currentBlock.y, m_currentBlock.z - (chunkPos.z * CHUNK_SIZE_X));

	}

	//Right Click
	else if (button == 4 && m_currentBlock.x != -1)
	{
		//Position du nouveau block
		Vector3<int> newBlocPos(m_currentBlock.x + m_currentFaceNormal.x, m_currentBlock.y + m_currentFaceNormal.y, m_currentBlock.z + m_currentFaceNormal.z);
		Vector3<int> chunkPos(newBlocPos.x / CHUNK_SIZE_X, 0, newBlocPos.z / CHUNK_SIZE_Z);

		//Si le chunk existe on place le block
		if (m_world.ChunkAt(chunkPos.x, chunkPos.z) && newBlocPos.x >= 0 && newBlocPos.z >= 0 && newBlocPos.y >= 0)
		{
			m_world.ChunkAt(chunkPos.x, chunkPos.z)->PlaceBlock(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X), newBlocPos.y, newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X), m_player.GetBlock());

			//Si ya collision on efface le block
			if (m_player.CheckCollision(m_world))
				m_world.ChunkAt(chunkPos.x, chunkPos.z)->SetBlock(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X), newBlocPos.y, newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X), BTYPE_AIR);
		}


	}
	//Scroll Up
	if (button == 8)
		m_player.SetBlock(1);

	//Scroll Down
	else if (button == 16)
		m_player.SetBlock(-1);

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
	glEnable(GL_TEXTURE_2D);

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
	if (displayInfo)
	{
		//Fps
		ss << "Fps: " << m_fps;
		PrintText(10, Height() - 25, 12, ss.str());

		//Chunk dirty
		ss.str("");
		ss << "Chunk not updated: " << m_chunkToUpdate;
		PrintText(10, Height() - 45, 12, ss.str());

		//vie du joueur
		ss.str("");
		ss << "Health: " << m_player.GetHP();
		PrintText(10, 130, 12, ss.str());

		//Armur du joueur
		ss.str("");
		ss << "Armor: " << m_player.GetArmor();
		PrintText(10, 110, 12, ss.str());

		//force du joueur
		ss.str("");
		ss << "A.Damage: " << m_player.GetAttackDamage();
		PrintText(10, 90, 12, ss.str());

		//Attack speed du joueur
		ss.str("");
		ss << "A.Speed: " << m_player.GetAttackSpeed();
		PrintText(10, 70, 12, ss.str());

		//Range du joueur
		ss.str("");
		ss << "A.Range: " << m_player.GetAttackRange();
		PrintText(10, 50, 12, ss.str());

		int orientation = (int)m_player.GetHorizontalRotation() % 360;

		if (orientation < 0)
			orientation = 360 + orientation;

		std::string direction = "";

		if (orientation >= 315 || orientation <= 45)
			direction = "Negative Z";

		if (orientation >= 45 && orientation <= 135)
			direction = "Positive X";

		if (orientation >= 135 && orientation <= 225)
			direction = "Positive Z";

		if (orientation >= 225 && orientation <= 315)
			direction = "Negative X";


		//Rotation du joueur
		ss.str("");
		ss << "Orientation: " << orientation << " toward: " << direction;
		PrintText(10, 30, 12, ss.str());

		//Position du joueur
		ss.str("");
		ss << "Position " << m_player.GetPosition();
		PrintText(10, 10, 12, ss.str());
	}
	ss.str("");
	//Pour chaque 10 point de vie on met un carre sinon un espace
	for (int i = 0; i < m_player.GetHP() / 5; i++)
	{
		ss << (char)254; // Le carractere ■
	}
	for (int i = 0; i < 100 / 5 - m_player.GetHP() / 5; i++)
	{
		ss << " ";
	}
	PrintText((Width() / 2 - (ss.str().length() * 12) / 2) - 10, Height() - 25, 16, ss.str());

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

	glDisable(GL_BLEND);

	//Block selectionne
	glLoadIdentity();
	glTranslated(Width() - 64, 16, 0);

	glBegin(GL_QUADS);

	glVertex2i(-2, -2);
	glVertex2i(50, -2);
	glVertex2i(50, 50);
	glVertex2i(-2, 50);
	glEnd();

	m_textureAtlas.Bind();
	glColor3f(1.f, 1.f, 1.f);

	glBegin(GL_QUADS);
	glTexCoord2f(m_bInfo[m_player.GetBlock()].u + m_bInfo[m_player.GetBlock()].w * .50f, m_bInfo[m_player.GetBlock()].v + m_bInfo[m_player.GetBlock()].h * .50f);
	glVertex2i(0, 0);
	glTexCoord2f(m_bInfo[m_player.GetBlock()].u + m_bInfo[m_player.GetBlock()].w * .00f, m_bInfo[m_player.GetBlock()].v + m_bInfo[m_player.GetBlock()].h * .50f);
	glVertex2i(48, 0);
	glTexCoord2f(m_bInfo[m_player.GetBlock()].u + m_bInfo[m_player.GetBlock()].w * .00f, m_bInfo[m_player.GetBlock()].v + m_bInfo[m_player.GetBlock()].h * .75f);
	glVertex2i(48, 48);
	glTexCoord2f(m_bInfo[m_player.GetBlock()].u + m_bInfo[m_player.GetBlock()].w * .50f, m_bInfo[m_player.GetBlock()].v + m_bInfo[m_player.GetBlock()].h * .75f);
	glVertex2i(0, 48);

	glEnd();


	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::PrintText(unsigned int x, unsigned int y, float size, const std::string & t)
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

	if ((m_player.GetPosition() - Vector3<float>(posX, posY, posZ)).Length() < EDITING_DISTANCE)
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

						if (Tool::InRangeWithEpsilon<float>(posX, x, x + 1, 0.05f) && Tool::InRangeWithEpsilon<float>(posY, y, y + 1, 0.05f) && Tool::InRangeWithEpsilon<float>(posZ, z, z + 1, 0.05f))
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
		if (Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z, 0.05f))
			m_currentFaceNormal.z = -1;
		else if (Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z + 1, 0.05f))
			m_currentFaceNormal.z = 1;
		else if (Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x, 0.05f))
			m_currentFaceNormal.x = -1;
		else if (Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x + 1, 0.05f))
			m_currentFaceNormal.x = 1;
		else if (Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y, 0.05f))
			m_currentFaceNormal.y = -1;
		else if (Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y + 1, 0.05f))
			m_currentFaceNormal.y = 1;
	}
}

