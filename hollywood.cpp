#pragma once

#include "hollywood.hpp"

Hollywood::Hollywood(Warehouse *warehouse)
{
	mWarehouse = warehouse;

	mWarehouse->addItem("assets/background.png");
	mWarehouse->addItem("assets/goat.png");

	mWarehouse->addItem("assets/villain/stopped.png");

	mWarehouse->addItem("assets/test/red1.png");
	mWarehouse->addItem("assets/test/red2.png");
	mWarehouse->addItem("assets/test/red3.png");
	mWarehouse->addItem("assets/test/red4.png");

	mWarehouse->addItem("assets/test/p1.png");
	mWarehouse->addItem("assets/test/p2.png");
	mWarehouse->addItem("assets/test/p3.png");
	mWarehouse->addItem("assets/test/p4.png");
}

Hollywood::~Hollywood()
{

}

Actor *Hollywood::createTestActor()
{
	Animation *ani = new Animation(0.5, "assets/test/red1.png",
		mWarehouse->getTexture("assets/test/red1.png"));
	ani->addFrame("assets/test/red2.png", mWarehouse->getTexture("assets/test/red2.png"));
	ani->addFrame("assets/test/red3.png", mWarehouse->getTexture("assets/test/red3.png"));
	ani->addFrame("assets/test/red4.png", mWarehouse->getTexture("assets/test/red4.png"));

	Animation *ani2 = new Animation(0.5, "assets/test/p1.png",
		mWarehouse->getTexture("assets/test/p1.png"));
	ani2->addFrame("assets/test/p2.png", mWarehouse->getTexture("assets/test/p2.png"));
	ani2->addFrame("assets/test/p3.png", mWarehouse->getTexture("assets/test/p3.png"));
	ani2->addFrame("assets/test/p4.png", mWarehouse->getTexture("assets/test/p4.png"));

	Actor *a = new Actor(ani);
	a->addAnimation("PURPLE", ani2);

	return a;
}

Actor *Hollywood::createVillain()
{
	Animation *ani = new Animation(1000, "assets/villain/stopped.png",
		mWarehouse->getTexture("assets/villain/stopped.png"));
	Actor *act = new Actor(ani);
	return act;
}

void Hollywood::deleteActor(Actor *actor)
{
	std::vector<sf::String> list = actor->getAssetList();
	for(unsigned int i = 0; i < list.size(); i++)
		mWarehouse->dereference(list[i]);
	delete actor;
}