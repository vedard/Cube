#include "engine.h"

Engine::Engine() :
	m_wireframe(false),
	m_shader01(),
	m_textureAtlas(NUMBER_OF_BLOCK - 1),
	m_world(),
	m_currentBlock(-1, -1, -1),
	displayInfo(false),
	m_fastInventoryKeySelected(-1),
	m_isInventoryOpen(false)
{
	m_LastTickTime = 0.0f;
	m_LastTickTimeWater = 0.0f;

	//Initialisation des touches
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
		m_keyboard[i] = false;

	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
		m_mouseButton[i] = false;

	//Creation du tableau des tableaux;
	m_bInfo = new BlockInfo[256];

	m_isMenuOpen = false;
}

Engine::~Engine()
{
	m_world.SaveMap("map.sav");
	Sound::DeInit();
	delete[] m_bInfo;
}

void Engine::Init()
{
	if (m_settings.m_isServer)
	{
		return;
	}

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

	


	// La lumiere
	GLfloat light0Amb[4] = { 5.f, 4.f, 3.f, 7.f };
	GLfloat light0Diff[4] = { 5.f, 4.f, 3.f, .7f };
	GLfloat light0Spec[4] = { 5.f, 4.f, 3.f, .7f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);


	//Shader
	std::cout << "Loading and compiling shaders ..." << std::endl;
	if (!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
	{
		std::cout << "Failed to load shader " << std::endl;
		exit(1);
	}

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{

}

void Engine::LoadResource()
{






	if (!m_settings.m_isServer)
	{
		//Load texture qui ne sont pas dans l'atlas
		LoadTexture(m_effectHurt, EFFECTS_PATH "HurtBlack.png");
		LoadTexture(m_hitMarker, EFFECTS_PATH "HitMarker.png");
		LoadTexture(m_textureSky, TEXTURE_PATH "sky.jpg");
		LoadTexture(m_textureFont, TEXTURE_PATH "font.png");

		//Load texture dans l'atlas
		AddTextureToAtlas(BTYPE_GRASS, "Grass", TEXTURE_PATH "block_grass.bmp", 1);
		AddTextureToAtlas(BTYPE_TEST, "Test", TEXTURE_PATH "block_test.bmp", 1);
		AddTextureToAtlas(BTYPE_STONE, "Stone", TEXTURE_PATH "block_stone.bmp", 1);
		AddTextureToAtlas(BTYPE_WOOD_PLANK, "Grass", TEXTURE_PATH "block_wood_plank.bmp", 1);
		AddTextureToAtlas(BTYPE_CHEST, "Grass", TEXTURE_PATH "block_chest.bmp", 1);
		AddTextureToAtlas(BTYPE_BED_ROCK, "Grass", TEXTURE_PATH "block_bed_rock.bmp", 1);
		AddTextureToAtlas(BTYPE_DIRT, "Grass", TEXTURE_PATH "block_dirt.bmp", 1);
		AddTextureToAtlas(BTYPE_IRON, "Grass", TEXTURE_PATH "block_iron.bmp", 1);
		AddTextureToAtlas(BTYPE_COAL, "Grass", TEXTURE_PATH "block_coal.bmp", 1);
		AddTextureToAtlas(BTYPE_DIAMOND, "Grass", TEXTURE_PATH "block_diamond.bmp", 1);
		AddTextureToAtlas(BTYPE_GOLD, "Grass", TEXTURE_PATH "block_gold.bmp", 1);
		AddTextureToAtlas(BTYPE_REDSTONE, "Grass", TEXTURE_PATH "block_redstone.bmp", 1);
		AddTextureToAtlas(BTYPE_LAPIS_LAZULI, "Grass", TEXTURE_PATH "block_lapis_lazuli.bmp", 1);
		AddTextureToAtlas(BTYPE_WOOD, "Grass", TEXTURE_PATH "block_wood.bmp", 1);
		AddTextureToAtlas(BTYPE_LEAVE, "Grass", TEXTURE_PATH "block_leave.png", 1);
		AddTextureToAtlas(BTYPE_WATER, "Grass", TEXTURE_PATH "block_water.png", 1);
		AddTextureToAtlas(BTYPE_SAND, "Grass", TEXTURE_PATH "block_sand.bmp", 1);
		AddTextureToAtlas(BTYPE_NETHEREACK, "Grass", TEXTURE_PATH "block_netherrack.bmp", 1);
		AddTextureToAtlas(BTYPE_LAVA, "Grass", TEXTURE_PATH "block_lava.bmp", 1);

		AddTextureToAtlas(BTYPE_RWATER1, "Grass", TEXTURE_PATH "block_rwater1.bmp", .90f);
		AddTextureToAtlas(BTYPE_RWATER2, "Grass", TEXTURE_PATH "block_rwater2.bmp", .5f);
		AddTextureToAtlas(BTYPE_RWATER3, "Grass", TEXTURE_PATH "block_rwater3.bmp", .25f);
		AddTextureToAtlas(BTYPE_FWATER, "Grass", TEXTURE_PATH "block_fwater.bmp", 1);

		AddTextureToAtlas(BTYPE_RLAVA1, "Grass", TEXTURE_PATH "block_rlava1.bmp", .90f);
		AddTextureToAtlas(BTYPE_RLAVA2, "Grass", TEXTURE_PATH "block_rlava2.bmp", .5f);
		AddTextureToAtlas(BTYPE_RLAVA3, "Grass", TEXTURE_PATH "block_rlava3.bmp", .25f);
		AddTextureToAtlas(BTYPE_FLAVA, "Grass", TEXTURE_PATH "block_flava.bmp", 1);

		if (!m_textureAtlas.Generate(64, false))
		{
			std::cout << "Unable to generate texture atlas ..." << std::endl;
			abort();
		}
		//Audio
		std::cout << " Loading audio ..." << std::endl;
		Sound::AddSound(Sound::M9_FIRE, WEAPONS_PATH "glock18-1.wav");
		Sound::AddSound(Sound::MP5K_FIRE, WEAPONS_PATH "mp7-1.wav");
		Sound::AddSound(Sound::AK47_FIRE, WEAPONS_PATH "ak47-1.wav");
		Sound::AddSound(Sound::GUN_DRAW, WEAPONS_PATH "glock_draw.wav");
		Sound::AddSound(Sound::FLESH_IMPACT, HURT_PATH "cowhurt3.ogg");
		Sound::AddSound(Sound::MUSIC1, MUSIC_PATH "music.wav");
		Sound::AddSound(Sound::DROWNING, HURT_PATH "drowning.wav");
		Sound::AddSound(Sound::GASPING, HURT_PATH "gasping.wav");
		Sound::AddSound(Sound::HURT, HURT_PATH "hurt.wav");
		Sound::AddSound(Sound::HITMARK, WEAPONS_PATH "hitmarker.wav");

		for (int i = 0; i < 9; i++)
		{
			if (i < 9)
			{
				Sound::AddSound(Sound::DEATH1 + i, DEATH_PATH "death" + std::to_string(i + 1) + ".wav");
			}
			if (i < 6)
			{
				Sound::AddSound(Sound::STEP1 + i, WALK_PATH "step" + std::to_string(i + 1) + ".wav");
			}
			if (i < 5)
			{
				Sound::AddSound(Sound::LEAVE1 + i, LEAVE_PATH "leave" + std::to_string(i + 1) + ".wav");
			}
			if (i < 4)
			{
				Sound::AddSound(Sound::GRASSSTEP1 + i, WALK_PATH "grass" + std::to_string(i + 1) + ".wav");
				Sound::AddSound(Sound::WATERSTEP1 + i, WALK_PATH "waterstep" + std::to_string(i + 1) + ".wav");
				Sound::AddSound(Sound::SANDSTEP1 + i, WALK_PATH "sand" + std::to_string(i + 1) + ".wav");
				Sound::AddSound(Sound::STONESTEP1 + i, WALK_PATH "stone" + std::to_string(i + 1) + ".wav");
				Sound::AddSound(Sound::WOODSTEP1 + i, WALK_PATH "wood" + std::to_string(i + 1) + ".wav");

			}



		}

			//Model 3d
			m_modelCow.LoadOBJ(MODEL_PATH "Cow.obj", TEXTURE_PATH "cow.png");
			m_modelCreeper.LoadOBJ(MODEL_PATH "Creeper.obj", TEXTURE_PATH "creeper.png");
			m_modelBear.LoadOBJ(MODEL_PATH "bear.obj", TEXTURE_PATH "bear.png");
			m_world.GetPlayer()->GetGuns()[W_PISTOL - 1].InitRessource(MODEL_PATH "m9.obj", TEXTURE_PATH "m9.jpg", Sound::M9_FIRE);
			m_world.GetPlayer()->GetGuns()[W_SUBMACHINE_GUN - 1].InitRessource(MODEL_PATH "mp5k.obj", TEXTURE_PATH "mp5k.png", Sound::MP5K_FIRE);
			m_world.GetPlayer()->GetGuns()[W_ASSAULT_RIFLE - 1].InitRessource(MODEL_PATH "ak47.obj", TEXTURE_PATH "ak47.bmp", Sound::AK47_FIRE);
		

		//Gun

		
		m_world.GetPlayer()->GetGuns()[W_PISTOL - 1].InitStat(false, 400, 100, 0.2);
		m_world.GetPlayer()->GetGuns()[W_SUBMACHINE_GUN - 1].InitStat(true, 800, 25, 0.25);
		m_world.GetPlayer()->GetGuns()[W_ASSAULT_RIFLE - 1].InitStat(true, 2400, 120, 0.4);

		
	}




	
}

void Engine::UnloadResource()
{

}

void Engine::UpdateEnvironement(float gameTime)
{
	Vector3<int> playerPos((int)m_world.GetPlayer()->GetPosition().x / CHUNK_SIZE_X, 0, (int)m_world.GetPlayer()->GetPosition().z / CHUNK_SIZE_Z);
	//Update le player
	m_world.GetPlayer()->Move(m_keyboard[m_settings.m_avancer], m_keyboard[m_settings.m_reculer], m_keyboard[m_settings.m_gauche], m_keyboard[m_settings.m_droite], m_world);

	
	// Update Guns
	if (m_mouseButton[4])
		m_world.GetPlayer()->GetGuns()[m_world.GetPlayer()->GetWeapon() - 1].EnableAiming();
	else
		m_world.GetPlayer()->GetGuns()[m_world.GetPlayer()->GetWeapon() - 1].DisableAiming();

	//Update les balles
	for (int k = 0; k < 3; k++)
	{
		m_world.GetPlayer()->GetGuns()[k].Update();
		for (int i = 0; i < MAX_BULLET; i++)
		{
			m_world.GetPlayer()->GetGuns()[k].GetBullets()[i].Update();
			Parametre& m_settings = Parametre::GetInstance();

			//Check si y a collision
			for (int j = 0; j < MAX_MONSTER; j++)
			{
				if (m_world.GetPlayer()->GetGuns()[k].GetBullets()[i].CheckCollision(m_world.GetMonster()[j]))
				{
					m_world.GetPlayer()->hasHit = 5;
					Sound::Play(Sound::HITMARK, m_settings.m_soundvolume * 5);
				}
			}
			for (int j = 0; j < MAX_COW; j++)
				if (m_world.GetPlayer()->GetGuns()[k].GetBullets()[i].CheckCollision(m_world.GetCow()[j]))
				{
					m_world.GetPlayer()->hasHit = 5;
					Sound::Play(Sound::HITMARK, m_settings.m_soundvolume * 5);
				}


			for (int j = 0; j < MAX_BEAR; j++)
				if (m_world.GetPlayer()->GetGuns()[k].GetBullets()[i].CheckCollision(m_world.GetBear()[j]))
				{
					m_world.GetPlayer()->hasHit = 5;
					Sound::Play(Sound::HITMARK, m_settings.m_soundvolume * 5);
				}

			m_world.GetPlayer()->GetGuns()[k].GetBullets()[i].CheckCollision(m_world);

		}
	}
	

	//Update les monstres
	for (int i = 0; i < MAX_MONSTER; i++)
		m_world.GetMonster()[i].Move(m_world);

	//Update les Cow
	for (int i = 0; i < MAX_COW; i++)

		m_world.GetCow()[i].Move(m_world);

	//Update les Bear
	for (int i = 0; i < MAX_BEAR; i++)
		m_world.GetBear()[i].Move(m_world);

	//m_world.InitChunks(playerPos.x, playerPos.z);
	std::thread t(&World::InitChunks, &m_world, playerPos.x, playerPos.z);
	t.detach();

	//Update les chunk autour du joueur si il sont dirty
	m_world.Update(playerPos.x, playerPos.z, m_bInfo);


}

void Engine::DrawEnvironement(float gameTime) {

	//Clear 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!m_world.GetPlayer()->GetisAlive())
	{
		m_wireframe = false;
		DrawDeathScreen();
		return;
	}
	

	Vector3<int> playerPos((int)m_world.GetPlayer()->GetPosition().x / CHUNK_SIZE_X, 0, (int)m_world.GetPlayer()->GetPosition().z / CHUNK_SIZE_Z);
	glColor3f(1.f, 1.f, 1.f);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//Place le joueur au centre du monde
	m_world.GetPlayer()->ApplyRotation();
	float shake = m_world.GetPlayer()->ApplyTranslation();



	//Ciel
	if (m_world.GetPlayer()->GetPosition().y > 64)
		DrawSky(gameTime);


	m_chunkToUpdate = m_world.ChunkNotUpdated(playerPos.x, playerPos.z);

	
		
	/// Position des lumières autour pour éclairer le joueur et les monstres
	// Position de la lumière 1
	GLfloat light0Pos1[4] = {
		m_world.GetPlayer()->GetPosition().x,
		m_world.GetPlayer()->GetPosition().y + 25,
		m_world.GetPlayer()->GetPosition().z - 50,
		1.f };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos1); 


	for (int i = 0; i < MAX_COW; i++)
		m_world.GetCow()[i].Draw(m_modelCow);
	for (int i = 0; i < MAX_BEAR; i++)
		m_world.GetBear()[i].Draw(m_modelBear);

	//Draw Monstres
	for (int i = 0; i < MAX_MONSTER; i++)
		m_world.GetMonster()[i].Draw(m_modelCreeper, false);

	

	//Draw guns
	if (m_world.GetPlayer()->GetWeapon() != W_BLOCK)
		m_world.GetPlayer()->GetGuns()[m_world.GetPlayer()->GetWeapon() - 1].Draw(
			m_world.GetPlayer()->GetPosition().x,
			m_world.GetPlayer()->GetPosition().y + m_world.GetPlayer()->GetDimension().y - shake,
			m_world.GetPlayer()->GetPosition().z,
			m_world.GetPlayer()->GetHorizontalRotation(), m_world.GetPlayer()->GetVerticalRotation());

	//Draw Chunks
	m_textureAtlas.Bind();
	m_world.Render(playerPos.x, playerPos.z, m_shader01.m_program);

	
	Shader::Disable();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	
	//Draw Bullets
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < MAX_BULLET; i++)
			m_world.GetPlayer()->GetGuns()[j].GetBullets()[i].Draw();

	//Draw Block focused (black square)
	if (m_world.GetPlayer()->GetWeapon() == W_BLOCK)
		DrawFocusedBlock();

	//Draw le hui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DrawHud();
	if (m_world.GetPlayer()->isHurt > 0)
		DrawHurtEffect();
	if (m_world.GetPlayer()->hasHit > 0)
		DrawHitMarker();
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Draw le menu
	if (m_isMenuOpen)
	{
		if (m_menu->m_currentMenu == SM_PRINCIPAL)
			DrawMenuPrincipal();
		else if (m_menu->m_currentMenu == SM_SETTINGS)
			DrawMenuSettings();
		else if (m_menu->m_currentMenu == SM_CONTROLS)
			DrawMenuControls();
		else if (m_menu->m_currentMenu == SM_SETTING_SELECTED)
			if (m_menu->m_currentMenuItem >= MS_CROSSCOLOR_R)
				DrawMenuSettingSelected(true);
			else
				DrawMenuSettingSelected(false);
		else if (m_menu->m_currentMenu == SM_CONTROL_SELECTED)
			DrawMenuControlSelected();
	}
}

