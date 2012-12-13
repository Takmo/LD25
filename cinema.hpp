#pragma once

#include <SFML/System/Vector2.hpp>

#include "actor.hpp"

class Cinema
{
public:

	/*
		CONSTRUCTOR
		Create a Cinema. This includes the screen and window.
	*/
	Cinema();

	/*
		DESTRUCTOR
		Destroy the cinema. Make sure you delete everything
		else before you destroy the cinema!
	*/
	~Cinema();

	/*
		bool intersects(float x, float y);
		Returns true if (X, Y) is visible on the cinema screen.
	*/
	bool intersects(float x, float y);

	/*
		bool intersects(Actor *actor);
		Returns true if actor intersects the screen.
	*/
	bool intersects(Actor *actor);

	/*
		void draw(Actor *actor);
		Draws the actor if intersects(actor).
	*/
	void draw(Actor *actor);

	/*
		sf::Vector2f getPosition();
		Returns the center position of the cinema screen.
	*/
	sf::Vector2f getPosition();

	/*
		void render();
		Render one frame.
	*/
	void render();

	/*
		void setPosition(float x, float y);
		Sets the position of the center of the cinema screen.
	*/
	void setPosition(float x, float y);

	/*
		void translate(float x, float y);
		Translates the center of the cinema screen locally,
		rather than globally.
	*/
	void translate(float x, float y);
};