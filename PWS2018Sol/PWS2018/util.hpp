#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2d.h>

namespace Util {
	//moet extern omdat in verschillende bestanden gereferenced
	//ergo: idk dit is raar, verander niet anders werkt het niet somehow
	extern sf::Font dafont;
	extern b2Body * groundBody;
	extern b2Body * body;
	extern b2World world;
	void initFonts();
	void initPhysics();
	void updatePhysics();
}