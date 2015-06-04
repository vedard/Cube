#include "openglcontext.h"
#include "define.h"

OpenglContext::OpenglContext() : m_maxFps(999999), m_fullscreen(false), m_title(""), m_lastFrameTime(0)
{
}

OpenglContext::~OpenglContext()
{
}

bool OpenglContext::Start(const std::string& title, int width, int height)
{
	m_title = title;

	InitWindow(width, height);

	Init();
	LoadResource();
	sf::Clock clock;

	while (m_app.isOpen())
	{
		clock.restart();

		sf::Event Event;
		while (m_app.pollEvent(Event))
		{
			switch (Event.type)
			{
			case sf::Event::Closed:
				m_app.close();
				break;
			case sf::Event::Resized:
				//DeInit();
				//InitWindow(Event.size.width, Event.size.height);
				//Init();
				break;
			case sf::Event::KeyPressed:
				KeyPressEvent(Event.key.code);
				break;
			case sf::Event::KeyReleased:
				KeyReleaseEvent(Event.key.code);
				break;
			case sf::Event::MouseMoved:
				MouseMoveEvent(Event.mouseMove.x, Event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				MousePressEvent(ConvertMouseButton(Event.mouseButton.button), Event.mouseButton.x, Event.mouseButton.y);
				break;
			case sf::Event::MouseButtonReleased:
				MouseReleaseEvent(ConvertMouseButton(Event.mouseButton.button), Event.mouseButton.x, Event.mouseButton.y);
				break;
			case sf::Event::MouseWheelMoved:
				if (Event.mouseWheel.delta > 0)
					MousePressEvent(MOUSE_BUTTON_WHEEL_UP, Event.mouseButton.x, Event.mouseButton.y);
				else
					MousePressEvent(MOUSE_BUTTON_WHEEL_DOWN, Event.mouseButton.x, Event.mouseButton.y);
				break;
			}
		}

		m_app.setActive();
		Render(m_lastFrameTime);
		m_app.display();

		m_lastFrameTime = clock.getElapsedTime().asSeconds();
	}

	UnloadResource();
	DeInit();

	return true;
}

bool OpenglContext::Stop()
{
	m_app.close();
	return true;
}

void OpenglContext::CenterMouse()
{
	sf::Mouse::setPosition(sf::Vector2i(Width() / 2, Height() / 2), m_app);
}

int OpenglContext::Width() const
{
	return m_app.getSize().x;
}

int OpenglContext::Height() const
{
	return m_app.getSize().y;
}

void OpenglContext::SetMaxFps(int maxFps)
{
	m_maxFps = maxFps;
	m_app.setFramerateLimit(maxFps);
}

int OpenglContext::GetMaxFps() const
{
	return m_maxFps;
}

void OpenglContext::SetFullscreen(bool fullscreen)
{
	ShowCursor();
	m_fullscreen = fullscreen;
	m_app.setPosition(sf::Vector2i(0,0));
	DeInit();
	InitWindow();
	Init();
}

bool OpenglContext::IsFullscreen() const
{
	return m_fullscreen;
}

void OpenglContext::MakeRelativeToCenter(int& x, int& y) const
{
	x = x - (Width() / 2);
	y = y - (Height() / 2);
}

void OpenglContext::ShowCursor()
{
	m_app.setMouseCursorVisible(true);
}

void OpenglContext::HideCursor()
{
	m_app.setMouseCursorVisible(false);
}

void OpenglContext::ShowCrossCursor() const
{
}

void OpenglContext::InitWindow(int width, int height)
{
	m_width = width;
	m_height = height;

	//Lis le fichier de configuration si il na pas deja ete lu
	static bool isConfigFileReaded = false;
	if(!isConfigFileReaded)
	{
		isConfigFileReaded = true;
		ReadConfig("Cube.conf");
	}



	std::cout << sf::VideoMode::getDesktopMode().width << std::endl; 	


	//Create windows
	if(m_fullscreen)
		m_app.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, 32), 
			m_title.c_str(), 
			sf::Style::Fullscreen , 
			sf::ContextSettings(32, 8, m_AntiAliasing));
	else
		m_app.create(sf::VideoMode((m_width != 0) ? m_width : 800, (m_height != 0) ? m_height : 600, 32), 
			m_title.c_str(), 
			sf::Style::Close, 
			sf::ContextSettings(32, 8, m_AntiAliasing));
	m_app.setVerticalSyncEnabled(m_vsync);
}

void OpenglContext::ReadConfig(const std::string& filename)
{

	Array2d<std::string> setting(10, 2);
	setting.Reset("null");
	setting.Get(0, 0) = "width";
	setting.Get(1, 0) = "height";
	setting.Get(2, 0) = "antialiasing";
	setting.Get(3, 0) = "fullscreen";
	setting.Get(4, 0) = "vsync";
	setting.Get(5, 0) = "render_distance";
	setting.Get(6, 0) = "cross_color_r";
	setting.Get(7, 0) = "cross_color_g";
	setting.Get(8, 0) = "cross_color_b";
	setting.Get(9, 0) = "mouse_sensibility";
	
	m_width = 0;
	m_height = 0;
	m_AntiAliasing = 0;
	m_vsync = true;
	m_renderDistance = 5;
	m_mouse_sensibility = 0.2f;
	m_cross_color_r = 0.f;
	m_cross_color_g = 0.f;
	m_cross_color_b = 0.f;

	std::cout << "Reading " << filename << "..." << std::endl;

	//Open file
	std::ifstream file;
	file.open(filename);

	//Read All file
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::stringstream ss(str);

	//Close it
	file.close();

	//read setting value
	while (ss)
	{
		std::string temp;

		ss >> temp;
		int index = setting.GetIndex(temp);
		if (index != -1)
			ss >> setting.Get(setting.GetIndex(temp), 1);
	}
	std::string a = setting.Get(2, 1);
	//Convert string into int
	if (setting.Get(0, 1) != "null")
		m_width =  atoi(setting.Get(0, 1).c_str());

	if (setting.Get(1, 1) != "null")
		m_height =  atoi(setting.Get(1, 1).c_str()); 

	if (setting.Get(2, 1) != "null")
		m_AntiAliasing = atoi(setting.Get(2, 1).c_str());

	if (setting.Get(3, 1) == "true" || setting.Get(3, 1) == "1")
		m_fullscreen = true;
	else
		m_fullscreen = false;

	if (setting.Get(4, 1) == "true" || setting.Get(4, 1) == "1")
		m_vsync = true;
	else
		m_vsync = false;

	if (setting.Get(5, 1) != "null")
		m_renderDistance = atoi(setting.Get(5, 1).c_str());
	if (setting.Get(6, 1) != "null")
		m_cross_color_r = atof(setting.Get(6, 1).c_str());
	if (setting.Get(7, 1) != "null")
		m_cross_color_g = atof(setting.Get(7, 1).c_str());
	if (setting.Get(8, 1) != "null")
		m_cross_color_b = atof(setting.Get(8, 1).c_str());
	if (setting.Get(9, 1) != "null")
		m_mouse_sensibility = atof(setting.Get(9, 1).c_str());

}

OpenglContext::MOUSE_BUTTON OpenglContext::ConvertMouseButton(sf::Mouse::Button button) const
{
	switch (button)
	{
	case sf::Mouse::Left:
		return MOUSE_BUTTON_LEFT;
	case sf::Mouse::Middle:
		return MOUSE_BUTTON_MIDDLE;
	case sf::Mouse::Right:
		return MOUSE_BUTTON_RIGHT;
	default:
		return MOUSE_BUTTON_NONE;
	}
}

