#pragma once

#include "cinema.hpp"
#include "hollywood.hpp"

class Game : public CinemaViewer
{
public:

	Game(Cinema *c);

	~Game();

	/*
		bool tick(double time);
		Ticks the game by time. Return false if game over.
	*/
	bool tick(double time);

private:

	void playerMove(double x, double y);

	std::map<sf::String, Actor*> mActors;

	std::map<sf::String, Actor*> mPhysics;

	Actor *mPlayer;

	Cinema *mCinema;
	Hollywood *mHollywood;
	Warehouse mWarehouse;
};