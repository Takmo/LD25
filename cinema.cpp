#pragma once

#include "cinema.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>

void CinemaViewer::click(int x, int y) {}
void CinemaViewer::keyPressed(sf::Keyboard::Key key) {}
void CinemaViewer::keyReleased(sf::Keyboard::Key key) {}
void CinemaViewer::lostFocus() {}

Cinema::Cinema()
{
	mRenderWindow.create(sf::VideoMode(1024, 768), "Dr. Ludum's Exploding Goats(tm)", sf::Style::Close);
	mRenderWindow.setFramerateLimit(60);
	mRenderWindow.setVerticalSyncEnabled(true);
	mRenderWindow.display();
	setViewer(this);
	mView = mRenderWindow.getDefaultView();
	mRenderWindow.setView(mView);
}

void Cinema::close()
{
	mRenderWindow.close();
}

void Cinema::draw(Actor *actor)
{
	if(!actor->isVisible())
		return;
	mRenderWindow.draw(*actor->getSprite());
}

void Cinema::draw(sf::Drawable *drawable)
{
	mRenderWindow.draw(*drawable);
}

Cinema::~Cinema()
{
	mRenderWindow.close();
}

bool Cinema::isKeyDown(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool Cinema::isOpen()
{
	return mRenderWindow.isOpen();
}

bool Cinema::pollEvents()
{
	bool retValue = true;
	sf::Event e;
	while(mRenderWindow.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
			retValue = false;
		if(e.type == sf::Event::LostFocus)
			mCinemaViewer->lostFocus();
		if(e.type == sf::Event::KeyPressed)
			mCinemaViewer->keyPressed(e.key.code);
		if(e.type == sf::Event::KeyReleased)
			mCinemaViewer->keyReleased(e.key.code);
	}
	return retValue;
}

void Cinema::setPosition(float x, float y)
{
	mView.setCenter(x, y);
	mRenderWindow.setView(mView);
}

void Cinema::setPosition(sf::Vector2f position)
{
	mView.setCenter(position);
	mRenderWindow.setView(mView);
}

void Cinema::setViewer(CinemaViewer *viewer)
{
	mCinemaViewer = viewer;
}

void Cinema::translate(float x, float y)
{
	mView.move(x, y);
	mRenderWindow.setView(mView);
}

void Cinema::render()
{
	mRenderWindow.display();
	mRenderWindow.clear();
}

void Cinema::resetCamera()
{
	sf::View reset = mRenderWindow.getDefaultView();
	mRenderWindow.setView(reset);
}