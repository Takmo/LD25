#pragma once

#include "hollywood.hpp"

Hollywood::Hollywood(Warehouse *warehouse)
{
	mWarehouse = warehouse;

	mWarehouse->addItem("assets/rofl.png");
}

Hollywood::~Hollywood()
{

}

Actor *Hollywood::createTestActor()
{
	return new Actor("assets/rofl.png", mWarehouse->getTexture("assets/rofl.png"), 0, 0);
}

void Hollywood::deleteActor(Actor *actor)
{
	std::vector<sf::String> list = actor->getAssetList();
	for(unsigned int i = 0; i < list.size(); i++)
		mWarehouse->dereference(list[i]);
	delete actor;
}