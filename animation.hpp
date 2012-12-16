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

	// Start animation over.
	void reset();

	// 0 if nothing. 1 if new frame. -1 if reset.
	int tick(double time);

protected:

	double mCurrentTime;	// The current time elapsed.
	double mFrameTime;	// The time between frames.
	int mCurrentFrame;
	std::vector<sf::Texture*> mFrames;
	std::vector<sf::String> mAssetList;	// Holds the names for later.
};