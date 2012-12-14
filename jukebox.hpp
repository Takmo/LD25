#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/System/String.hpp>

#include <map>

class Jukebox
{
public:

	// Constructor
	Jukebox();

	// Destructor
	~Jukebox();

	void addMusic(sf::String musicFile);

	void playMusic(sf::String music);

	void pauseMusic(sf::String music);

	void stopMusic(sf::String music);
	
private:

	std::map<sf::String, sf::Music*> mMusic;
};