void Engine::SetDayOrNight(float gametime)
{
	float time = sin(gametime / DAY_TIME);

	GLfloat light0Amb[4] = { 0, 0, 0, 0 };
	GLfloat fogcolor[4] = { 0, 0, 0, 0 };

	// Controle les cycles de couleurs de la lumière
	m_redLight = 5.f;
	m_greenLight = 0.48f * sin(time) + 4.5f;
	m_blueLight = 0.95f * sin(time) + 3.8;

	// Controle les cycles de couleurs du fog
	m_redFog = 0.5f * sin(time) + 0.45f;
	m_greenFog = 0.5f * sin(time) + 0.45f;
	m_blueFog = 0.5f * sin(time) + 0.48f;

	// Controle le cycle de densite du fog
	m_fogDensity = -0.031f * sin(time) + 0.052f;
	m_fogStart = 1.68f * sin(time) + 16;

	light0Amb[0] = m_redLight;
	light0Amb[1] = m_greenLight;
	light0Amb[2] = m_blueLight;
	light0Amb[3] = 7.f;

	fogcolor[0] = m_redFog;
	fogcolor[1] = m_greenFog;
	fogcolor[2] = m_blueFog;
	fogcolor[3] = 1;

	// Le fog
	glEnable(GL_FOG);
	GLint fogmode = GL_EXP2;
	glFogi(GL_FOG_MODE, fogmode);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, m_fogDensity);
	glFogf(GL_FOG_START, m_fogStart);
	glFogf(GL_FOG_END, 24.f);

	// La lumiere
	GLfloat light0Diff[4] = { 5.f, 4.f, 3.f, .7f };
	GLfloat light0Spec[4] = { 5.f, 4.f, 3.f, .7f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);
}

