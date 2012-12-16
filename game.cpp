#pragma once

#include "game.hpp"

Game::Game(Cinema *c)
{
	mCinema = c;
	c->setViewer(this);
	mHollywood = new Hollywood(&mWarehouse);
	mPlayer = mHollywood->createTestActor();
	mCinema->setPosition(mPlayer->getPosition().x, mPlayer->getPosition().y);

}

Game::~Game()
{
	mHollywood->deleteActor(mPlayer);
	std::map<sf::String, Actor*>::iterator i;
	for(i = mActors.begin(); i != mActors.end(); i++)
		mHollywood->deleteActor(i->second);
	delete mHollywood;
}

bool Game::tick(double time)
{
	// Check input.
	if(mCinema->isKeyDown(sf::Keyboard::Left))
		playerMove(-200 * time, 0);
	if(mCinema->isKeyDown(sf::Keyboard::Right))
		playerMove(200 * time, 0);

	std::map<sf::String, Actor*>::iterator i;
	for(i = mActors.begin(); i != mActors.end(); i++)
		i->second->tick(time);
	mPlayer->tick(time);
	for(i = mActors.begin(); i != mActors.end(); i++)
		mCinema->draw(i->second);
	mCinema->draw(mPlayer);
	return true;
}

void Game::playerMove(double x, double y)
{
	sf::FloatRect current = mPlayer->getBounds();
	sf::FloatRect test = current;

	bool hor = true;
	bool vert = true;

	// Test horrizontal collision.
	test.left += x;
	std::map<sf::String, Actor*>::iterator i;
	for(i = mPhysics.begin(); i != mPhysics.end(); i++)
		if(i->second->getBounds().intersects(test))
			hor = false;

	// Test vertical collision.
	test = current;
	test.top += y;
	for(i = mPhysics.begin(); i != mPhysics.end(); i++)
		if(i->second->getBounds().intersects(test))
			vert = false;

	// Actually move the player.
	test = current;
	if(hor)
	{
		mPlayer->translate(x, 0);
		mCinema->translate(x, 0);
	}
	if(vert)
	{
		mPlayer->translate(0, y);
		mCinema->translate(0, y);
	}
}