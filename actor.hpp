#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

#include <vector>

class Animation;
// TODO ADD ANIMATION!

class Actor
{
public:

	/*
		CONSTRUCTOR
		Create and actor with a default texture and position. Visible by default.
	*/
	Actor(sf::Texture *defaultTexture, float x, float y, bool visible = true);
	
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
		void addSound(sf::String name, sf::SoundBuffer *sound);
		Add a sound to the actor's soundbank that can be played
		at a later time.
	*/
	void addSound(sf::String name, sf::SoundBuffer *sound);
	
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
		std::vector<sf::String> getAnimationList();
		Returns a vector list of names of Animations.
	*/
	std::vector<sf::String> getAnimationList();

	/*
		sf::FloatRect getBounds();
		Returns the bounds of the actor.
	*/
	sf::FloatRect getBounds();

	/*
		const sf::Texture *getCurrentTexture();
		Returns the current texture of the actor.
	*/
	const sf::Texture *getCurrentTexture();

	/*
		std::vector<sf::String> getSoundList();
		Returns a vector list of names of sounds.
	*/
	std::vector<sf::String> getSoundList();

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
		void playAnimation(sf::String name);
		Sets name as the current playing animation.
		If no animation is found, have no animation.
	*/
	void playAnimation(sf::String name);
	
	/*
		void playSound(sf::String name);
		Play the sound with the slected name.
	*/
	void playSound(sf::String name);
	
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
		void stopAnimation();
		Stop the current animation and set the
		texture to default.
	*/
	void stopAnimation();

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
	sf::Sprite mSprite;
	sf::Texture *mDefaultTexture;

};