void Engine::Render(float elapsedTime)
{
	static float gameTime = elapsedTime;
	static float nextGameUpdate = gameTime;

	if(m_isInventoryOpen)
	{
		DrawEnvironement(gameTime);
		return;
	}

	gameTime += elapsedTime;

	//gestion des ticks
	if (gameTime - m_LastTickTime >= TICK_DELAY)
	{
		m_music.PlayNext();
		m_LastTickTime = gameTime;
		m_world.GetPlayer()->Tick();
	}

	//Spawn des monstre aleatoirement
	if ((int)(gameTime * 100) % 100 == 0)
		m_world.SpawnCows();
	if ((int)(gameTime * 100) % 100 == 0)
		m_world.SpawnBears();

	if ((int)(gameTime * 100) % 100 == 0)
		m_world.SpawnMonsters();

	//On met a jour le fps
	if ((int)(gameTime * 100) % 10 == 0)
		m_fps = (int)round(1.f / elapsedTime);

	int loops = 0;

	//Lock les mouvements a 50 fps
	while (gameTime > nextGameUpdate && loops < 10)
	{
		//Gestion des Ticks

		//Footstep
		static Vector3<float> lastpos = m_world.GetPlayer()->GetPosition();
		if (sqrtf(pow(lastpos.x - m_world.GetPlayer()->GetPosition().x, 2) + pow(lastpos.z - m_world.GetPlayer()->GetPosition().z, 2)) > 1.8f && !m_world.GetPlayer()->GetisInAir())
		{
			if (m_world.GetPlayer()->footUnderwater())
			{
				Sound::Play(Sound::WATERSTEP1 + rand() % 4);
			}
			else
			{
				switch (m_world.GetPlayer()->blockUnderPlayer())
				{
				case 1: // GRASS
					Sound::Play(Sound::GRASSSTEP1 + rand() % 4);
					break;
				case 3: // STONE
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				case 5: // WOOD PLANK
					Sound::Play(Sound::WOODSTEP1 + rand() % 4);
					break;
				case 7: // DIRT
					Sound::Play(Sound::GRASSSTEP1 + rand() % 4);
					break;
				case 8: // IRON
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				case 9: // COAL
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				case 10: // DIAMOND
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				case 11: // GOLD
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				case 12: // REDSTONE
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				case 13: // LAPIS
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				case 14: // WOOD
					Sound::Play(Sound::WOODSTEP1 + rand() % 4);
					break;
				case 15: // LEAVE
					Sound::Play(Sound::GRASSSTEP1 + rand() % 4);
					break;
				case 26: // SAND
					Sound::Play(Sound::SANDSTEP1 + rand() % 4);
					break;
				default:
					Sound::Play(Sound::STONESTEP1 + rand() % 4);
					break;
				}
			}
			lastpos = m_world.GetPlayer()->GetPosition();
		}

		
		if (m_mouseButton[1] && m_world.GetPlayer()->GetWeapon() != W_BLOCK && m_world.GetPlayer()->Shoot(m_world) == false)
			m_mouseButton[1] = false;


		//Net
		static int sdf = 0;
		if (sdf++ % 3 == 0)
		{
			m_Netctl.Send("p " + Tool::VectorToString(m_world.GetPlayer()->GetPosition()));
			m_Netctl.Send("h " + std::to_string(m_world.GetPlayer()->GetHorizontalRotation()));
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
			m_world.ChunkAt((float)cx, (float)cz)->SetBlock(bx, by, bz, bt, ' ');
		}
		SetDayOrNight(gameTime);
		UpdateEnvironement(gameTime);

		//Time control
		//1 / 0.02 = 50 fps
		nextGameUpdate += 0.02f;
		loops++;
	}

	if (!m_settings.m_isServer)
	{
		GetBlocAtCursor();
		DrawEnvironement(gameTime);
	}
}

