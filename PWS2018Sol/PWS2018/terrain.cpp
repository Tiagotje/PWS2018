#include "Terrain.hpp"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <Box2D/Box2d.h>

#include "physics.hpp"
#include "main.hpp"

//De (helft van) de dikte van de lijn
const int WIDTH = 5;

//Geeft de kleinste waarde van 2
double min(double a, double b) {
	if (a < b) return a;
	return b;
}

//Geeft een random getal tussen 0.0 en 1.0
double randd() {
	return ((double)rand() / (double)RAND_MAX);
}

//Geeft een kleur ergens tussen (116,110,60) en (141, 220, 0)
sf::Color randomGreen() {
	double r = randd();
	return sf::Color(116 * r + 141 * (1 - r), 110 * r + 220 * (1 - r), 60 * r);
}

//Geeft aan of getal c tussen a en b zit
bool inrange(int a, int b, int c) {
	if (c >= a && c <= b) return true;
	if (c >= b && c <= a) return true;
	return false;
}

//Genereert de grond
std::vector<sf::Vector2f> Gen(int startX, int maxX, int sAmp, int minH, int maxH, bool neg)
{
	std::vector<sf::Vector2f> ret;
	int x = startX;
	int y = (int)(0.5f*minH + 0.5f*maxH);
	int prevX, prevY;
	//Iedere nieuwe waarde = vorige + willekeurige delta (maar tussen de min & max in)
	do {
		prevX = x;
		prevY = y;
		y = prevY + (sAmp) * (randd() - 0.5f)  * (1.0 + prevX / 500);
		if (y < minH)
			y += (minH - y) * 2;
		if (y > maxH)
			y += (maxH - y) * 2;
		int dX = 50 + randd() * 100 + (abs(prevY - y) / 2);
		if (neg)
			dX = -dX;
		x = prevX + dX;
		//Stop het nieuwe punt(x,y) in een lijst
		ret.push_back(sf::Vector2f(x, y));
		//Ga door totdat je voorbij het einde bent
	} while (inrange(startX, maxX, prevX));
	return ret;
}

sf::VertexArray LineToVertexArray(std::vector<sf::Vector2f> line)
{
	sf::VertexArray lines(sf::TriangleStrip, line.size() * 2);
	for (int i = 0; i < line.size(); i++)
	{
		sf::Color c = randomGreen();
		lines[2 * i + 0].position = line[i] + sf::Vector2f(0, WIDTH);
		lines[2 * i + 0].color = c;
		lines[2 * i + 1].position = line[i] + sf::Vector2f(0, -WIDTH);
		lines[2 * i + 1].color = c;
	}
	return lines;
}

void FoodposGen(std::vector<b2Vec2> data)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 5000);

	for (int i = 0; i < foodsize; i++)
	{
		float xPos = dis(gen);
		//zoek bijpassende y pos door te intrapoleren
		b2Vec2 p1;
		b2Vec2 p2;
		for (int j = 0; j < data.size(); j++)
			if (data[j].x > xPos) {
				p1 = data[j - 1];
				p2 = data[j];
				break;
			}
		//factor = 0.0 tm 1.0 hoever bij punt 1 of punt 2
		float factor = (xPos - p1.x) / (p2.x - p1.x);
		//vind passende tussenpositie
		float yPos = p1.y * (1 - factor) + p2.y * factor + 1 + dis(gen)/250;
		foodpos[i] = b2Vec2(xPos, yPos);
	}
}

//Punten tussen -5.000 en +5.000
sf::VertexArray TerrainGen()
{
	//Start de Random Number Generator op
	std::srand(time(NULL));
	//Genereert linker & rechter deel, -200 tot 200 = beginplatform
	std::vector<sf::Vector2f> right = Gen(200, 50000, 10, 150, 650, false);
	//Zet de punten in de juiste volgorde van links -> rechts
	std::vector<sf::Vector2f> line;
	line.push_back(sf::Vector2f(-200, 400));
	line.push_back(sf::Vector2f(200, 400));
	for (int i = 0; i < right.size(); i++)
		line.push_back(right[i]);


	//Define fixtures with shape (Box2D)
	b2ChainShape chain;
	std::vector<b2Vec2> b2points;
	//b2points = vector links -> rechts
	for (int i = 0; i < line.size(); i++) {
		b2points.push_back(b2Vec2(line[i].x * 0.1, (line[i].y - WIDTH) * -0.1));
	}
	//Genereer foodpos met data
	FoodposGen(b2points);
	//genereer chain met data
	chain.CreateChain(b2points.data(), b2points.size());
	Phys::groundBody->CreateFixture(&chain, 0.0f);
	b2Filter groundFilter = b2Filter();
	groundFilter.categoryBits = 3;
	groundFilter.maskBits = 3;
	Phys::groundBody->GetFixtureList()->SetFilterData(groundFilter);

	//Zet punten om dikke lijn (SFML)
	return LineToVertexArray(line);
}