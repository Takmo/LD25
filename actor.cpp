#pragma once

#include "actor.hpp"

Actor::Actor(Animation *defaultAnimation, bool visible)
{
	mVisible = visible;
	addAnimation("DEFAULT", defaultAnimation);
	mCurrentAnimation = mAnimations.begin();
}

Actor::~Actor()
{
	// Delete the animations.
	std::map<sf::String, Animation*>::iterator ai;
	for(ai = mAnimations.begin(); ai != mAnimations.end(); ai++)
		delete ai->second;
	mAnimations.clear();
}

void Actor::addAnimation(sf::String name, Animation *animation)
{
	mAnimations.insert(std::make_pair(name, animation));
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
	std::map<sf::String, Animation*>::iterator ai;
	for(ai = mAnimations.begin(); ai != mAnimations.end(); ai++)
	{
		std::vector<sf::String>::iterator ti;
		for(unsigned int n = 0; n < ai->second->getFrameList().size(); n++)
			r.push_back(ai->second->getFrameList()[n]);
	}
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

void Actor::setAnimation(sf::String name)
{
	if(mCurrentAnimation == mAnimations.find(name))
		return;
	mCurrentAnimation->second->reset();
	mCurrentAnimation = mAnimations.find(name);
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
	switch(mCurrentAnimation->second->tick(time))
	{
	case 0:
		break;
	default:
		mSprite.setTexture(*mCurrentAnimation->second->getCurrentFrame());
		break;
	}
}

void Actor::translate(float x, float y)
{
	sf::Vector2f oldPos = mSprite.getPosition();
	mSprite.setPosition(oldPos.x + x, oldPos.y + y);
}
