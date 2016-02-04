#include <SFML/Window.hpp>
#include <iostream>
#include "engine.h"
#include "tool.h"



int main()
{
	Parametre& m_settings = Parametre::GetInstance();
	Tool::CheckTypes();
	Engine engine;
	engine.SetMaxFps(0);
	engine.Start("Cube");
}