void Engine::KeyPressEvent(unsigned char key)
{
	//update le teableau
	m_keyboard[key] = true;


	if ((key == FIRST_FAST_INVENTORY_KEY || key == SECOND_FAST_INVENTORY_KEY || key == THIRD_FAST_INVENTORY_KEY)) {		
		m_fastInventoryKeySelected = m_fastInventoryKeySelected != key ? key : -1;
	}

	if(m_isInventoryOpen) {
		if(m_keyboard[m_settings.m_openinventory]) {
			m_isInventoryOpen = false;
			m_world.GetPlayer()->GetInventory()->Init();
		} else {
			m_world.GetPlayer()->GetInventory()->OnKeyDown(key);		
		}
	} else if(m_isMenuOpen) {
		ManageAllMenuKeys(key);
	} else {
		if(m_keyboard[m_settings.m_openinventory] && !m_settings.m_inventaire_creatif) {
			m_isInventoryOpen = true;
		} else if (m_keyboard[m_settings.m_menu]) {
			m_isMenuOpen = true;
			ShowCursor();
			m_menu = new Menu(SM_PRINCIPAL);
		} else if (m_keyboard[m_settings.m_fullscreen]) { //f10 -> toggle fulscreen mode
			m_settings.m_isfullscreen = !m_settings.m_isfullscreen;
			m_settings.Save();
			SetFullscreen(IsFullscreen());
			m_keyboard[key] = false;
		} else if (m_keyboard[m_settings.m_noclip]) { //V -> toogle noclip mode
			m_world.GetPlayer()->ToggleNoClip();
		} else if (m_keyboard[m_settings.m_crouch]) { //Lctr -> Sneak
			m_world.GetPlayer()->SetSneak(true);
		} else if (m_keyboard[m_settings.m_run]) { //LSHIFT -> RUN
			m_world.GetPlayer()->SetRunning(true);
		}
				 
		if (m_keyboard[m_settings.m_inventory1]) { //1 -> W_BLOCK
			m_world.GetPlayer()->SetWeapon(W_BLOCK);
		}

		if (m_keyboard[m_settings.m_inventory2])
		{
			m_world.GetPlayer()->SetWeapon(W_PISTOL);
			Sound::Play(Sound::GUN_DRAW);
		}
		//3 ->  W_SUBMACHINE_GUN
		if (m_keyboard[m_settings.m_inventory3]) {
			m_world.GetPlayer()->SetWeapon(W_SUBMACHINE_GUN);
			Sound::Play(Sound::GUN_DRAW);
		} else if (m_keyboard[m_settings.m_inventory4]) { //4 ->  W_ASSAULT_RIFLE
			m_world.GetPlayer()->SetWeapon(W_ASSAULT_RIFLE);
			Sound::Play(Sound::GUN_DRAW);
		} else if (m_keyboard[m_settings.m_spawnmonster]) { //M -> spawn monster
			for (int i = 0; i < MAX_MONSTER; i++) {
				if (!m_world.GetMonster()[i].GetisAlive()) {
					m_world.GetMonster()[i].Spawn(m_world, (int)((m_world.GetPlayer()->GetPosition().x) - 50 + rand() % 100), (int)((m_world.GetPlayer()->GetPosition().z) - 50 + rand() % 100));
					break;
				}
			}
		} else if (m_keyboard[m_settings.m_wireframe]) { //y -> toggle wireframe mode
			m_wireframe = !m_wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
		} else if (m_keyboard[m_settings.m_info]) { //F3 -> toggle info
			displayInfo = !displayInfo;
		} else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::F5]) { //Lshift + F5 -> delete Cache
			for (int i = 0; i < WORLD_SIZE; i++) { 
				for (int j = 0; j < WORLD_SIZE; j++) { 
					m_world.ChunkAt((float)i, (float)j)->DeleteCache();
				}
			}
		} else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::O]) { //Lshift + O -> open map
			//m_world.LoadMap("map.sav", m_bInfo);
			std::thread t(std::bind(&World::LoadMap, &m_world, "map.sav", m_bInfo));
			t.detach();
			//m_world.GetPlayer()->Spawn(m_world);
		} else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::W]) { //Lshift + W -> Write map
			//m_world.SaveMap("map.sav");
			std::thread t(&World::SaveMap, &m_world, "map.sav");
			t.detach();
		} else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::R]) { //Lshift + R -> Random map
			//m_world.InitMap(time(NULL));
			std::thread t(&World::InitMap, &m_world, time(NULL));
			t.detach();
			//m_world.GetPlayer()->Spawn(m_world);
		} else if (m_keyboard[sf::Keyboard::RShift] && m_keyboard[sf::Keyboard::F]) { //Lshift + F -> Flat map
			//m_world.InitMap(0);
			std::thread t(&World::InitMap, &m_world, 0);
			t.detach();
			//m_world.GetPlayer()->Spawn(m_world);
		}

		if (!m_world.GetPlayer()->GetisAlive()) {
			if (m_keyboard[sf::Keyboard::Return]) {
				m_world.GetPlayer()->Spawn(m_world, WORLD_SIZE*CHUNK_SIZE_X / 2, WORLD_SIZE*CHUNK_SIZE_X / 2);
				m_world.GetPlayer()->isHurt = 0;
			}
		}
		
		if (m_keyboard[m_settings.m_jump]) { //space -> jump
			m_world.GetPlayer()->Jump();
		}
	}
}

void Engine::KeyReleaseEvent(unsigned char key)
{
	// update le tableau
	m_keyboard[key] = false;

	//end sneak
	if (!m_keyboard[sf::Keyboard::LControl])
		m_world.GetPlayer()->SetSneak(false);

	//end run
	if (!m_keyboard[sf::Keyboard::LShift])
		m_world.GetPlayer()->SetRunning(false);

}

void Engine::MouseMoveEvent(int x, int y)
{
	if (!m_isMenuOpen)
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
		
		if(!m_isInventoryOpen) {
			m_world.GetPlayer()->TurnLeftRight(relativeX * m_settings.m_mousesensibility);
			m_world.GetPlayer()->TurnTopBottom(relativeY *m_settings.m_mousesensibility);
		}
	}

}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
	if (m_isMenuOpen || m_isInventoryOpen) {
	} else {
		//update le teableau
		m_mouseButton[button] = true;

		if (m_world.GetPlayer()->GetWeapon() == W_BLOCK)
		{
			//Left Click
			if (button == 1 && m_currentBlock.x != -1)
			{
				Vector3<int> chunkPos(m_currentBlock.x / CHUNK_SIZE_X, 0, m_currentBlock.z / CHUNK_SIZE_Z);

				//AddToInventory
				//Add Block to Player Inventory BEFORE removal in the world. IF the inventory is not in creative mode
				if (!m_settings.m_inventaire_creatif)
				{
					BlockType btype = m_world.ChunkAt((float)chunkPos.x, (float)chunkPos.z)->GetBlock(m_currentBlock.x - (chunkPos.x * CHUNK_SIZE_X), m_currentBlock.y, m_currentBlock.z - (chunkPos.z * CHUNK_SIZE_X));
					if(btype != 0)					
						m_world.GetPlayer()->AddToInventory(btype);
				}

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
					bool removable = true;
					if (!m_settings.m_inventaire_creatif)
					{
						removable = m_world.GetPlayer()->RemoveFromInventory(m_world.GetPlayer()->GetBlock());
					}
					if (removable)
					{
						m_world.ChunkAt((float)chunkPos.x, (float)chunkPos.z)->PlaceBlock(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X), newBlocPos.y, newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X), m_world.GetPlayer()->GetBlock());

						//Si ya collision on efface le block
						if (m_world.GetPlayer()->CheckCollision(m_world))
							m_world.ChunkAt((float)chunkPos.x, (float)chunkPos.z)->SetBlock(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X), newBlocPos.y, newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X), BTYPE_AIR, 'Q');
						else
						{
							m_Netctl.Send("m " +
								std::to_string(chunkPos.x) +
								" " + std::to_string(chunkPos.z) +
								" " + std::to_string(newBlocPos.x - (chunkPos.x * CHUNK_SIZE_X)) +
								" " + std::to_string(newBlocPos.y) +
								" " + std::to_string(newBlocPos.z - (chunkPos.z * CHUNK_SIZE_X)) +
								" " + std::to_string(m_world.GetPlayer()->GetBlock()));
						}
					}
				}


			}

			if(m_settings.m_inventaire_creatif)
			{
				//Scroll Up
				if (button == 8)
					m_world.GetPlayer()->SetBlock(1);

				//Scroll Down
				else if (button == 16)
					m_world.GetPlayer()->SetBlock(-1);
			}
		}
	}

}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
	//update le teableau
	m_mouseButton[button] = false;
}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
	if (m_settings.m_isServer)
		return false;

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
		ss << "Health: " << m_world.GetPlayer()->GetHP();
		PrintText(10, 130, 12, ss.str());

		//Armur du joueur
		ss.str("");
		ss << "Armor: " << m_world.GetPlayer()->GetArmor();
		PrintText(10, 110, 12, ss.str());

		//force du joueur
		ss.str("");
		ss << "A.Damage: " << m_world.GetPlayer()->GetAttackDamage();
		PrintText(10, 90, 12, ss.str());

		//Attack speed du joueur
		ss.str("");
		ss << "A.Speed: " << m_world.GetPlayer()->GetAttackSpeed();
		PrintText(10, 70, 12, ss.str());

		//Range du joueur
		ss.str("");
		ss << "A.Range: " << m_world.GetPlayer()->GetAttackRange();
		PrintText(10, 50, 12, ss.str());

		int orientation = (int)m_world.GetPlayer()->GetHorizontalRotation() % 360;

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
		ss << "Position " << m_world.GetPlayer()->GetPosition();
		PrintText(10, 10, 12, ss.str());
	}
	ss.str("");
	//Pour chaque 10 point de vie on met un carre sinon un espace
	for (int i = 0; i < m_world.GetPlayer()->GetHP() / 5; i++)
	{
		ss << (char)254; // Le carractere ■
	}
	for (int i = 0; i < 100 / 5 - m_world.GetPlayer()->GetHP() / 5; i++)
	{
		ss << " ";
	}
	PrintText((Width() / 2 - (ss.str().length() * 12) / 2) - 10, Height() - 25, 16, ss.str());

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// Affichage du crosshair
	if (!m_world.GetPlayer()->GetGuns()[m_world.GetPlayer()->GetWeapon() - 1].isAiming())
		DrawCross(m_settings.m_crossred, m_settings.m_crossgreen, m_settings.m_crossblue);

	if (m_world.GetPlayer()->GetWeapon() == W_BLOCK && m_settings.m_inventaire_creatif)
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
		glTexCoord2f(m_bInfo[m_world.GetPlayer()->GetBlock()].u + m_bInfo[m_world.GetPlayer()->GetBlock()].w * .50f, m_bInfo[m_world.GetPlayer()->GetBlock()].v + m_bInfo[m_world.GetPlayer()->GetBlock()].h * .50f);
		glVertex2i(0, 0);
		glTexCoord2f(m_bInfo[m_world.GetPlayer()->GetBlock()].u + m_bInfo[m_world.GetPlayer()->GetBlock()].w * .00f, m_bInfo[m_world.GetPlayer()->GetBlock()].v + m_bInfo[m_world.GetPlayer()->GetBlock()].h * .50f);
		glVertex2i(48, 0);
		glTexCoord2f(m_bInfo[m_world.GetPlayer()->GetBlock()].u + m_bInfo[m_world.GetPlayer()->GetBlock()].w * .00f, m_bInfo[m_world.GetPlayer()->GetBlock()].v + m_bInfo[m_world.GetPlayer()->GetBlock()].h * .75f);
		glVertex2i(48, 48);
		glTexCoord2f(m_bInfo[m_world.GetPlayer()->GetBlock()].u + m_bInfo[m_world.GetPlayer()->GetBlock()].w * .50f, m_bInfo[m_world.GetPlayer()->GetBlock()].v + m_bInfo[m_world.GetPlayer()->GetBlock()].h * .75f);
		glVertex2i(0, 48);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	if(!m_settings.m_inventaire_creatif)
		RenderFastInventory();

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
	PrintText(Width() / 2 - (ss.str().length() * 48) / 2, Height() / 2, 64, ss.str());

	ss.str("");
	ss << "Press enter to rise from your ashes";
	PrintText(Width() / 2 - (ss.str().length() * 24) / 2, Height() / 2 - 96, 32, ss.str());

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

	if ((m_world.GetPlayer()->GetPosition() - Vector3<float>((const float)posX, (const float)posY, (const float)posZ)).Length() < EDITING_DISTANCE)
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
	glPushMatrix();
	glLoadIdentity();
	glTranslated(Width() / 2, Height() / 2, 0);
	glColor3f(r, g, b);
	glBegin(GL_QUADS);

	glVertex2i(-1, -25);
	glVertex2i(1, -25);
	glVertex2i(1, -20);
	glVertex2i(-1, -20);

	glVertex2i(-1, 25);
	glVertex2i(-1, 20);
	glVertex2i(1, 20);
	glVertex2i(1, 25);

	glVertex2i(-25, 1);
	glVertex2i(-25, -1);
	glVertex2i(-20, -1);
	glVertex2i(-20, 1);

	glVertex2i(25, 1);
	glVertex2i(20, 1);
	glVertex2i(20, -1);
	glVertex2i(25, -1);

	glEnd();
	glPopMatrix();
}

