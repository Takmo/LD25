#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

#include <map>
#include <vector>

#include "animation.hpp"

class Actor
{
public:

	/*
		CONSTRUCTOR
		Create and actor with a default texture and position. Visible by default.
	*/
	Actor(Animation *defaultAnimation, bool visible = true);
	
	/*
		DESTRUCTOR
		Deletes the object. Does NOT delete the resources. That is
		delegated to Hollywood.
	*/
	~Actor();
	
	/*
		void addAnimation(sf::String name, Animation *animation);
		Creates a possible animation with the provided name.
		Can be played or stopped by referencing the name.
	*/
	void addAnimation(sf::String name, Animation *animation);
	
	/*
		bool intersects(float x, float y);
		Checks to see if the actor's bounds contain point (X, Y).
	*/
	bool intersects(float x, float y);
	
	/*
		bool intersects(Actor *other);
		Checks to see if other->getBounds() intersects
		with this->getBounds().
	*/
	bool intersects(Actor *other);

	/*
		sf::FloatRect getBounds();
		Returns the bounds of the actor.
	*/
	sf::FloatRect getBounds();

	/*
		sf::Sprite *getSprite();
		Returns the actor's sprite.
	*/
	sf::Sprite *getSprite();

	/*
		std::vector<sf::String> getAssetList();
		Returns a vector list of all assets.
		This includes both animation textures and sounds.
	*/
	std::vector<sf::String> getAssetList();

	/*
		sf::Vector2f getPosition();
		Returns the position of the actor.
	*/
	sf::Vector2f getPosition();

	/*
		bool isVisible();
		Returns true if the actor is visible.
	*/
	bool isVisible();

	/*
		void setAnimation(sf::String name);
		Sets the current animation.
	*/
	void setAnimation(sf::String name);
	
	/*
		void setPosition(float x, float y);
		Sets the current position to (X, Y).
	*/
	void setPosition(float x, float y);
	
	/*
		void setVisible(bool visible = true);
		Set the actor to be visible or invisible.
	*/
	void setVisible(bool visible = true);

	/*
		virtual void tick(double time);
		Ticks the current animation and performs other logic.
	*/
	virtual void tick(double time);
	
	/*
		void translate(float x, float y);
		Move the actor locally by (X, Y).
	*/
	void translate(float x, float y);
	
protected:

	bool mVisible;	// If the actor is invisible, don't draw.
	std::map<sf::String, Animation*>::iterator mCurrentAnimation;
	std::map<sf::String, Animation*> mAnimations;
	sf::Sprite mSprite;

};
