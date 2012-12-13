#pragma once

#include "cinema.hpp"

Cinema::Cinema()
{
	mRenderWindow.create(sf::VideoMode(800, 600), "Ludum Dare 25", sf::Style::Close);
	mRenderWindow.setVerticalSyncEnabled(true);
	mRenderWindow.display();
}

Cinema::~Cinema()
{
	mRenderWindow.close();
}

void Cinema::render()
{
	mRenderWindow.display();
}