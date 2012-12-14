#pragma once

#include <iostream>

#include "warehouse.hpp"

Warehouse::Warehouse()
{

}

Warehouse::~Warehouse()
{
	// Make sure all items are dereferenced.
	std::map<sf::String, int>::iterator refIter;
	for(refIter = mReferences.begin(); refIter != mReferences.end(); refIter++)
		if(refIter->second > 0)
			std::cout << "Warehouse: Resource was not dereferenced: " <<
				(std::string)refIter->first << std::endl;
	mReferences.clear();

	// Delete all SoundBuffers from warehouse.
	std::map<sf::String, sf::SoundBuffer*>::iterator soundIter;
	for(soundIter = mSounds.begin(); soundIter != mSounds.end(); soundIter++)
		delete soundIter->second;
	mSounds.clear();

	// Delete all Textures from warehouse.
	std::map<sf::String, sf::Texture*>::iterator texIter;
	for(texIter = mTextures.begin(); texIter != mTextures.end(); texIter++)
		delete texIter->second;
	mTextures.clear();
}

void Warehouse::addItem(sf::String resourceName)
{
	sf::Texture *t;
	sf::SoundBuffer *s;
	switch(resourceType(resourceName))
	{
	case 1:	// If it's a texture...
		t = new sf::Texture();
		if(!t->loadFromFile(resourceName))
		{
			std::cout << "Warehouse: Graphic not found: " <<
				(std::string)resourceName << std::endl;
			delete t;
			return;
		}
		mTextures.insert(std::make_pair(resourceName, t));
		mReferences.insert(std::make_pair(resourceName, 0));
		break;
	case 2:	// If it's a sound...
		s = new sf::SoundBuffer();
		if(!s->loadFromFile(resourceName))
		{
			std::cout << "Warehouse: Sound not found: " <<
				(std::string)resourceName << std::endl;
			delete s;
			return;
		}
		mSounds.insert(std::make_pair(resourceName, s));
		mReferences.insert(std::make_pair(resourceName, 0));
		break;
	default:	// What is this I don't even...
		std::cout << "Warehouse: Tried to load invalid resource: " <<
			(std::string)resourceName <<std::endl;
		break;
	}
}

void Warehouse::dereference(sf::String resourceName)
{
	std::map<sf::String, int>::iterator iter = mReferences.find(resourceName);
	if(iter == mReferences.end())
	{
		std::cout << "Warehouse: Tried to dereference invalid resource: " <<
			(std::string)resourceName << std::endl;
		return;
	}
	if(iter->second < 1)
	{
		std::cout << "Warehouse: Tried to deference an unreferenced resource: " <<
			(std::string)resourceName << std::endl;
		return;
	}
	iter->second--;	// The actual dereferencing part.
}

sf::SoundBuffer *Warehouse::getSound(sf::String resourceName)
{
	std::map<sf::String, sf::SoundBuffer*>::iterator iter = mSounds.find(resourceName);
	if(iter == mSounds.end())
	{
		std::cout << "Warehouse: Tried to get invalid sound: " <<
			(std::string)resourceName << std::endl;
		return 0;
	}
	mReferences.find(resourceName)->second++;
	return iter->second;
}

sf::Texture *Warehouse::getTexture(sf::String resourceName)
{
	std::map<sf::String, sf::Texture*>::iterator iter = mTextures.find(resourceName);
	if(iter == mTextures.end())
	{
		std::cout << "Warehouse: Tried to get invalid texture: " <<
			(std::string)resourceName << std::endl;
		return 0;
	}
	mReferences.find(resourceName)->second++;
	return iter->second;
}

int Warehouse::resourceType(sf::String resource)
{
	if(resource.isEmpty())
		return 0;	// Invalid
	if(resource.find(".png", resource.getSize() - 4) != sf::String::InvalidPos)
		return 1;	// Texture
	if(resource.find(".ogg", resource.getSize() - 4) != sf::String::InvalidPos)
		return 2;	// Sound
	return 0;	// No valid types.
}
