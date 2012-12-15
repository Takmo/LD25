#pragma once

#include "animation.hpp"

Animation::Animation(double frameTime, sf::String firstName, sf::Texture *firstTexture)
{
	mCurrentTime = 0;
	mFrameTime = frameTime;
	mFrames.push_back(firstTexture);
	mAssetList.push_back(firstName);
	mCurrentFrame = mFrames.begin();
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
	return *mCurrentFrame;
}

std::vector<sf::String> Animation::getFrameList()
{
	return mAssetList;
}

bool Animation::tick(double time)
{
	mCurrentTime += time;
	if(mCurrentTime >= mFrameTime)
	{
		mCurrentFrame++;
		mCurrentTime = 0;
		if(mCurrentFrame == mFrames.end())
		{
			mCurrentFrame = mFrames.begin();
			return true;
		}
	}
	return false;
}