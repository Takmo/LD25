#pragma once

#include "level.hpp"

#include <fstream>
#include <sstream>

std::vector<std::string> stringSplit(const std::string &source, const char *delimiter = " ", bool keepEmpty = false);

Level::Level(std::string levelName, Cinema *cinema, Hollywood *hollywood)
{
	mCinema = cinema;
	mHollywood = hollywood;
	mIsPaused = false;

	mIsDead = false;
	mIsWon = false;
	mHasConfirmed = false;

	mScore = 0;

	// Setup font.
	mFont.loadFromFile("assets/PressStart2P.ttf");
	mPausedText = sf::Text("PAUSED", mFont, 50);
	mPausedText.setOrigin(mPausedText.getLocalBounds().width / 2,
		mPausedText.getLocalBounds().height / 2);
	mPausedText.setPosition(512, 384);
	mPausedVibrationText = sf::Text("Vibration: Off", mFont, 16);
	mPausedVibrationText.setOrigin(mPausedVibrationText.getLocalBounds().width / 2,
		mPausedVibrationText.getLocalBounds().height / 2);
	mPausedVibrationText.setPosition(512, 430);
	mDeadText = sf::Text("WASTED", mFont, 50);
	mDeadText.setOrigin(mDeadText.getLocalBounds().width / 2,
		mDeadText.getLocalBounds().height / 2);
	mDeadText.setPosition(512, 384);
	mDeadTextConfirm = sf::Text("(Press ENTER to try again!)", mFont, 16);
	mDeadTextConfirm.setOrigin(mDeadTextConfirm.getLocalBounds().width / 2,
		mDeadTextConfirm.getLocalBounds().height / 2);
	mDeadTextConfirm.setPosition(512, 430);
	mWinText = sf::Text("You're the goat!", mFont, 50);
	mWinText.setOrigin(mWinText.getLocalBounds().width / 2,
		mWinText.getLocalBounds().height / 2);
	mWinText.setPosition(512, 384);
	mWinTextConfirm = sf::Text("(That means you win. Press ENTER to continue!)", mFont, 16);
	mWinTextConfirm.setOrigin(mWinTextConfirm.getLocalBounds().width / 2,
		mWinTextConfirm.getLocalBounds().height / 2);
	mWinTextConfirm.setPosition(512, 430);
	updateScore();
	updateTimer();

	// Load the pause screen stuff.
	sf::Image i;
	i.create(1024, 768, sf::Color(0, 0, 0, 170));
	mPauseTexture.loadFromImage(i);
	mPauseSprite.setTexture(mPauseTexture);

	// Set this as input listener.
	mCinema->setViewer(this);

	// Load player, goatshed, and goat.
	mPlayer = mHollywood->createVillain();
	mGoat = mHollywood->createGoat();
	mGoat->setVisible(false);
	mGoatShed = mHollywood->createGoatShed();
	mJumpVelocity = 0;
	mHasGoat = false;

	// Load the configuration.
	loadConfig(levelName);

	// Exploding goat stuff.
	mGoatDetonationTime = -1;

	// Load the sounds.
	mExplosionBuffer.loadFromFile("assets/explosion.wav");
	mExplosionSound.setBuffer(mExplosionBuffer);
	mJumpBuffer.loadFromFile("assets/jump.wav");
	mJumpSound.setBuffer(mJumpBuffer);
	mGetGoatBuffer.loadFromFile("assets/getgoat.wav");
	mGetGoatSound.setBuffer(mGetGoatBuffer);
	mSetGoatBuffer.loadFromFile("assets/setgoat.wav");
	mSetGoatSound.setBuffer(mSetGoatBuffer);
}

Level::~Level()
{
	mHollywood->deleteActor(mGoatShed);
	mHollywood->deleteActor(mGoat);
	mHollywood->deleteActor(mPlayer);
	for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
		mHollywood->deleteActor(*i);
	for(unsigned int i = 0; i < mPhysicsActors.size(); i++)
		mHollywood->deleteActor(mPhysicsActors[i]);
}

