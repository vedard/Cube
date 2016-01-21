#include "engine.h"

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
		m_keyboard[i] = false;

	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
		m_mouseButton[i] = false;

	//Creation du tableau des tableaux;
	m_bInfo = new BlockInfo[256];
	m_cow = new Animal[MAX_COW];
	m_monster = new Monster[MAX_MONSTER];
	playerGun = new Gun[3];
}

Engine::~Engine()
{

	m_world.SaveMap("map.sav");
	//Sound::DeInit();
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
	glAlphaFunc(GL_GREATER, (GLclampf)0.6);

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
	LoadTexture(m_textureFont, TEXTURE_PATH "font.png");

	//Load texture dans l'atlas
	AddTextureToAtlas(BTYPE_GRASS, "Grass", TEXTURE_PATH "block_grass.bmp");
	AddTextureToAtlas(BTYPE_TEST, "Test", TEXTURE_PATH "block_test.bmp");
	AddTextureToAtlas(BTYPE_STONE, "Stone", TEXTURE_PATH "block_stone.bmp");
	AddTextureToAtlas(BTYPE_WOOD_PLANK, "Grass", TEXTURE_PATH "block_wood_plank.bmp");
	AddTextureToAtlas(BTYPE_CHEST, "Grass", TEXTURE_PATH "block_chest.bmp");
	AddTextureToAtlas(BTYPE_BED_ROCK, "Grass", TEXTURE_PATH "block_bed_rock.bmp");
	AddTextureToAtlas(BTYPE_DIRT, "Grass", TEXTURE_PATH "block_dirt.bmp");
	AddTextureToAtlas(BTYPE_IRON, "Grass", TEXTURE_PATH "block_iron.bmp");
	AddTextureToAtlas(BTYPE_COAL, "Grass", TEXTURE_PATH "block_coal.bmp");
	AddTextureToAtlas(BTYPE_DIAMOND, "Grass", TEXTURE_PATH "block_diamond.bmp");
	AddTextureToAtlas(BTYPE_GOLD, "Grass", TEXTURE_PATH "block_gold.bmp");
	AddTextureToAtlas(BTYPE_REDSTONE, "Grass", TEXTURE_PATH "block_redstone.bmp");
	AddTextureToAtlas(BTYPE_LAPIS_LAZULI, "Grass", TEXTURE_PATH "block_lapis_lazuli.bmp");
	AddTextureToAtlas(BTYPE_WOOD, "Grass", TEXTURE_PATH "block_wood.bmp");
	AddTextureToAtlas(BTYPE_LEAVE, "Grass", TEXTURE_PATH "block_leave.png");
	AddTextureToAtlas(BTYPE_WATER, "Grass", TEXTURE_PATH "block_water.png");
	AddTextureToAtlas(BTYPE_SAND, "Grass", TEXTURE_PATH "block_sand.bmp");
	AddTextureToAtlas(BTYPE_NETHEREACK, "Grass", TEXTURE_PATH "block_netherrack.bmp");
	AddTextureToAtlas(BTYPE_LAVA, "Grass", TEXTURE_PATH "block_lava.bmp");

	if (!m_textureAtlas.Generate(64, false))
	{
		std::cout << " Unable to generate texture atlas ..." << std::endl;
		abort();
	}

	//Audio
	std::cout << " Loading audio ..." << std::endl;
	Sound::AddSound(Sound::M9_FIRE, AUDIO_PATH "glock18-1.wav");
	Sound::AddSound(Sound::MP5K_FIRE, AUDIO_PATH "mp7-1.wav");
	Sound::AddSound(Sound::AK47_FIRE, AUDIO_PATH "ak47-1.wav");
	Sound::AddSound(Sound::GUN_DRAW, AUDIO_PATH "glock_draw.wav");
	Sound::AddSound(Sound::FLESH_IMPACT, AUDIO_PATH "cowhurt3.ogg");
	for (int i = 0; i < 6; i++)
		Sound::AddSound(Sound::STEP1 + i, AUDIO_PATH "grass" + std::to_string(i + 1) + ".wav");

	if (!m_music.openFromFile(AUDIO_PATH "music.wav"))
		abort();
	m_music.setLoop(true);
	m_music.setVolume(0);
	m_music.play();


	//Model 3d

	m_modelCow.LoadOBJ(MODEL_PATH "Cow.obj", TEXTURE_PATH "cow.png");
	m_modelRaptor.LoadOBJ(MODEL_PATH "Creeper.obj", TEXTURE_PATH "creeper.png");


	//Gun
	playerGun[W_PISTOL - 1].Init(MODEL_PATH "m9.obj", TEXTURE_PATH "m9.jpg", Sound::M9_FIRE, true, 400, 1000);
	playerGun[W_SUBMACHINE_GUN - 1].Init(MODEL_PATH "mp5k.obj", TEXTURE_PATH "mp5k.png", Sound::MP5K_FIRE, true, 800, 25);
	playerGun[W_ASSAULT_RIFLE - 1].Init(MODEL_PATH "ak47.obj", TEXTURE_PATH "ak47.bmp", Sound::AK47_FIRE, true, 600, 40);

	//Shader
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

	//Entity

	// -- Player
	m_player.SetName("Player 1");


	//  -- Monster
	for (int i = 0; i < MAX_MONSTER; i++)
	{
		m_monster[i].SetName("Monster " + std::to_string(i + 1));
		m_monster[i].SetTarget(&m_player);
	}

	//  -- Cow
	for (int i = 0; i < MAX_COW; i++)
		m_cow[i].SetName("Cow " + std::to_string(i + 1));


}

