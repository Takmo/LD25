#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

#include <vector>

class Animation
{
public:

	Animation(double frameTime, sf::String firstName, sf::Texture *firstTexture);

	~Animation();

	void addFrame(sf::String name, sf::Texture *frame);

	sf::Texture *getCurrentFrame();

	std::vector<sf::String> getFrameList();

	/*
		bool tick(double time);
		Advance the animation by time.
		Return true if animation is finished.
	*/
	bool tick(double time);

protected:

	double mCurrentTime;	// The current time elapsed.
	double mFrameTime;	// The time between frames.
	std::vector<sf::Texture*>::iterator mCurrentFrame;
	std::vector<sf::Texture*> mFrames;
	std::vector<sf::String> mAssetList;	// Holds the names for later.
};