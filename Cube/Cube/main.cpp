#include <SFML/Window.hpp>
#include <iostream>
#include "engine.h"
#include "tool.h"



int main()
{
    Tool::CheckTypes();


    Engine engine;
    engine.SetMaxFps(0);
    engine.Start("Cube");
	
}
