#pragma once

#include "game.hpp"

Game::Game(Cinema *c)
{
	mCinema = c;
	c->setViewer(this);
	mHollywood = new Hollywood(&mWarehouse);
	randomActor = mHollywood->createTestActor();
}

Game::~Game()
{
	mHollywood->deleteActor(randomActor);
	delete mHollywood;
}

void Game::pauseGame()
{

}

bool Game::tick(double time)
{
	mCinema->draw(randomActor);
	return true;
}