void Engine::UnloadResource()
{

}

void Engine::Render(float elapsedTime)
{

	//Clear 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!m_player.GetisAlive())
	{
		m_wireframe = false;	
		DrawDeathScreen();
		return;
	}

	static float gameTime = elapsedTime;
	static float nextGameUpdate = gameTime;
	gameTime += elapsedTime;

	//Chunk du joueur
	Vector3<int> playerPos((int)m_player.GetPosition().x / CHUNK_SIZE_X, 0, (int)m_player.GetPosition().z / CHUNK_SIZE_Z);

	//Spawn des monstre aleatoirement
	if ((int)(gameTime * 100) % 1000 == 0)
		for (int i = 0; i < MAX_MONSTER; i++)
			if (!m_monster[i].GetisAlive())
			{
				m_monster[i].Spawn(m_world, (int)((m_player.GetPosition().x) - 50 + rand() % 100), (int)((m_player.GetPosition().z) - 50 + rand() % 100));
				break;
			}

	if ((int)(gameTime * 100) % 100 == 0)
		for (int i = 0; i < MAX_COW; i++)
			if (!m_cow[i].GetisAlive())
			{
				m_cow[i].Spawn(m_world, (int)((m_player.GetPosition().x) - 100 + rand() % 200), (int)((m_player.GetPosition().z) - 100 + rand() % 200));
				break;
			}

	//On met a jour le fps
	if ((int)(gameTime * 100) % 10 == 0)
		m_fps = (int)round(1.f / elapsedTime);

	int loops = 0;

	//Lock les mouvements a 50 fps
	while (gameTime > nextGameUpdate && loops < 10)
	{
		//Update le player
		m_player.Move(m_keyboard[sf::Keyboard::W], m_keyboard[sf::Keyboard::S], m_keyboard[sf::Keyboard::A], m_keyboard[sf::Keyboard::D], m_world);

		//Footstep
		static Vector3<float> lastpos = m_player.GetPosition();
		if (sqrtf(pow(lastpos.x - m_player.GetPosition().x, 2) + pow(lastpos.z - m_player.GetPosition().z, 2)) > 1.8f && !m_player.GetisInAir())
		{
			Sound::Play(Sound::STEP1 + rand() % 6, 12);
			lastpos = m_player.GetPosition();
		}

		//Tirer
		if (m_mouseButton[1] && m_player.GetWeapon() != W_BLOCK)
		{
			playerGun[m_player.GetWeapon() - 1].Shoot(m_player.GetPosition().x, m_player.GetPosition().y + m_player.GetDimension().y, m_player.GetPosition().z, m_player.GetHorizontalRotation(), m_player.GetVerticalRotation());
			(playerGun[m_player.GetWeapon() - 1].GetIsAuto()) ? false : m_mouseButton[1] = false;
		}

		//Update les balles
		for (int k = 0; k < 3; k++)				
			for (int i = 0; i < MAX_BULLET; i++)
			{
				playerGun[k].GetBullets()[i].Update();

				//Check si y a collision
				for (int j = 0; j < MAX_MONSTER; j++)
					playerGun[k].GetBullets()[i].CheckCollision(m_monster[j]);

				for (int j = 0; j < MAX_COW; j++)
					playerGun[k].GetBullets()[i].CheckCollision(m_cow[j]);

				playerGun[k].GetBullets()[i].CheckCollision(m_world);

			}
		
		//Update les monstres
		for (int i = 0; i < MAX_MONSTER; i++)
			m_monster[i].Move(m_world);

		//Update les monstres
		for (int i = 0; i < MAX_COW; i++)
			m_cow[i].Move(m_world);

		//Net
		static int sdf = 0;
		if (sdf++ % 3 == 0)
		{
			m_Netctl.Send("p " + Tool::VectorToString(m_player.GetPosition()));
			m_Netctl.Send("h " + std::to_string(m_player.GetHorizontalRotation()));	
		}

		std::string packetBuffer = m_Netctl.Receive();
		if (packetBuffer[0] == 'p')
			m_playerActor.SetPos(Tool::StringToVector(packetBuffer.substr(2)));
		else if (packetBuffer[0] == 'h')
			m_playerActor.SetRot((float)atof(packetBuffer.substr(2).c_str()));
		else if (packetBuffer[0] == 'm')
		{
			std::cout << packetBuffer << std::endl;
			std::stringstream ss(packetBuffer);
			char a;
			int cx, cz, bx, by, bz;
			int bt;
			ss >> a >> cx >> cz >> bx >> by >> bz >> bt;
			std::cout << cx << " " << cz << " " << bx << " " << by << " " << bz << " " << bt << " " << std::endl;
			m_world.ChunkAt((float)cx, (float)cz)->SetBlock(bx, by, bz,bt);
		}
		
		
		//Time control
		//1 / 0.02 = 50 fps
		nextGameUpdate += 0.02f;
		loops++;
	}

	glColor3f(1.f, 1.f, 1.f);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


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

	m_chunkToUpdate = m_world.ChunkNotUpdated(playerPos.x, playerPos.z);

	//Draw Monstres
	for (int i = 0; i < MAX_MONSTER; i++)
		m_monster[i].Draw(m_modelRaptor, false);

	for (int i = 0; i < MAX_COW; i++)
		m_cow[i].Draw(m_modelCow);

	m_playerActor.Draw(m_modelRaptor);

	//Draw guns
	if (m_player.GetWeapon() != W_BLOCK)
	playerGun[m_player.GetWeapon()-1].Draw(
			m_player.GetPosition().x,
			m_player.GetPosition().y + m_player.GetDimension().y,
			m_player.GetPosition().z,
			m_player.GetHorizontalRotation(), m_player.GetVerticalRotation());
	

	//m_world.InitChunks(playerPos.x, playerPos.z);
	std::thread t(&World::InitChunks, &m_world, playerPos.x, playerPos.z);
	t.detach();
	

	//Update les chunk autour du joueur si il sont dirty
	m_world.Update(playerPos.x, playerPos.z, m_bInfo);

	//Draw Chunks
	m_textureAtlas.Bind();
	m_world.Render(playerPos.x, playerPos.z, m_shader01.m_program);

	Shader::Disable();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//Draw Bullets
	for (int j = 0; j < 3; j++)	
		for (int i = 0; i < MAX_BULLET; i++)	
			playerGun[j].GetBullets()[i].Draw();
	
	//Draw Block focused (black square)
	if (m_player.GetWeapon() == W_BLOCK)
	{

		glPushMatrix();
		glTranslatef((GLfloat)m_currentBlock.x, (GLfloat)m_currentBlock.y, (GLfloat)m_currentBlock.z);

		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 0.0f, 0.0f);

		if (m_currentFaceNormal.z == -1)
		{
			glVertex3f(0, 0, 0 - (GLfloat)0.01);
			glVertex3f(0, (GLfloat)0.99, 0 - (GLfloat)0.01);
			glVertex3f((GLfloat)0.99, (GLfloat)0.99, 0 - (GLfloat)0.01);
			glVertex3f((GLfloat)0.99, 0, 0 - (GLfloat)0.01);
		}
		else if (m_currentFaceNormal.x == 1)
		{
			glVertex3f(1 + (GLfloat)0.01, (GLfloat)0.99, 0);
			glVertex3f(1 + (GLfloat)0.01, (GLfloat)0.99, (GLfloat)0.99);
			glVertex3f(1 + (GLfloat)0.01, 0, (GLfloat)0.99);
			glVertex3f(1 + (GLfloat)0.01, 0, 0);
		}
		else if (m_currentFaceNormal.z == 1)
		{
			glVertex3f(0, 0, 1 + (GLfloat)0.01 + (GLfloat)0.01);
			glVertex3f((GLfloat)0.99, 0, 1 + (GLfloat)0.01 + (GLfloat)0.01);
			glVertex3f((GLfloat)0.99, (GLfloat)0.99, 1 + (GLfloat)0.01 + (GLfloat)0.01);
			glVertex3f(0, (GLfloat)0.99, 1 + (GLfloat)0.01 + (GLfloat)0.01);
		}
		else if (m_currentFaceNormal.x == -1)
		{
			glVertex3f(0 - (GLfloat)0.01, (GLfloat)0.99, (GLfloat)0.99);
			glVertex3f(0 - (GLfloat)0.01, (GLfloat)0.99, 0);
			glVertex3f(0 - (GLfloat)0.01, 0, 0);
			glVertex3f(0 - (GLfloat)0.01, 0, (GLfloat)0.99);
		}
		else if (m_currentFaceNormal.y == 1)
		{
			glVertex3f(0, 1 + (GLfloat)0.01, (GLfloat)0.99);
			glVertex3f((GLfloat)0.99, 1 + (GLfloat)0.01, (GLfloat)0.99);
			glVertex3f((GLfloat)0.99, 1 + (GLfloat)0.01, 0);
			glVertex3f(0, 1 + (GLfloat)0.01, 0);
		}
		else if (m_currentFaceNormal.y == -1)
		{
			glVertex3f(0, 0 - (GLfloat)0.01, (GLfloat)0.99);
			glVertex3f(0, 0 - (GLfloat)0.01, 0);
			glVertex3f((GLfloat)0.99, 0 - (GLfloat)0.01, 0);
			glVertex3f((GLfloat)0.99, 0 - (GLfloat)0.01, (GLfloat)0.99);

		}
		glEnd();
		glPopMatrix();
	}

	GetBlocAtCursor();
	
	//Draw le hui
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

	//1 -> W_BLOCK 
	if (m_keyboard[sf::Keyboard::Num1])
		m_player.SetWeapon(W_BLOCK);

	//2 ->  W_PISTOL
	if (m_keyboard[sf::Keyboard::Num2])
	{
		m_player.SetWeapon(W_PISTOL);
		Sound::Play(Sound::GUN_DRAW);

	}
	//3 ->  W_SUBMACHINE_GUN
	if (m_keyboard[sf::Keyboard::Num3])
	{
		m_player.SetWeapon(W_SUBMACHINE_GUN);
		Sound::Play(Sound::GUN_DRAW);
	}
	//4 ->  W_ASSAULT_RIFLE
	if (m_keyboard[sf::Keyboard::Num4])
	{
		m_player.SetWeapon(W_ASSAULT_RIFLE);
		Sound::Play(Sound::GUN_DRAW);
	}
	//M -> spawn monster
	else if (m_keyboard[sf::Keyboard::M])
	{
		for (int i = 0; i < MAX_MONSTER; i++)
			if (!m_monster[i].GetisAlive())
			{
				m_monster[i].Spawn(m_world, (int)((m_player.GetPosition().x) - 50 + rand() % 100), (int)((m_player.GetPosition().z) - 50 + rand() % 100));
				break;
			}

	}


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
				m_world.ChunkAt((float)i, (float)j)->DeleteCache();

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

	if(!m_player.GetisAlive())
		if(m_keyboard[sf::Keyboard::Return])
			m_player.Spawn(m_world, WORLD_SIZE*CHUNK_SIZE_X / 2, WORLD_SIZE*CHUNK_SIZE_X / 2);
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

	float relativeX, relativeY;
	relativeX = (float)(x - Width() / 2);
	relativeY = (float)(y - Height() / 2);

	m_player.TurnLeftRight(relativeX * m_mouse_sensibility);
	m_player.TurnTopBottom(relativeY * m_mouse_sensibility);

}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
	//update le teableau
	m_mouseButton[button] = true;

	if (m_player.GetWeapon() == W_BLOCK)
	{
		//Left Click
		if (button == 1 && m_currentBlock.x != -1)
		{
			Vector3<int> chunkPos(m_currentBlock.x / CHUNK_SIZE_X, 0, m_currentBlock.z / CHUNK_SIZE_Z);
			m_world.ChunkAt((float)chunkPos.x, (float)chunkPos.z)->RemoveBloc(m_currentBlock.x - (chunkPos.x * CHUNK_SIZE_X), m_currentBlock.y, m_currentBlock.z - (chunkPos.z * CHUNK_SIZE_X));
			m_Netctl.Send("m " + 
				std::to_string(chunkPos.x) + 
				" " + std::to_string(chunkPos.z) + 
				" " + std::to_string(m_currentBlock.x - (chunkPos.x * CHUNK_SIZE_X)) + 
				" " + std::to_string(m_currentBlock.y) + 
				" " + std::to_string(m_currentBlock.z - (chunkPos.z * CHUNK_SIZE_X)) +
				" " + "0");
		}

		//Right Click
		else if (button == 4 && m_currentBlock.x != -1)
		{
			//Position du nouveau block
			Vector3<int> newBlocPos(m_currentBlock.x + m_currentFaceNormal.x, m_currentBlock.y + m_currentFaceNormal.y, m_currentBlock.z + m_currentFaceNormal.z);
			Vector3<int> chunkPos(newBlocPos.x / CHUNK_SIZE_X, 0, newBlocPos.z / CHUNK_SIZE_Z);

			//Si le chunk existe on place le block
			if (m_world.ChunkAt((float)chunkPos.x, (float)chunkPos.z) && newBlocPos.x >= 0 && newBlocPos.z >= 0 && newBlocPos.y >= 0)
			{
				m_world.ChunkAt((float)chunkPos.x, (float)chunkPos.z)->PlaceBlock(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X), newBlocPos.y, newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X), m_player.GetBlock());

				//Si ya collision on efface le block
				if (m_player.CheckCollision(m_world))
					m_world.ChunkAt((float)chunkPos.x, (float)chunkPos.z)->SetBlock(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X), newBlocPos.y, newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X), BTYPE_AIR);
				else
				{
					m_Netctl.Send("m " +
						std::to_string(chunkPos.x) +
						" " + std::to_string(chunkPos.z) +
						" " + std::to_string(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X)) +
						" " + std::to_string(newBlocPos.y) +
						" " + std::to_string(newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X)) +
						" " + std::to_string(m_player.GetBlock()));
				}
			}


		}
		//Scroll Up
		if (button == 8)
			m_player.SetBlock(1);

		//Scroll Down
		else if (button == 16)
			m_player.SetBlock(-1);
	}

}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
	//update le teableau
	m_mouseButton[button] = false;
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

