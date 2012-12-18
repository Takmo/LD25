#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "actor.hpp"

class CinemaViewer
{
public:
	virtual void click(int x, int y);
	virtual void keyPressed(sf::Keyboard::Key key);
	virtual void keyReleased(sf::Keyboard::Key key);
	virtual void lostFocus();
};

class Cinema : public CinemaViewer
{
public:

	/*
		CONSTRUCTOR
		Create a Cinema. This includes the screen and window.
	*/
	Cinema();

	~Cinema();

	void close();

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
		void draw(Actor *actor);
		Draws the sprite.
	*/
	void draw(sf::Drawable *drawable);

	/*
		sf::Vector2f getPosition();
		Returns the center position of the cinema screen.
	*/
	sf::Vector2f getPosition();

	/*
		book isKeyDown(sf::Keyboard::Key key);
		Check if key is down.
	*/
	bool isKeyDown(sf::Keyboard::Key key);

	bool isOpen();

	/*
		bool pollEvents();
		Polls the window for events. Returns false if closed.
	*/
	bool pollEvents();

	/*
		void render();
		Render one frame.
	*/
	void render();

	/*
		void resetCamera();
		Reset the camera to default position.
		Useful for interface drawing.
	*/
	void resetCamera();

	/*
		void setPosition(float x, float y);
		Sets the position of the center of the cinema screen.
	*/
	void setPosition(float x, float y);

	/*
		void setPosition(float x, float y);
		Sets the position of the center of the cinema screen.
	*/
	void setPosition(sf::Vector2f position);

	/*
		void setViewer(CinemaViewer *viewer);
		Sets the current CinemaViewer that receives event notifications.
	*/
	void setViewer(CinemaViewer *viewer);

	/*
		void translate(float x, float y);
		Translates the center of the cinema screen locally,
		rather than globally.
	*/
	void translate(float x, float y);

private:

	CinemaViewer *mCinemaViewer;
	sf::RenderWindow mRenderWindow;	// The SFML RenderWindow.
	sf::View mView;
};