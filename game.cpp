#pragma once

#include "game.hpp"

Game::Game(Cinema *c)
{
	mCinema = c;
	c->setViewer(this);
	mHollywood = new Hollywood(&mWarehouse);
	background.setTexture(*mWarehouse.getTexture("assets/background.png"));
	mPlayer = mHollywood->createTestActor();
	mPlayer->setPosition(100, 500);

	Actor *goat1 = mHollywood->createGoat();
	goat1->setPosition(450, 525);
	Actor *goat2 = mHollywood->createGoat();
	goat2->setPosition(550, 525);
	Actor *goat3 = mHollywood->createGoat();
	goat3->setPosition(650, 525);

	mActors.insert(std::make_pair("Villain", mPlayer));
	mActors.insert(std::make_pair("Goat1", goat1));
	mActors.insert(std::make_pair("Goat2", goat2));
	mActors.insert(std::make_pair("Goat3", goat3));
}

Game::~Game()
{
	std::map<sf::String, Actor*>::iterator i;
	for(i = mActors.begin(); i != mActors.end(); i++)
		mHollywood->deleteActor(i->second);
	mWarehouse.dereference("assets/background.png");
	delete mHollywood;
}

void Game::pauseGame()
{

}

bool Game::tick(double time)
{
	mCinema->draw(&background);

	if(mCinema->isKeyDown(sf::Keyboard::A))
		mPlayer->translate(-100 * time, 0);
	if(mCinema->isKeyDown(sf::Keyboard::D))
		mPlayer->translate(100 * time, 0);
	if(mCinema->isKeyDown(sf::Keyboard::W))
		mPlayer->translate(0, -100 * time);
	if(mCinema->isKeyDown(sf::Keyboard::S))
		mPlayer->translate(0, 100 * time);

	std::map<sf::String, Actor*>::iterator i;
	for(i = mActors.begin(); i != mActors.end(); i++)
		i->second->tick(time);
	for(i = mActors.begin(); i != mActors.end(); i++)
		mCinema->draw(i->second);
	return true;
}