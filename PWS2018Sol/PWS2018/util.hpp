#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2d.h>

namespace Util {
	//moet extern omdat in verschillende bestanden gereferenced
	//ergo: idk dit is raar, verander niet anders werkt het niet somehow
	extern sf::Font dafont;
	void initFonts();
}