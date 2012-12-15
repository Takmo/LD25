#include "actor.hpp"
#include "cinema.hpp"
#include "game.hpp"
#include "warehouse.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	Cinema c;
	Game g(&c);

	sf::Clock mTimer;
	double deltaTime = 0;

	while(c.pollEvents())
	{
		deltaTime = mTimer.getElapsedTime().asSeconds();
		mTimer.restart();
		g.tick(deltaTime);
		c.render();
	}

	return 0;
}