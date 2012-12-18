#include "cinema.hpp"
#include "hollywood.hpp"

#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>

// Pasted from http://stackoverflow.com/questions/10051679/c-tokenize-string

std::vector<std::string> stringSplit(const std::string &source, const char *delimiter = " ", bool keepEmpty = false)
{
    std::vector<std::string> results;

    size_t prev = 0;
    size_t next = 0;

    while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
    {
        if (keepEmpty || (next - prev != 0))
        {
            results.push_back(source.substr(prev, next - prev));
        }
        prev = next + 1;
    }

    if (prev < source.size())
    {
        results.push_back(source.substr(prev));
    }

    return results;
}

int main(int argc, char **argv)
{
	// Select file.
	std::string levelName;
	std::cout << "Level name: ";
	std::cin >> levelName;

	/*
		IMPORTANT VARIABLES
	*/

	// Set up render system.
	Warehouse w;
	Hollywood h = Hollywood(&w);
	sf::RenderWindow c(sf::VideoMode(1024, 768), "LD25 Level Editor", sf::Style::Close);
	c.setVerticalSyncEnabled(true);
	c.setFramerateLimit(60);
	sf::View v = c.getDefaultView();

	// Set the background.
	sf::Texture bgt;
	bgt.loadFromFile("assets/" + levelName + "/background.png");
	sf::Sprite bgs;
	bgs.setTexture(bgt);

	// Important actor stuff.
	Actor *mPlayer = h.createVillain();
	Actor *mGoatShed = h.createGoatShed();
	std::list<Actor*> mPlatforms;
	std::list<Actor*> mVictims;
	std::list<sf::String> mAssets;
	Actor *mSelected = 0;

	/*
		LOAD DEFAULT STUFF FROM FILE OR OTHER.
	*/
	std::ifstream cf;
	cf.open("assets/" + levelName + "/level.txt");
	if(cf.is_open())
	{
		// Loading and stuff. Stole code from Stack Overflow.
		std::vector<std::vector<std::string>> mLines;
		while(!cf.eof())
		{
			std::string buffer;
			std::getline(cf, buffer);
				
			// Split into lines divided by ':'
			mLines.push_back(stringSplit(buffer, ":"));
		}
		cf.close();

		// Start positioning things.
		float x = (float) atof(mLines[0][0].c_str());
		float y = (float) atof(mLines[0][1].c_str());
		mPlayer->setPosition(x, y);
		x = (float) atof(mLines[1][0].c_str());
		y = (float) atof(mLines[1][1].c_str());
		mGoatShed->setPosition(x, y);

		// Add items and actors, etc...
		for(unsigned int i = 2; i < mLines.size(); i++)
		{
			if(mLines[i].size() == 0)
				break;
			if(mLines[i][0] == "add")
			{
				h.addItem(mLines[i][1]);
				mAssets.push_back(mLines[i][1]);
			}
			if(mLines[i][0] == "platform")
			{
				Actor *a = h.createPlatform(mLines[i][1]);
				a->setPosition((float) atof(mLines[i][2].c_str()),
					(float) atof(mLines[i][3].c_str()));
				mPlatforms.push_back(a);
			}
			if(mLines[i][0] == "victim")
			{
				Actor *a = h.createVictim(mLines[i][1]);
				a->setPosition((float) atof(mLines[i][2].c_str()),
					(float) atof(mLines[i][3].c_str()));
				mVictims.push_back(a);
			}
		}
	}
	else
	{
		mGoatShed->setPosition(v.getCenter().x, v.getCenter().y);
		mPlayer->setPosition(v.getCenter().x, v.getCenter().y);
	}

	/*
		MAIN LOOP STUFF.
	*/
	sf::Clock timer;
	float deltaTime = 0;
	bool cont = true;
	while(cont)
	{
		c.setView(v);
		sf::Event e;
		while(c.pollEvent(e))
		{
			std::string inp;
			bool conf = false;
			Actor *n;
			sf::Vector2f coords;
			std::ofstream file;
			switch(e.type)
			{
			case sf::Event::Closed:
				cont = false;
				break;
			case sf::Event::KeyPressed:
				switch(e.key.code)
				{
				case sf::Keyboard::Num0:	// SAVE
					file.open("assets/" + levelName + "/level.txt", std::ios::trunc);
					if(!file.is_open())
					{
						std::cout << "COULD NOT OPEN FILE FOR SAVING!\n";
						break;
					}
					file << mPlayer->getPosition().x << ":" << mPlayer->getPosition().y << std::endl;
					file << mGoatShed->getPosition().x << ":" << mPlayer->getPosition().y << std::endl;
					for(std::list<sf::String>::iterator i = mAssets.begin(); i != mAssets.end(); i++)
						file << "add:" << (std::string)(*i) << std::endl;
					for(std::list<Actor*>::iterator i = mPlatforms.begin(); i != mPlatforms.end(); i++)
						file << "platform:" << (std::string)(*i)->getAssetList()[0] << ":" << (*i)->getPosition().x << ":" << (*i)->getPosition().y << std::endl;
					for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
						file << "victim:" << (std::string)(*i)->getAssetList()[0] << ":" << (*i)->getPosition().x << ":" << (*i)->getPosition().y << std::endl;
					file.close();
					std::cout << "SAVED!\n";
					break;
				case sf::Keyboard::Num1:	// ADD PLATFORM
					std::cout << "Platform: ";
					std::cin >> inp;
					for(std::list<sf::String>::iterator i = mAssets.begin(); i != mAssets.end(); i++)
						if(inp == *i)
							conf = true;
					if(!conf)
					{
						mAssets.push_back(inp);
						h.addItem(inp);
					}
					n = h.createPlatform(inp);
					n->setPosition(v.getCenter().x, v.getCenter().y);
					mPlatforms.push_back(n);
					break;
				case sf::Keyboard::Num2:	// ADD VICTIM
					std::cout << "Victim: ";
					std::cin >> inp;
					for(std::list<sf::String>::iterator i = mAssets.begin(); i != mAssets.end(); i++)
						if(inp == *i)
							conf = true;
					if(!conf)
					{
						mAssets.push_back(inp);
						h.addItem(inp);
					}
					n = h.createVictim(inp);
					n->setPosition(v.getCenter().x, v.getCenter().y);
					mVictims.push_back(n);
					break;
				case sf::Keyboard::Back:	// DELETE ACTOR
					if(mSelected == 0 || mSelected == mGoatShed || mSelected == mPlayer)
						std::cout << "Cannot delete actor.\n";
					else
					{
						for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
						{
							if(mSelected == *i)
							{
								Actor *a = *i;
								mVictims.remove(*i);
								mSelected = 0;
								h.deleteActor(a);
								break;
							}
						}
						for(std::list<Actor*>::iterator i = mPlatforms.begin(); i != mPlatforms.end(); i++)
						{
							if(mSelected == 0)
								break;
							if(mSelected == *i)
							{
								Actor *a = *i;
								mPlatforms.remove(*i);
								mSelected = 0;
								h.deleteActor(a);
								break;
							}
						}
					}
					break;
				}
				break;
			case sf::Event::MouseButtonPressed:
				if(e.mouseButton.button == sf::Mouse::Right)
				{
					mSelected = 0;
					break;
				}
				if(mSelected == 0)
				{
					coords = c.convertCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
					for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
						if((*i)->intersects(coords.x, coords.y))
							mSelected = *i;
					for(std::list<Actor*>::iterator i = mPlatforms.begin(); i != mPlatforms.end(); i++)
						if((*i)->intersects(coords.x, coords.y))
							mSelected = *i;
					if(mGoatShed->intersects(coords.x, coords.y))
					{
						mSelected = mGoatShed;
					}
					if(mPlayer->intersects(coords.x, coords.y))
					{
						mSelected = mPlayer;
					}
					break;
				}
				else
				{
					coords = c.convertCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
					mSelected->setPosition(coords.x, coords.y);
					mSelected = 0;
				}
				break;
			}
		}
		if(cont == false)
			break;
		deltaTime = timer.getElapsedTime().asSeconds();

		c.setView(c.getDefaultView());
		c.draw(bgs);

		float moveX = 0;
		float moveY = 0;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			moveY -= (200 * deltaTime);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			moveX -= (200 * deltaTime);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			moveY += (200 * deltaTime);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			moveX += (200 * deltaTime);
		v.move(moveX, moveY);

		c.setView(v);

		c.draw(*mGoatShed->getSprite());
		c.draw(*mPlayer->getSprite());

		for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
			c.draw(*(*i)->getSprite());
		for(std::list<Actor*>::iterator i = mPlatforms.begin(); i != mPlatforms.end(); i++)
			c.draw(*(*i)->getSprite());
		
		timer.restart();
		c.display();
		c.clear();
		c.setView(c.getDefaultView());
	}

	/*
		CLEANUP
	*/
	for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
		h.deleteActor(*i);
	mVictims.clear();
	for(std::list<Actor*>::iterator i = mPlatforms.begin(); i != mPlatforms.end(); i++)
		h.deleteActor(*i);
	mPlatforms.clear();
	h.deleteActor(mGoatShed);
	h.deleteActor(mPlayer);
}