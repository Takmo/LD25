#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>

#include <map>

class Jukebox {
public:
	// Constructor
	Jukebox();
	// Destructor
	~Jukebox();
	bool addSound(sf::String soundFile);
	bool addMusic(sf::String musicFile);
	
	void playSound(sf::String sound);

	void playMusic(sf::String music);
	void pauseMusic(sf::String music);
	void stopMusic(sf::String music);
	

};