void Engine::DrawHud() const
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

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	// Affichage du crosshair
	DrawCross(m_cross_color_r, m_cross_color_g, m_cross_color_b);

	if (m_player.GetWeapon() == W_BLOCK)
	{
		//Block selectionne
		glLoadIdentity();
		glTranslated(Width() - 64, 16, 0);

		//contour 	
		glColor3f(0.f, 0.f, 0.f);
		glBegin(GL_QUADS);
		glVertex2i(-2, -2);
		glVertex2i(50, -2);
		glVertex2i(50, 50);
		glVertex2i(-2, 50);
		glEnd();


		//block
		m_textureAtlas.Bind();
		glEnable(GL_TEXTURE_2D);
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
	}


	glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void Engine::DrawDeathScreen() const
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

	//Text
	std::ostringstream ss;
	ss << "You're dead !";
	PrintText(Width()/2 - (ss.str().length() * 48) / 2, Height()/2, 64, ss.str());
	
	ss.str("");
	ss << "Press enter to rise from your ashes";
	PrintText(Width()/2 - (ss.str().length() * 24) / 2, Height()/2 - 96, 32, ss.str());

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void Engine::PrintText(unsigned int x, unsigned int y, float size, const std::string & t) const
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

	if ((m_player.GetPosition() - Vector3<float>((const float)posX, (const float)posY, (const float)posZ)).Length() < EDITING_DISTANCE)
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
						BlockType bt = m_world.BlockAt((float)x, (float)y, (float)z);

						if (bt == BTYPE_AIR)
							continue;

						m_currentBlock.x = x;
						m_currentBlock.y = y;
						m_currentBlock.z = z;

						if (Tool::InRangeWithEpsilon<float>((const float)posX, (const float)x, (const float)(x + 1), 0.05f) && Tool::InRangeWithEpsilon<float>((const float)posY, (const float)y, (const float)(y + 1), 0.05f) && Tool::InRangeWithEpsilon<float>((const float)posZ, (const float)z, (const float)(z + 1), 0.05f))
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
		if (Tool::EqualWithEpsilon<float>((const float)posZ, (const float)m_currentBlock.z, 0.05f))
			m_currentFaceNormal.z = -1;
		else if (Tool::EqualWithEpsilon<float>((const float)posZ, (const float)(m_currentBlock.z + 1), 0.05f))
			m_currentFaceNormal.z = 1;
		else if (Tool::EqualWithEpsilon<float>((const float)posX, (const float)(m_currentBlock.x), 0.05f))
			m_currentFaceNormal.x = -1;
		else if (Tool::EqualWithEpsilon<float>((const float)posX, (const float)(m_currentBlock.x + 1), 0.05f))
			m_currentFaceNormal.x = 1;
		else if (Tool::EqualWithEpsilon<float>((const float)posY, (const float)(m_currentBlock.y), 0.05f))
			m_currentFaceNormal.y = -1;
		else if (Tool::EqualWithEpsilon<float>((const float)posY, (const float)(m_currentBlock.y + 1), 0.05f))
			m_currentFaceNormal.y = 1;
	}
}

void Engine::DrawCross(float r, float g, float b) const
{
	glLoadIdentity();
	glTranslated(Width() / 2, Height() / 2, 0);
	glColor3f(r, g, b);
	glBegin(GL_QUADS);

	glVertex2i(-1, -10);
	glVertex2i(1, -10);
	glVertex2i(1, -3);
	glVertex2i(-1, -3);

	glVertex2i(-1, 10);
	glVertex2i(-1, 3);
	glVertex2i(1, 3);
	glVertex2i(1, 10);

	glVertex2i(-10, 1);
	glVertex2i(-10, -1);
	glVertex2i(-3, -1);
	glVertex2i(-3, 1);

	glVertex2i(10, 1);
	glVertex2i(3, 1);
	glVertex2i(3, -1);
	glVertex2i(10, -1);

	glEnd();

}

void Engine::AddTextureToAtlas(BlockType type, const std::string &name, const std::string &path)
{
	m_bInfo[type].Init(type, name);
	m_texBlockIndex = m_textureAtlas.AddTexture(path);
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[type].u, m_bInfo[type].v, m_bInfo[type].w, m_bInfo[type].h);
}
