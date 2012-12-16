#pragma once

#include "animation.hpp"

Animation::Animation(double frameTime, sf::String firstName, sf::Texture *firstTexture)
{
	mCurrentTime = 0;
	mFrameTime = frameTime;
	mFrames.push_back(firstTexture);
	mAssetList.push_back(firstName);
	mCurrentFrame = 0;
}

Animation::~Animation()
{
	mAssetList.clear();
	mFrames.clear();
}

void Animation::addFrame(sf::String name, sf::Texture *frame)
{
	mFrames.push_back(frame);
	mAssetList.push_back(name);
}

sf::Texture *Animation::getCurrentFrame()
{
	return mFrames[mCurrentFrame];
}

std::vector<sf::String> Animation::getFrameList()
{
	return mAssetList;
}

void Animation::reset()
{
	mCurrentFrame = 0;
	mCurrentTime = 0;
}

int Animation::tick(double time)
{
	mCurrentTime += time;
	if(mCurrentTime >= mFrameTime)
	{
		mCurrentFrame++;
		mCurrentTime = 0;
		if(mCurrentFrame == mFrames.size())
		{
			mCurrentFrame = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}