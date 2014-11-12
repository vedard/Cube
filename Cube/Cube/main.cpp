#include <SFML/Window.hpp>
#include <iostream>
#include "engine.h"
#include "tool.h"



int main(int *argcp, char **argv)
{
    Tool::CheckTypes();

	
    Engine engine;
    engine.SetMaxFps(60);
    engine.Start("Cube", 1366, 768, false);
	
}
