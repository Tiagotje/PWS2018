#include "fonts.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

sf::Font Util::dafont;

//zodat de functie maar 1 keer laadt
bool init = false;

void Util::initFonts()
{
	if (init)
		return;
	else
		init = true;
	int status;
	status = Util::dafont.loadFromFile("dafont.ttf");
	if (!status)
		std::cout << "Error with dafont.ttf" << std::endl;
}