void Engine::DrawSky(float gameTime) const
{
	glPushMatrix();
	glTranslatef(m_world.GetPlayer()->GetPosition().x, 0, m_world.GetPlayer()->GetPosition().z);

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

void Engine::DrawFocusedBlock() const {

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

void Engine::AddTextureToAtlas(BlockType type, const std::string &name, const std::string &path, float hauteur)
{
	m_bInfo[type].Init(type, name);
	m_bInfo[type].SetHauteur(hauteur);
	m_texBlockIndex = m_textureAtlas.AddTexture(path);
	m_textureAtlas.TextureIndexToCoord(m_texBlockIndex, m_bInfo[type].u, m_bInfo[type].v, m_bInfo[type].w, m_bInfo[type].h);
}

void Engine::DrawMenuPrincipal() const
{
	// Menu specs
	int menuHeight = 150;
	int menuWidth = 200;
	int menuPositionX = Width() / 2;
	int menuPositionY = Height() / 2;

	glEnable(GL_TEXTURE_2D);
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Préparer le font pour écrire dans le menu
	m_textureFont.Bind();
	std::ostringstream ss;

	// Translate au centre pour y dessiner le menu
	glLoadIdentity();
	glTranslated(menuPositionX, menuPositionY, 0);

	// Zone menu
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
	glVertex2i(-menuWidth, -menuHeight);
	glVertex2i(menuWidth, -menuHeight);
	glVertex2i(menuWidth, menuHeight);
	glVertex2i(-menuWidth, menuHeight);
	glEnd();


	// Dessiner les boutons et mettre une couleur unique au bouton sélectionné.
	glColor3f(0.5f, 0.5f, 0.5f);

	DrawMenuButton(MP_CONTROLS, "Controls", Width() / 2 - 35, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MP_SETTINGS, "Settings", Width() / 2 - 35, (Height() / 2));
	DrawMenuButton(MP_EXIT_GAME, "Exit Game", Width() / 2 - 40, (Height() / 2) - (menuHeight / 2));


	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::DrawMenuSettings() const
{
	// Menu stats/info
	std::string fullscreen;
	std::string gameWidth;
	std::string gameHeight;
	std::string antiAliasing;
	std::string vSync;
	std::string renderDistance;
	std::string mouseSensivity;
	std::string rCrossColor;
	std::string gCrossColor;
	std::string bCrossColor;

	// Menu specs
	int menuHeight = 300;
	int menuWidth = 400;
	int menuPositionX = Width() / 2;
	int menuPositionY = Height() / 2;

	// Mettre les stats/infos en string
	gameWidth = std::to_string(m_settings.m_width);
	gameHeight = std::to_string(m_settings.m_height);
	antiAliasing = std::to_string(m_settings.m_antialiasing) + "x";
	renderDistance = std::to_string(m_settings.m_renderdistance);
	rCrossColor = std::to_string(m_settings.m_crossred);
	gCrossColor = std::to_string(m_settings.m_crossgreen);
	bCrossColor = std::to_string(m_settings.m_crossblue);
	mouseSensivity = std::to_string(m_settings.m_mousesensibility);

	if (m_settings.m_isfullscreen == true)
		fullscreen = "True";
	else
		fullscreen = "False";

	if (m_settings.m_vsync == true)
		vSync = "True";
	else
		vSync = "False";

	glEnable(GL_TEXTURE_2D);
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Translate au centre pour y dessiner le menu
	glLoadIdentity();
	glTranslated(menuPositionX, menuPositionY, 0);

	// Zone menu
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
	glVertex2i(-menuWidth, -menuHeight);
	glVertex2i(menuWidth, -menuHeight);
	glVertex2i(menuWidth, menuHeight);
	glVertex2i(-menuWidth, menuHeight);
	glEnd();

	// Préparer le font pour écrire dans le menu
	m_textureFont.Bind();
	glColor3f(0.7f, 0.7f, 0.7f);

	// Dessiner le titre
	PrintText(Width() / 2 - 60, (Height() / 2) + (menuHeight / 2) + (menuHeight / 4), 20.f, "Settings");

	int column1Width = (Width() / 2) - menuWidth + 40;
	int column2Width = (Width() / 2) - (menuWidth / 2);
	int column3Width = (Width() / 2) + (menuWidth / 4);
	int column4Width = (Width() / 2) + menuWidth - (menuWidth / 4);

	// Dessiner les boutons et mettre une couleur unique au bouton sélectionné.
	DrawMenuButton(MS_FULLSCREEN, "Fullscreen", column1Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MS_FULLSCREEN, fullscreen, column2Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MS_WIDTH, "Width", column1Width, (Height() / 2) + (menuHeight / 4));
	DrawMenuButton(MS_WIDTH, gameWidth, column2Width, (Height() / 2) + (menuHeight / 4));
	DrawMenuButton(MS_HEIGHT, "Height", column1Width, (Height() / 2));
	DrawMenuButton(MS_HEIGHT, gameHeight, column2Width, (Height() / 2));
	DrawMenuButton(MS_ANTI_ALIASING, "Anti-Aliasing", column1Width, (Height() / 2) - (menuHeight / 4));
	DrawMenuButton(MS_ANTI_ALIASING, antiAliasing, column2Width, (Height() / 2) - (menuHeight / 4));
	DrawMenuButton(MS_VSYNC, "V-Sync", column1Width, (Height() / 2) - (menuHeight / 2));
	DrawMenuButton(MS_VSYNC, vSync, column2Width, (Height() / 2) - (menuHeight / 2));
	DrawMenuButton(MS_RENDER_DISTANCE, "Render Distance", column1Width, (Height() / 2) - (menuHeight * 3 / 4));
	DrawMenuButton(MS_RENDER_DISTANCE, renderDistance, column2Width, (Height() / 2) - (menuHeight * 3 / 4));

	DrawMenuButton(MS_SOUND_VOLUME, "Sound Volume", column3Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MS_SOUND_VOLUME, std::to_string(m_settings.m_soundvolume), column4Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MS_MUSIC_VOLUME, "Music Volume", column3Width, (Height() / 2) + (menuHeight / 4));
	DrawMenuButton(MS_MUSIC_VOLUME, std::to_string(m_settings.m_musicvolume), column4Width, (Height() / 2) + (menuHeight / 4));
	DrawMenuButton(MS_CROSSCOLOR_R, "Cross Color R", column3Width, (Height() / 2));
	DrawMenuButton(MS_CROSSCOLOR_R, rCrossColor, column4Width, (Height() / 2));
	DrawMenuButton(MS_CROSSCOLOR_G, "Cross Color G", column3Width, (Height() / 2) - (menuHeight / 4));
	DrawMenuButton(MS_CROSSCOLOR_G, gCrossColor, column4Width, (Height() / 2) - (menuHeight / 4));
	DrawMenuButton(MS_CROSSCOLOR_B, "Cross Color B", column3Width, (Height() / 2) - (menuHeight / 2));
	DrawMenuButton(MS_CROSSCOLOR_B, bCrossColor, column4Width, (Height() / 2) - (menuHeight / 2));
	DrawMenuButton(MS_MOUSE_SENSITIVITY, "Mouse Sensivity", column3Width, (Height() / 2) - (menuHeight * 3 / 4));
	DrawMenuButton(MS_MOUSE_SENSITIVITY, mouseSensivity, column4Width, (Height() / 2) - (menuHeight * 3 / 4));


	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::DrawMenuControls() const
{
	// Menu specs
	int menuHeight = 300;
	int menuWidth = 400;
	int menuPositionX = Width() / 2;
	int menuPositionY = Height() / 2;

	glEnable(GL_TEXTURE_2D);
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Translate au centre pour y dessiner le menu
	glLoadIdentity();
	glTranslated(menuPositionX, menuPositionY, 0);

	// Zone menu
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
	glVertex2i(-menuWidth, -menuHeight);
	glVertex2i(menuWidth, -menuHeight);
	glVertex2i(menuWidth, menuHeight);
	glVertex2i(-menuWidth, menuHeight);
	glEnd();

	// Préparer le font pour écrire dans le menu
	m_textureFont.Bind();
	glColor3f(0.7f, 0.7f, 0.7f);

	// Dessiner le titre
	PrintText(Width() / 2 - 60, (Height() / 2) + (menuHeight / 2) + (menuHeight / 4), 20.f, "Controls");

	int column1Width = (Width() / 2) - menuWidth + 40;
	int column2Width = (Width() / 2) - (menuWidth / 2);
	int column3Width = (Width() / 2) + (menuWidth / 6);
	int column4Width = (Width() / 2) + menuWidth - (menuWidth / 2);

	// Dessiner les boutons et mettre une couleur unique au bouton sélectionné.
	DrawMenuButton(MC_AVANCER, "Forward", column1Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MC_GAUCHE, "Left", column1Width, (Height() / 2) + (menuHeight / 4));
	DrawMenuButton(MC_RECULER, "Backward", column1Width, (Height() / 2));
	DrawMenuButton(MC_DROITE, "Right", column1Width, (Height() / 2) - (menuHeight / 4));
	DrawMenuButton(MC_FULLSCREEN, "Fullscreen", column1Width, (Height() / 2) - (menuHeight / 2));

	DrawMenuButton(MC_INFO, "Info", column2Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MC_CROUCH, "Crouch", column2Width, (Height() / 2) + (menuHeight / 4));
	DrawMenuButton(MC_RUN, "Run", column2Width, (Height() / 2));
	DrawMenuButton(MC_JUMP, "Jump", column2Width, (Height() / 2) - (menuHeight / 4));
	DrawMenuButton(MC_NOCLIP, "No Clip", column2Width, (Height() / 2) - (menuHeight / 2));

	DrawMenuButton(MC_INVENTORY1, "Inventory 1", column3Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MC_INVENTORY2, "Inventory 2", column3Width, (Height() / 2) + (menuHeight / 4));
	DrawMenuButton(MC_INVENTORY3, "Inventory 3", column3Width, (Height() / 2));
	DrawMenuButton(MC_INVENTORY4, "Inventory 4", column3Width, (Height() / 2) - (menuHeight / 4));
	DrawMenuButton(MC_INVENTORY, "Inventory", column3Width, (Height() / 2) - (menuHeight / 2));

	DrawMenuButton(MC_SPAWNMONSTER, "Spawn Monster", column4Width, (Height() / 2) + (menuHeight / 2));
	DrawMenuButton(MC_WIREFRAME, "Wireframe", column4Width, (Height() / 2) + (menuHeight / 4));


	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::DrawMenuButton(int menuItem, std::string text, int xPos, int yPos) const
{
	if (m_menu->m_currentMenuItem == menuItem)
		glColor3f(1.f, 0.f, 0.f);
	else
		glColor3f(0.5f, 0.5f, 0.5f);

	PrintText(xPos, yPos, 12.f, text);
}

void Engine::ManageAllMenuKeys(unsigned char key)
{
	// Fermer menu
	if (m_keyboard[m_settings.m_menu])
	{
		if (m_isMenuOpen)
		{
			m_isMenuOpen = false;
			m_menu = new Menu(SM_PRINCIPAL);
			HideCursor();
		}
	}
	else if (m_keyboard[sf::Keyboard::Return])
	{
		ManageMenuEnterKeyPress();
	}
	else if (m_keyboard[sf::Keyboard::BackSpace])
	{
		if (m_menu->m_currentMenu == SM_PRINCIPAL)
		{
			m_isMenuOpen = false;
			m_menu = new Menu(SM_PRINCIPAL);
			HideCursor();
		}
		else if (m_menu->m_currentMenu == SM_SETTINGS || m_menu->m_currentMenu == SM_CONTROLS)
			m_menu = new Menu(SM_PRINCIPAL);
		else
			m_menu->OnKeyDown(key); // Laisser la classe menu gérer ses keyPress
	}
	else
	{
		m_menu->OnKeyDown(key); // Laisser la classe menu gérer ses keyPress

		if (m_menu->m_currentMenu == SM_CONTROL_SELECTED && m_menu->m_controlSelected == KEY_BINDED_SUCCESSFULLY)
		{
			int lastMenuItem = m_menu->m_currentMenuItem;
			m_menu = new Menu(SM_CONTROLS);
			m_menu->m_currentMenuItem = lastMenuItem;
		}
	}
}

void Engine::ManageMenuEnterKeyPress()
{
	if (m_menu->m_currentMenu == SM_PRINCIPAL)
	{
		if (m_menu->m_currentMenuItem == MP_EXIT_GAME)
		{
			CloseGame();
		}
		else if (m_menu->m_currentMenuItem == MP_SETTINGS)
			m_menu = new Menu(SM_SETTINGS);
		else if (m_menu->m_currentMenu == MP_CONTROLS)
			m_menu = new Menu(SM_CONTROLS);
	}
	else if (m_menu->m_currentMenu == SM_SETTINGS || m_menu->m_currentMenu == SM_SETTING_SELECTED)
	{
		if (m_menu->m_currentMenuItem == MS_FULLSCREEN)
		{
			m_settings.m_isfullscreen = !m_settings.m_isfullscreen;
			m_settings.Save();
			SetFullscreen(IsFullscreen());
		}
		else if (m_menu->m_currentMenuItem == MS_WIDTH)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				if (m_menu->m_settingNewValue < MIN_WIDTH)
					m_menu->m_settingNewValue = MIN_WIDTH;

				m_settings.m_width = m_menu->m_settingNewValue;
				m_settings.Save();
				ResetScreen();

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_HEIGHT)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				if (m_menu->m_settingNewValue < MIN_HEIGHT)
					m_menu->m_settingNewValue = MIN_HEIGHT;

				m_settings.m_height = m_menu->m_settingNewValue;
				m_settings.Save();
				ResetScreen();

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_ANTI_ALIASING)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				if (m_menu->m_settingNewValue == 0 ||
					m_menu->m_settingNewValue == 2 ||
					m_menu->m_settingNewValue == 4 ||
					m_menu->m_settingNewValue == 8)
				{

					m_settings.m_antialiasing = m_menu->m_settingNewValue;
					m_settings.Save();
					ResetScreen();

					m_menu->m_settingNewValue = 0;
					m_menu->m_currentMenu = SM_SETTINGS;
				}
			}
		}
		else if (m_menu->m_currentMenuItem == MS_VSYNC)
		{
			m_settings.m_vsync = !m_settings.m_vsync;
			m_settings.Save();
			m_app.setVerticalSyncEnabled(m_settings.m_vsync);
		}
		else if (m_menu->m_currentMenuItem == MS_RENDER_DISTANCE)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				m_settings.m_renderdistance = m_menu->m_settingNewValue;
				m_settings.Save();

				m_world.SetUpdateDistance(m_settings.m_renderdistance);

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_SOUND_VOLUME)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				if (m_menu->m_settingNewValue < MIN_VOLUME)
					m_menu->m_settingNewValue = MIN_VOLUME;
				else if (m_menu->m_settingNewValue > MAX_VOLUME)
					m_menu->m_settingNewValue = MAX_VOLUME;

				m_settings.m_soundvolume = m_menu->m_settingNewValue;
				m_settings.Save();

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_MUSIC_VOLUME)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				if (m_menu->m_settingNewValue < MIN_VOLUME)
					m_menu->m_settingNewValue = MIN_VOLUME;
				else if (m_menu->m_settingNewValue > MAX_VOLUME)
					m_menu->m_settingNewValue = MAX_VOLUME;

				m_settings.m_musicvolume = m_menu->m_settingNewValue;
				m_settings.Save();
				m_music.setVolume(m_settings.m_musicvolume);

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_CROSSCOLOR_R)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				float rValue = m_menu->m_settingNewValue;
				for (size_t i = 0; i < m_menu->m_digitCount; i++)
					rValue = rValue / 10;

				m_settings.m_crossred = rValue;
				m_settings.Save();

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_CROSSCOLOR_G)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				float rValue = m_menu->m_settingNewValue;
				for (size_t i = 0; i < m_menu->m_digitCount; i++)
					rValue = rValue / 10;

				m_settings.m_crossgreen = rValue;
				m_settings.Save();

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_CROSSCOLOR_B)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				float rValue = m_menu->m_settingNewValue;
				for (size_t i = 0; i < m_menu->m_digitCount; i++)
					rValue = rValue / 10;

				m_settings.m_crossblue = rValue;
				m_settings.Save();

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
		else if (m_menu->m_currentMenuItem == MS_MOUSE_SENSITIVITY)
		{
			if (m_menu->m_currentMenu == SM_SETTINGS)
				m_menu->m_currentMenu = SM_SETTING_SELECTED;
			else
			{
				float floatValue = m_menu->m_settingNewValue;
				for (size_t i = 0; i < m_menu->m_digitCount; i++)
					floatValue = floatValue / 10;

				m_settings.m_mousesensibility = floatValue;
				m_settings.Save();

				m_menu->m_settingNewValue = 0;
				m_menu->m_currentMenu = SM_SETTINGS;
			}
		}
	}
	else if (m_menu->m_currentMenu == SM_CONTROLS || m_menu->m_currentMenu == SM_CONTROL_SELECTED)
	{
		if (m_menu->m_currentMenuItem == MC_AVANCER)
		{
			m_menu->m_controlSelected = "Forward";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_GAUCHE)
		{
			m_menu->m_controlSelected = "Left";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_RECULER)
		{
			m_menu->m_controlSelected = "Backward";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_DROITE)
		{
			m_menu->m_controlSelected = "Right";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_FULLSCREEN)
		{
			m_menu->m_controlSelected = "Fullscreen";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_INFO)
		{
			m_menu->m_controlSelected = "Info";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_CROUCH)
		{
			m_menu->m_controlSelected = "Crouch";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_RUN)
		{
			m_menu->m_controlSelected = "Run";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_JUMP)
		{
			m_menu->m_controlSelected = "Jump";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_NOCLIP)
		{
			m_menu->m_controlSelected = "No Clip";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_INVENTORY1)
		{
			m_menu->m_controlSelected = "Inventory 1";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_INVENTORY2)
		{
			m_menu->m_controlSelected = "Inventory 2";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_INVENTORY3)
		{
			m_menu->m_controlSelected = "Inventory 3";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_INVENTORY4)
		{
			m_menu->m_controlSelected = "Inventory 4";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_INVENTORY)
		{
			m_menu->m_controlSelected = "Inventory";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_SPAWNMONSTER)
		{
			m_menu->m_controlSelected = "Spawn Monster";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
		else if (m_menu->m_currentMenuItem == MC_WIREFRAME)
		{
			m_menu->m_controlSelected = "Wireframe";
			m_menu->m_currentMenu = SM_CONTROL_SELECTED;
		}
	}
}