void Level::drawInterface()
{
	// Reset camera position for drawing interfaces and such.
	mCinema->resetCamera();

	mCinema->draw(&mScoreText);
	mCinema->draw(&mTimerText);

	if(mIsDead)
	{
		mCinema->draw(&mPauseSprite);
		mCinema->draw(&mDeadText);
		mCinema->draw(&mDeadTextConfirm);
		return;
	}

	if(mIsWon)
	{
		mCinema->draw(&mPauseSprite);
		mCinema->draw(&mWinText);
		mCinema->draw(&mWinTextConfirm);
		return;
	}
	
	// Draw pause screen if necessary.
	if(mIsPaused)
	{
		mCinema->draw(&mPauseSprite);
		mCinema->draw(&mPausedText);
		mCinema->draw(&mPausedVibrationText);
	}
}

void Level::drawLevel()
{
	mCinema->draw(&mBackgroundSprite);

	// Set the camera position.
	mCinema->setPosition(mPlayer->getPosition());

	mCinema->draw(mGoatShed);
	for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
		mCinema->draw(*i);
	mCinema->draw(mGoat);
	mCinema->draw(mPlayer);
	for(unsigned int i = 0; i < mPhysicsActors.size(); i++)
		mCinema->draw(mPhysicsActors[i]);
}

int Level::tick(float time)
{
	// If confirmed, return dead or won.
	if(mHasConfirmed && mIsDead)
		return -1;
	if(mHasConfirmed && mIsWon)
		return 1;
	if(mIsDead || mIsWon)
		return 0;

	// If paused cancel.
	if(mIsPaused)
		return 0;

	// Add to mGoatTimer;
	if(mGoatDetonationTime > 0)
		mGoatTimer += time;

	// Check for movement.
	if(mCinema->isKeyDown(sf::Keyboard::A))
		tryMovement(-400 * time, 0);
	if(mCinema->isKeyDown(sf::Keyboard::D))
		tryMovement(400 * time, 0);

	// Apply gravity and jumping.
	applyGravity(time);
	applyJump(time);

	// Kill player if below Y=3000
	if(mPlayer->getPosition().y > 2000)
	{
		mPlayer->setVisible(false);
		mIsDead = true;
		return 0;
	}

	// Update timer if you have a goat.
	if(mGoatDetonationTime > 0)
		updateTimer();

	// Check goat explosion.
	if(mGoatDetonationTime > 0 && mGoatTimer > mGoatDetonationTime)
	{
		mExplosionSound.play();
		mGoat->setVisible(false);
		if(mHasGoat)
		{
			mPlayer->setVisible(false);
			mIsDead = true;
			return 0;
		}
		mHasGoat = false;
		sf::FloatRect explosion = mGoat->getBounds();
		explosion.top -= 100;
		explosion.height += 100;
		explosion.left -= 100;
		explosion.width += 100;
		if(explosion.intersects(mPlayer->getBounds()))
		{
			mPlayer->setVisible(false);
			mIsDead = true;
			return 0;
		}
		for(std::list<Actor*>::iterator i = mVictims.begin(); i != mVictims.end(); i++)
			if(explosion.intersects((*i)->getBounds()))
			{
				mHollywood->deleteActor(*i);
				mVictims.remove(*i);
				i = mVictims.begin();
				// Set score.
				if(mGoatDetonationTime == 3)
					mScore += 500;
				if(mGoatDetonationTime == 5)
					mScore += 400;
				if(mGoatDetonationTime == 7)
					mScore += 300;
				if(mGoatDetonationTime == 9)
					mScore += 200;
				if(mGoatDetonationTime == 11)
					mScore += 100;
				mScore += 50;
				updateScore();
				if(mVictims.size() == 0)
				{
					mIsWon = true;
					return 0;
				}
			}
		mGoatDetonationTime = -1;
		updateTimer();
	}

	// Tick player and goat animations.
	mPlayer->tick(time);
	mGoat->tick(time);
	
	return 0;
}

