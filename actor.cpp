#pragma once

#include "actor.hpp"

Actor::Actor(sf::String defaultName, sf::Texture *defaultTexture, float x, float y, bool visible)
{
	mDefaultTexture = defaultTexture;
	mDefaultTextureName = defaultName;
	mSprite.setTexture(*mDefaultTexture);
	mSprite.setPosition(x, y);
	mVisible = visible;
	mCurrentAnimation = mAnimations.end();;
}

Actor::~Actor()
{
	// Delete the animations.
	std::map<sf::String, Animation*>::iterator ai;
	for(ai = mAnimations.begin(); ai != mAnimations.end(); ai++)
		delete ai->second;
	mAnimations.clear();
	
	// Delete the sounds.
	std::map<sf::String, sf::Sound*>::iterator si;
	for(si = mSounds.begin(); si != mSounds.end(); si++)
		delete si->second;
	mSounds.clear();
}

void Actor::addAnimation(sf::String name, Animation *animation)
{
	mAnimations.insert(std::make_pair(name, animation));
}

void Actor::addSound(sf::String name, sf::SoundBuffer *sound)
{
	mSounds.insert(std::make_pair(name, new sf::Sound(*sound)));
}

sf::FloatRect Actor::getBounds()
{
	return mSprite.getGlobalBounds();
}

sf::Sprite *Actor::getSprite()
{
	return &mSprite;
}

std::vector<sf::String> Actor::getAssetList()
{
	std::vector<sf::String> r;
	r.push_back(mDefaultTextureName);
	std::map<sf::String, Animation*>::iterator ai;
	for(ai = mAnimations.begin(); ai != mAnimations.end(); ai++)
	{
		std::vector<sf::String>::iterator ti;
		for(ti = ai->second->getFrameList().begin(); ti != ai->second->getFrameList().end(); ti++)
			r.push_back(*ti);
	}
	std::map<sf::String, sf::Sound*>::iterator si;
	for(si = mSounds.begin(); si != mSounds.end(); si++)
		r.push_back(si->first);
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
