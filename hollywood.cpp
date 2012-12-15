#pragma once

#include "hollywood.hpp"

Hollywood::Hollywood(Warehouse *warehouse)
{
	mWarehouse = warehouse;

	mWarehouse->addItem("assets/background.png");
	mWarehouse->addItem("assets/goat.png");
	mWarehouse->addItem("assets/villain.png");
}

Hollywood::~Hollywood()
{

}

Actor *Hollywood::createTestActor()
{
	return new Actor("assets/villain.png", mWarehouse->getTexture("assets/villain.png"), 0, 0);
}

Actor *Hollywood::createGoat()
{
	return new Actor("assets/goat.png", mWarehouse->getTexture("assets/goat.png"), 0, 0);
}

void Hollywood::deleteActor(Actor *actor)
{
	std::vector<sf::String> list = actor->getAssetList();
	for(unsigned int i = 0; i < list.size(); i++)
		mWarehouse->dereference(list[i]);
	delete actor;
}