#include "actor.hpp"
#include "cinema.hpp"
#include "level.hpp"
#include "warehouse.hpp"

#include <fstream>

#include <iostream>

int main(int argc, char **argv)
{
	std::vector<sf::String> mLevels;
	unsigned int currentLevel = 0;
	std::ifstream file;
	file.open("./levels.txt");
	if(!file.is_open())
	{
		std::cout << "COULD NOT OPEN LEVELS.TXT.\n";
		system("pause");
		return 0;
	}
	while(!file.eof())
	{
		std::string buffer;
		file >> buffer;
		mLevels.push_back(buffer);
	}
	file.close();
	Cinema c;

	Warehouse w;
	Hollywood h(&w);

	Level *l = new Level(mLevels[currentLevel], &c, &h);

	sf::Clock mTimer;
	float deltaTime = 0;

	while(c.pollEvents())
	{
		deltaTime = mTimer.getElapsedTime().asSeconds();
		switch(l->tick(deltaTime))
		{
		case -1:
			delete l;
			l = new Level(mLevels[currentLevel], &c, &h);
			break;
		case 1:
			if(currentLevel < mLevels.size() - 1)
			{
				currentLevel++;
				delete l;
				l = new Level(mLevels[currentLevel], &c, &h);
			}
			else
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