#pragma once

#include "cinema.hpp"
#include "hollywood.hpp"

class Game : public CinemaViewer
{
public:

	Game(Cinema *c);

	~Game();

	void pauseGame();

	/*
		bool tick(double time);
		Ticks the game by time. Return false if game over.
	*/
	bool tick(double time);

private:

	Actor *randomActor;	// TODO REMOVE THIS.

	Cinema *mCinema;
	Hollywood *mHollywood;
	Warehouse mWarehouse;
};