void Level::keyPressed(sf::Keyboard::Key key)
{
	// Check confirmation on death or winning.
	if(key == sf::Keyboard::Return && (mIsDead || mIsWon))
		mHasConfirmed = true;

	// If dead or won, ignore other things.
	if(mIsDead || mIsWon)
		return;

	// Pause if necessary.
	if(key == sf::Keyboard::Escape && !mIsDead && !mIsWon)
		mIsPaused = !mIsPaused;

	// Don't do anything if paused.
	if(mIsPaused)
		return;

	if(key == sf::Keyboard::Space && mJumpVelocity == 0)
	{
		mJumpVelocity = 350;
		mJumpSound.play();
	}
	if(key == sf::Keyboard::S && mHasGoat && mGoatDetonationTime > 0)
		plantGoat();
	
	// Check for number keys to set timer.
	if(key == sf::Keyboard::Num1 && !mHasGoat && mGoatDetonationTime < 0 && 
		mPlayer->getBounds().intersects(mGoatShed->getBounds()))
	{
		mGetGoatSound.play();
		mHasGoat = true;
		mGoatDetonationTime = 3;
		mGoatTimer = 0;
	}
	if(key == sf::Keyboard::Num2 && !mHasGoat && mGoatDetonationTime < 0 && 
		mPlayer->getBounds().intersects(mGoatShed->getBounds()))
	{
		mGetGoatSound.play();
		mHasGoat = true;
		mGoatDetonationTime = 5;
		mGoatTimer = 0;
	}
	if(key == sf::Keyboard::Num3 && !mHasGoat && mGoatDetonationTime < 0 && 
		mPlayer->getBounds().intersects(mGoatShed->getBounds()))
	{
		mGetGoatSound.play();
		mHasGoat = true;
		mGoatDetonationTime = 7;
		mGoatTimer = 0;
	}
	if(key == sf::Keyboard::Num4 && !mHasGoat && mGoatDetonationTime < 0 && 
		mPlayer->getBounds().intersects(mGoatShed->getBounds()))
	{
		mGetGoatSound.play();
		mHasGoat = true;
		mGoatDetonationTime = 9;
		mGoatTimer = 0;
	}
	if(key == sf::Keyboard::Num5 && !mHasGoat && mGoatDetonationTime < 0 && 
		mPlayer->getBounds().intersects(mGoatShed->getBounds()))
	{
		mGetGoatSound.play();
		mHasGoat = true;
		mGoatDetonationTime = 11;
		mGoatTimer = 0;
	}
}

void Level::lostFocus()
{
	if(!mIsDead && !mIsWon)
		mIsPaused = true;
}

void Level::applyGravity(float time)
{
	// If jumping, decrease jump velocity.
	if(mJumpVelocity > 0)
	{
		mJumpVelocity -= 450 * time;
		if(mJumpVelocity < 0)
			mJumpVelocity = -1;
		return;
	}

	// If not jumping, try moving down.
	tryMovement(0, 400 * time);
}

void Level::applyJump(float time)
{
	if(mJumpVelocity > 0)
		tryMovement(0, -mJumpVelocity * time);
}

