#include "openglcontext.h"
#include "define.h"
#include <chrono>
#include <thread>

OpenglContext::OpenglContext() : m_maxFps(999999), m_fullscreen(false), m_title(""), m_lastFrameTime(0)
{
}

OpenglContext::~OpenglContext()
{
}

bool OpenglContext::Start(const std::string& title, int width, int height)
{
	m_title = title;

	InitWindow(m_settings.m_width, m_settings.m_height);

	Init();
	LoadResource();
	sf::Clock clock;

	if (m_settings.m_isServer)
	{ 
		while (true)
		{
			clock.restart();
			Render(m_lastFrameTime);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			m_lastFrameTime = clock.getElapsedTime().asSeconds();
		}
	}
	else
	{
#ifdef NODISPLAY
#else
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
#endif
	}

	UnloadResource();
	DeInit();

	return true;
}

bool OpenglContext::Stop()
{
#ifdef NODISPLAY
#else
	m_app.close();
#endif
	return true;
}

void OpenglContext::CenterMouse()
{
#ifdef NODISPLAY
#else
	sf::Mouse::setPosition(sf::Vector2i(Width() / 2, Height() / 2), m_app);
#endif
}

int OpenglContext::Width() const
{
#ifdef NODISPLAY
	return 0;
#else
	return m_app.getSize().x;
#endif
}

int OpenglContext::Height() const
{
#ifdef NODISPLAY
	return 0;
#else
	return m_app.getSize().y;
#endif
}

void OpenglContext::SetMaxFps(int maxFps)
{
	m_maxFps = maxFps;
#ifdef NODISPLAY
#else
	m_app.setFramerateLimit(maxFps);
#endif
}

int OpenglContext::GetMaxFps() const
{
	return m_maxFps;
}

void OpenglContext::SetFullscreen(bool fullscreen)
{
	ShowCursor();
	m_settings.m_isfullscreen = fullscreen;
#ifdef NODISPLAY
#else
	m_app.setPosition(sf::Vector2i(0, 0));
#endif
	DeInit();
	InitWindow();
	Init();
}

bool OpenglContext::IsFullscreen() const
{
	return m_settings.m_isfullscreen;
}

void OpenglContext::ResetScreen()
{
	DeInit();
	InitWindow();
	Init();
}

void OpenglContext::MakeRelativeToCenter(int& x, int& y) const
{
	x = x - (Width() / 2);
	y = y - (Height() / 2);
}

void OpenglContext::ShowCursor()
{
#ifdef NODISPLAY
#else
	m_app.setMouseCursorVisible(true);
#endif
}

void OpenglContext::HideCursor()
{
#ifdef NODISPLAY
#else
	m_app.setMouseCursorVisible(false);
#endif
}

void OpenglContext::ShowCrossCursor() const
{
}

void OpenglContext::InitWindow(int width, int height)
{
	//Lis le fichier de configuration si il na pas deja ete lu
	if (m_settings.m_isServer)
		return;

#ifdef NODISPLAY
#else
	std::cout << sf::VideoMode::getDesktopMode().width << std::endl;

	if (m_settings.m_isfullscreen)
		m_app.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, 32),
			m_title.c_str(),
			sf::Style::Fullscreen,
			sf::ContextSettings(32, 8, m_settings.m_antialiasing));
	else
		m_app.create(sf::VideoMode((m_settings.m_width != 0) ? m_settings.m_width : MIN_WIDTH, (m_settings.m_height != 0) ? m_settings.m_height : MIN_HEIGHT, 32),
			m_title.c_str(),
			sf::Style::Close,
			sf::ContextSettings(32, 8, m_settings.m_antialiasing));
	m_app.setVerticalSyncEnabled(m_settings.m_vsync);
#endif
}

void OpenglContext::SetVSync(bool enable)
{
#ifdef NODISPLAY
#else
	m_app.setVerticalSyncEnabled(enable);
#endif

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

