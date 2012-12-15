#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "actor.hpp"
#include "warehouse.hpp"

class Hollywood
{
public:

	/*
		CONSTRUCTOR
		Creates an actor factor that can create specific
		actors for us.
	*/
	Hollywood(Warehouse *warehouse);

	/*
		DESTRUCTOR
		Destroys the actor factory. Does NOT destroy actors.
		All actors MUST be deleted BEFORE the factory is deleted.
	*/
	~Hollywood();

	/*
		Actor *createTestActor();
		Create a test actor with default settings.
		This will serve as a template for any future actors.
	*/
	Actor *createTestActor();

	Actor *createGoat();

	/*
		void deleteActor(Actor *actor);
		Deletes the provided actor and dereferences the
		actor's texture in the Warehouse.
		Actors MUST be deleted using this command. Do NOT
		manually delete actors.
	*/
	void deleteActor(Actor *actor);

private:

	Warehouse *mWarehouse;
};