void Level::loadConfig(std::string levelName)
{
	mBackgroundTexture.loadFromFile(levelName + "/background.png");
	mBackgroundSprite.setTexture(mBackgroundTexture);

	// Loading and stuff. Stole code from Stack Overflow.
	std::vector<std::vector<std::string>> mLines;
	std::ifstream cf;
	cf.open(levelName + "/level.txt");
	if(!cf.is_open())
	{
		puts("COULD NOT OPEN LEVEL FILE.");
		return;
	}

	while(!cf.eof())
	{
		std::string buffer;
		std::getline(cf, buffer);

		// Split into lines divided by ':'
		mLines.push_back(stringSplit(buffer, ":"));
	}
	cf.close();

	// Start positioning things.
	float x = (float) atof(mLines[0][0].c_str());
	float y = (float) atof(mLines[0][1].c_str());
	mPlayer->setPosition(x, y);
	x = (float) atof(mLines[1][0].c_str());
	y = (float) atof(mLines[1][1].c_str());
	mGoatShed->setPosition(x, y);

	// Add items and actors, etc...
	for(unsigned int i = 2; i < mLines.size(); i++)
	{
		if(mLines[i][0] == "add")
			mHollywood->addItem(mLines[i][1]);
		if(mLines[i][0] == "platform")
		{
			Actor *a = mHollywood->createPlatform(mLines[i][1]);
			a->setPosition((float) atof(mLines[i][2].c_str()),
				(float) atof(mLines[i][3].c_str()));
			mPhysicsActors.push_back(a);
		}
		if(mLines[i][0] == "victim")
		{
			Actor *a = mHollywood->createVictim(mLines[i][1]);
			a->setPosition((float) atof(mLines[i][2].c_str()),
				(float) atof(mLines[i][3].c_str()));
			mVictims.push_back(a);
		}
	}
}

void Level::plantGoat()
{
	mSetGoatSound.play();
	mHasGoat = false;
	mGoat->setPosition(mPlayer->getPosition().x, mPlayer->getPosition().y + 5);
	mGoat->setVisible();
}

void Level::tryMovement(float x, float y)
{
	// The current player's bounds.
	sf::FloatRect current = mPlayer->getBounds();
	
	// The testing bounds.
	sf::FloatRect test = current;

	// Check horizontal.
	test.left += x;
	for(unsigned int i = 0; i < mPhysicsActors.size(); i++)
		if(test.intersects(mPhysicsActors[i]->getBounds()))
			return;

	// Reset test;
	test = current;

	// Check vertical.
	test.top += y;
	for(unsigned int i = 0; i < mPhysicsActors.size(); i++)
		if(test.intersects(mPhysicsActors[i]->getBounds()))
		{
			if(y > 0)	// If collision is downward allow more jumping.
				mJumpVelocity = 0;
			if(y < 0)	// If collision is upward stop jump.
				mJumpVelocity = -1;
			return;
		}

	// If nothing intersected...
	mPlayer->translate(x, y);
}

void Level::updateScore()
{
	char buffer[13];
	sprintf_s(buffer, "Score: %d", mScore);
	mScoreText = sf::Text(buffer, mFont, 16);
	mScoreText.setOrigin(mScoreText.getLocalBounds().width / 2,
		mScoreText.getLocalBounds().height / 2);
	mScoreText.setPosition(512, 25);
}

void Level::updateTimer()
{
	if(mGoatDetonationTime > 0)
	{
		char buffer[13];
		sprintf_s(buffer, "%d", (int)(mGoatDetonationTime - mGoatTimer));
		mTimerText = sf::Text(buffer, mFont, 16);
	}
	else
		mTimerText = sf::Text("GRAB GOAT", mFont, 16);
	mTimerText.setOrigin(mTimerText.getLocalBounds().width / 2,
		mTimerText.getLocalBounds().height / 2);
	mTimerText.setPosition(512, 700);
}

// Pasted from http://stackoverflow.com/questions/10051679/c-tokenize-string

std::vector<std::string> stringSplit(const std::string &source, const char *delimiter, bool keepEmpty)
{
    std::vector<std::string> results;

    size_t prev = 0;
    size_t next = 0;

    while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
    {
        if (keepEmpty || (next - prev != 0))
        {
            results.push_back(source.substr(prev, next - prev));
        }
        prev = next + 1;
    }

    if (prev < source.size())
    {
        results.push_back(source.substr(prev));
    }

    return results;
}