#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

#include <map>

/*
	NOTES ON REFERENCES

	Every resource has a reference count. Every time you
	access a resource, you add one to the reference count.
	When you are finished with a resource, you must dereference
	it. This is to make sure that no resources are being used
	when the Warehouse is closed. This is also useful for
	preventing memory leaks!
*/

class Warehouse
{
public:

	/*
		CONSTRUCTOR
		Creates an empty warehouse for us to store resources in.
	*/
	Warehouse();

	/*
		DESTRUCTOR
		Deletes all resources and them closes Warehouse.
		Throws sf::String("Warehouse: Some resources not dereferenced.") if
		the reference count is greater than zero for any resources.
	*/
	~Warehouse();

	/*
		void addItem(sf::String resourceName);
		Add an image or sound to the list of resources.
		TODO: GET ANGRY WHEN YOU ADD INVALID RESOURCE.
	*/
	void addItem(sf::String resourceName);

	/*
		void dereference(sf::String resourceName);
		Dereference (subtract one from the ref count)
		for resource resourceName.
	*/
	void dereference(sf::String resourceName);

	/*
		sf::SoundBuffer getSound(sf::String resourceName);
		Retrieve stored sound data that matches the resourceName.
		Also adds one to the reference count of resourceName.
		TODO: GET ANGRY WHEN YOU ASK FOR INVALID RESOURCE.
	*/
	sf::SoundBuffer *getSound(sf::String resourceName);

	/*
		sf::Texture getTexture(sf::String resourceName);
		Retrieved stored texture data that matches the resourceName.
		Also adds one to the reference count of resourceName.
		TODO: GET ANGRY WHEN YOU ASK FOR INVALID RESOURCE.
	*/
	sf::Texture *getTexture(sf::String resourceName);

private:

	/*
		int resourceType(sf::String resource);
		Check to see the type of resource according to file extension.
		1 = Texture, 2 = Sound, Else = INVALID
	*/
	int resourceType(sf::String resource);

	std::map<sf::String, int> mReferences;	// Keeps the resource ref count.
	std::map<sf::String, sf::SoundBuffer*> mSounds;
	std::map<sf::String, sf::Texture*> mTextures;
};