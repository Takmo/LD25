#pragma once

#include "cinema.hpp"

#include <SFML/Window/Event.hpp>

void CinemaViewer::keyPressed(sf::Keyboard::Key key) {}
void CinemaViewer::keyReleased(sf::Keyboard::Key key) {}

Cinema::Cinema()
{
	mRenderWindow.create(sf::VideoMode(800, 600), "Ludum Dare 25", sf::Style::Close);
	mRenderWindow.setFramerateLimit(30);
	mRenderWindow.display();
	setViewer(this);
}

void Cinema::draw(Actor *actor)
{
	if(!actor->isVisible())
		return;
	mRenderWindow.draw(*actor->getSprite());
}

void Cinema::draw(sf::Sprite *sprite)
{
	mRenderWindow.draw(*sprite);
}

Cinema::~Cinema()
{
	mRenderWindow.close();
}

bool Cinema::isKeyDown(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool Cinema::pollEvents()
{
	bool retValue = true;
	sf::Event e;
	while(mRenderWindow.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
			retValue = false;
		if(e.type == sf::Event::KeyPressed)
			mCinemaViewer->keyPressed(e.key.code);
		if(e.type == sf::Event::KeyReleased)
			mCinemaViewer->keyReleased(e.key.code);
	}
	return retValue;
}

void Cinema::setViewer(CinemaViewer *viewer)
{
	mCinemaViewer = viewer;
}

void Cinema::render()
{
	mRenderWindow.display();
	mRenderWindow.clear();
}