void Engine::DrawMenuSettingSelected(bool isFloat)
{
	// Menu specs
	int menuWidth = 100;
	int menuHeight = 60;
	int menuPositionX = Width() / 2;
	int menuPositionY = Height() / 2;

	glEnable(GL_TEXTURE_2D);
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Translate au centre pour y dessiner le menu
	glLoadIdentity();
	glTranslated(menuPositionX, menuPositionY, 0);

	// Zone menu
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
	glVertex2i(-menuWidth, -menuHeight);
	glVertex2i(menuWidth, -menuHeight);
	glVertex2i(menuWidth, menuHeight);
	glVertex2i(-menuWidth, menuHeight);
	glEnd();

	// Préparer le font pour écrire dans le menu
	m_textureFont.Bind();
	glColor3f(0.7f, 0.7f, 0.7f);

	PrintText(Width() / 2 - 80, Height() / 2 + menuHeight - 30, 12.f, "Backspace to erase");
	PrintText(Width() / 2 - 68, Height() / 2 + 5, 12.f, "Escape to cancel");
	PrintText(Width() / 2 - 70, Height() / 2 - 20, 12.f, "Enter to confirm");
	if (isFloat == true)
		PrintText(Width() / 2 - 70, Height() / 2 - menuHeight + 8, 12.f, "0." + std::to_string(m_menu->m_settingNewValue));
	else
		PrintText(Width() / 2 - 70, Height() / 2 - menuHeight + 8, 12.f, std::to_string(m_menu->m_settingNewValue));

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::DrawMenuControlSelected()
{
	// Menu specs
	std::string str = KEY_ALREADY_BOUND;
	int menuWidth = str.length() * 6 + 10;
	int menuHeight = 20;
	int menuPositionX = Width() / 2;
	int menuPositionY = Height() / 2;

	glEnable(GL_TEXTURE_2D);
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Translate au centre pour y dessiner le menu
	glLoadIdentity();
	glTranslated(menuPositionX, menuPositionY, 0);

	// Zone menu
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
	glVertex2i(-menuWidth, -menuHeight);
	glVertex2i(menuWidth, -menuHeight);
	glVertex2i(menuWidth, menuHeight);
	glVertex2i(-menuWidth, menuHeight);
	glEnd();

	// Préparer le font pour écrire dans le menu
	m_textureFont.Bind();
	glColor3f(1.f, 0.f, 0.f);

	if (m_menu->m_controlSelected == KEY_ALREADY_BOUND)
		PrintText(Width() / 2 - (m_menu->m_controlSelected.length() * 6), Height() / 2 + menuHeight - 30, 16.f, m_menu->m_controlSelected);
	else
		PrintText(Width() / 2 - (m_menu->m_controlSelected.length() * 6), Height() / 2 + menuHeight - 30, 16.f, m_menu->m_controlSelected);

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::RenderFastInventory() const
{
	int keys[3] = { THIRD_FAST_INVENTORY_KEY, SECOND_FAST_INVENTORY_KEY, FIRST_FAST_INVENTORY_KEY };
	int position = 0;	
	int quantity = 0;
	std::ostringstream oss;
	Inventory* inv = m_world.GetPlayer()->GetInventory(); 
	Item* items = inv->GetItems();
	BlockType bloctype = BTYPE_AIR;
	int x = 0;
	int y = 0;
	float size = 10.f;
	std::string t = "99+";
		
	if (m_world.GetPlayer()->GetWeapon() != W_BLOCK || !m_settings.m_inventaire_creatif)
	{
		glLoadIdentity();
		glTranslated(Width(), 16, 0);
	}
	
	glTranslated(0,-64,0);

	if(m_fastInventoryKeySelected == -1)
		m_world.GetPlayer()->SetBlockDirect(BTYPE_AIR);

	for(int j = 0; j < 5; j++)
	{
		glTranslated(0,64,0);
		for (int i = 0; i < 3; i++)
		{
			position = j*3+i;
			glTranslated(-64, 0, 0);

			bloctype = items[position].GetType();
			quantity = items[position].GetQuantity();
	
			if(inv->GetCurrentIndex() == position)
				glColor3f(0.f,0.f,1.f);
			else if(inv->GetCurrentMoveIndex() == position)
				glColor3f(102.f/255.f,102.f/255.f,1.f);
			else if (j == 0 && keys[i] == m_fastInventoryKeySelected) {
				glColor3f(1.f, 0.f, 0.f);
				if(m_world.GetPlayer()->GetBlock() != bloctype) {
					m_world.GetPlayer()->SetBlockDirect(bloctype);
				}
			} else
				glColor3f(0.f, 0.f, 0.f);

			if(quantity > 99)
				t = "99+";
			else
			{
				oss.str("");
				oss << quantity;
				t = oss.str();
			}

			glBegin(GL_QUADS);
			glVertex2i(-2, -2);
			glVertex2i(50, -2);
			glVertex2i(50, 50);
			glVertex2i(-2, 50);
			glEnd();
			
			if(quantity == 0)			
			{
				if(j == 0) {
					glColor3f(1.f,1.f,0.f);
				} else {
					glColor3f(1.f,1.f,77.f/255.f);
				}
				glBegin(GL_QUADS);
				glVertex2i(00, 00);
				glVertex2i(48, 00);
				glVertex2i(48, 48);
				glVertex2i(00, 48);
				glEnd();
			}
			else
			{
				//block
				m_textureAtlas.Bind();
				glEnable(GL_TEXTURE_2D);
				glColor3f(1.f, 1.f, 1.f);

				glBegin(GL_QUADS);
				glTexCoord2f(m_bInfo[bloctype].u + m_bInfo[bloctype].w * .50f, m_bInfo[bloctype].v + m_bInfo[bloctype].h * .50f);
				glVertex2i(0, 0);
				glTexCoord2f(m_bInfo[bloctype].u + m_bInfo[bloctype].w * .00f, m_bInfo[bloctype].v + m_bInfo[bloctype].h * .50f);
				glVertex2i(48, 0);
				glTexCoord2f(m_bInfo[bloctype].u + m_bInfo[bloctype].w * .00f, m_bInfo[bloctype].v + m_bInfo[bloctype].h * .75f);
				glVertex2i(48, 48);
				glTexCoord2f(m_bInfo[bloctype].u + m_bInfo[bloctype].w * .50f, m_bInfo[bloctype].v + m_bInfo[bloctype].h * .75f);
				glVertex2i(0, 48);		
				glEnd();
			
				m_textureFont.Bind();
			
				glPushMatrix();			
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
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
		}

		if(j == 0 && !m_isInventoryOpen)
			break;
		
		glTranslated(+64 * 3, 0 , 0); 
	}
}



void Engine::DrawHurtEffect() const
{
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	m_effectHurt.Bind();
	//static const int crossSize = 800;
	glLoadIdentity();
	glBegin(GL_QUADS);
	int ishurt = m_world.GetPlayer()->isHurt;
	int diviseur = 1;
	switch (ishurt)
	{
	case 20:
		diviseur = 54;
		glDisable(GL_BLEND);
		break;
	case 19:
		diviseur = 48;
		glDisable(GL_BLEND);
		break;
	case 18:
		diviseur = 40;
		glDisable(GL_BLEND);
		break;
	case 17:
		diviseur = 37;
		glDisable(GL_BLEND);
		break;
	case 16:
		diviseur = 35;
		glDisable(GL_BLEND);
		break;
	case 15:
		diviseur = 31;
		break;
	case 14:
		diviseur = 29;
		break;
	case 13:
		diviseur = 26;
		break;
	case 12:
		diviseur = 24;
		break;
	case 11:
		diviseur = 21;
		break;
	case 10:
		diviseur = 19;
		break;
	case 9:
		diviseur = 16;
		break;
	case 8:
		diviseur = 14;
		break;
	case 7:
		diviseur = 11;
		break;
	case 6:
		diviseur = 9;
		break;
	case 5:
		diviseur = 6;
		break;
	case 4:
		diviseur = 4;
		break;
	case 3:
		diviseur = 3;
		break;
	case 2:
		diviseur = 2;
		break;
	case 1:
		diviseur = 1;
		break;





	}
	glTexCoord2f(0, 0);
	glVertex2i(-(Width() / diviseur), -(Height() / diviseur));
	glTexCoord2f(1, 0);
	glVertex2i(Width() + (Width() / diviseur), -(Height() / diviseur));
	glTexCoord2f(1, 1);
	glVertex2i(Width() + (Width() / diviseur), Height() + (Height() / diviseur));
	glTexCoord2f(0, 1);
	glVertex2i(-(Width() / diviseur), Height() + (Height() / diviseur));

	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_BLEND);
	glPopMatrix();
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_BLEND);
	glPopMatrix();

}


void Engine::SetLightSource(float gametime)
{
	int daytime = (int)gametime % DAY_LENGTH;
	float positionX;
	float positionY;
	if (daytime / (DAY_LENGTH / 4) == 0)
	{
		positionX = (DAY_LENGTH / 4) - daytime;
		positionY = daytime;
	}
	else if (daytime / (DAY_LENGTH / 4) == 1)
	{
		positionX = (DAY_LENGTH / 4) - daytime;
		positionX = (DAY_LENGTH / 4) - positionX;
	}
	else if (daytime / (DAY_LENGTH / 4) == 2)
	{
		positionX = (DAY_LENGTH / 4) - daytime - (DAY_LENGTH / 2);
		positionY = daytime - (DAY_LENGTH / 2);
	}
	else
	{
		positionX = (DAY_LENGTH / 4) - daytime - (DAY_LENGTH / 2);
		positionX = (DAY_LENGTH / 4) - positionX;
	}

	GLfloat light0Pos[4] = { m_world.GetPlayer()->GetPosition().x + positionX ,  m_world.GetPlayer()->GetPosition().y + positionY, 0.0f, 0.0f };
	GLfloat light0Amb[4] = { 2.f, 2.f, 2.f, 1.0f };
	GLfloat light0Diff[4] = { 5.0f, 5.0f, 5.0f, 1.0f };
	GLfloat light0Spec[4] = { 1.2f, 1.2f, 1.2f, 1.0f };


	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);

}

void Engine::CloseGame()
{
	m_world.m_threadcontinue = false;
	int sound = Sound::LEAVE1 + rand() % 5;
	Sound::PlayAndWait(sound);
	Stop();
}

void Engine::DrawHitMarker() const
{
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	m_hitMarker.Bind();
	//static const int crossSize = 800;
	glLoadIdentity();
	glTranslated(Width() / 2 - 3, Height() / 2 - 2, 0);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2i(-30, -30);

	glTexCoord2f(1, 0);
	glVertex2i(30, -30);

	glTexCoord2f(1, 1);
	glVertex2i(30, 30);

	glTexCoord2f(0, 1);
	glVertex2i(-30, 30);


	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_BLEND);
	glPopMatrix();
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_BLEND);
	glPopMatrix();
}
