#include "actor.hpp"
#include "cinema.hpp"
#include "level.hpp"
#include "warehouse.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	Cinema c;

	Warehouse w;
	Hollywood h(&w);

	Level *l = new Level("blank", &c, &h);

	sf::Clock mTimer;
	float deltaTime = 0;

	while(c.pollEvents())
	{
		deltaTime = mTimer.getElapsedTime().asSeconds();
		switch(l->tick(deltaTime))
		{
		case -1:
			delete l;
			l = new Level("blank", &c, &h);
			break;
		case 1:
			c.close();
			break;
		default:
			break;
		}
		if(!c.isOpen())
			break;
		mTimer.restart();
		l->drawLevel();
		l->drawInterface();
		c.render();
	}

	delete l;

	return 0;
}