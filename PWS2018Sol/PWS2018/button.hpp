#pragma once

#include <SFML/Graphics.hpp>
#include <cstring>

//Definities van de knop, zie button.cpp

class Button {
public:
	Button();
	Button(sf::View, sf::FloatRect, 
		sf::Color, std::string, int, void(*f)());
	void setSize(sf::FloatRect);
	void checkMouse(sf::Vector2i);
	void setView(sf::View);
	void setFont(sf::Font);
	void setFunc(void(*f)());
	void setText(std::string, int);
	void setColor(sf::Color);
	void setTextPos(int x, int y);
	void draw();
private:
	sf::Text buttonText;
	sf::RectangleShape rect;
	sf::View view;
	sf::Font font;
	void(*func)();
};