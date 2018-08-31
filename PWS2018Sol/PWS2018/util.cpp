#include "util.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

sf::Font Util::dafont;

//zodat de functie maar 1 keer laadt
bool initFont = false;

void Util::initFonts()
{
	if (initFont)
		return;
	initFont = true;
	int status;
	status = Util::dafont.loadFromFile("dafont.ttf");
	if (!status)
		std::cout << "Error with dafont.ttf" << std::endl;
}

