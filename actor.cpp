#pragma once

#include "actor.hpp"

#include <iostream>

Actor::Actor(sf::Texture *defaultTexture, float x, float y, bool visible)
{
	mDefaultTexture = defaultTexture;
	mSprite.setTexture(*mDefaultTexture);
	mSprite.setPosition(x, y);
	mVisible = visible;
}

Actor::~Actor()
{
	std::map<sf::String, sf::Sound*>::iterator i;
	for(i = mSounds.begin(); i != mSounds.end(); i++)
		delete i->second;
	mSounds.clear();
}

void Actor::addSound(sf::String name, sf::SoundBuffer *sound)
{
	mSounds.insert(std::make_pair(name, new sf::Sound(*sound)));
}

sf::FloatRect Actor::getBounds()
{
	return mSprite.getGlobalBounds();
}

const sf::Texture *Actor::getCurrentTexture()
{
	return mSprite.getTexture();
}

std::vector<sf::String> Actor::getSoundList()
{
	std::vector<sf::String> r;
	std::map<sf::String, sf::Sound*>::iterator i;
	for(i = mSounds.begin(); i != mSounds.end(); i++)
		r.push_back(i->first);
	return r;
}

sf::Vector2f Actor::getPosition()
{
	return mSprite.getPosition();
}

bool Actor::intersects(float x, float y)
{
	if(mSprite.getGlobalBounds().contains(x, y))
		return true;
	return false;
}

bool Actor::intersects(Actor *other)
{
	if(mSprite.getGlobalBounds().intersects(other->getBounds()))
		return true;
	return false;
}

bool Actor::isVisible()
{
	return mVisible;
}

void Actor::playSound(sf::String name)
{
	std::map<sf::String, sf::Sound*>::iterator i = mSounds.find(name);
	if(i == mSounds.end())
	{
		std::cout << "Actor: Tried to play nonexistant sound: " <<
			(std::string)name << std::endl;
		return;
	}
	i->second->play();
}

void Actor::setPosition(float x, float y)
{
	mSprite.setPosition(x, y);
}

void Actor::setVisible(bool visible)
{
	mVisible = visible;
}

void Actor::tick(double time)
{
	// TODO
}

void Actor::translate(float x, float y)
{
	sf::Vector2f oldPos = mSprite.getPosition();
	mSprite.setPosition(oldPos.x + x, oldPos.y + y);
}