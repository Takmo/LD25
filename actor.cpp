#pragma once

#include "actor.hpp"

Actor::Actor(sf::Texture *defaultTexture, float x, float y, bool visible)
{
	mDefaultTexture = defaultTexture;
	mSprite.setTexture(*mDefaultTexture);
	mSprite.setPosition(x, y);
	mVisible = visible;
}

Actor::~Actor()
{

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

sf::FloatRect Actor::getBounds()
{
	return mSprite.getGlobalBounds();
}

const sf::Texture *Actor::getCurrentTexture()
{
	return mSprite.getTexture();
}

sf::Vector2f Actor::getPosition()
{
	return mSprite.getPosition();
}

void Actor::setPosition(float x, float y)
{
	mSprite.setPosition(x, y);
}

void Actor::setVisible(bool visible)
{
	mVisible = visible;
}

void Actor::translate(float x, float y)
{
	sf::Vector2f oldPos = mSprite.getPosition();
	mSprite.setPosition(oldPos.x + x, oldPos.y + y);
}