#pragma once

#include "level.hpp"

Level::Level(sf::String configFile, Cinema *cinema, Hollywood *hollywood)
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
	loadConfig(configFile);

	// Exploding goat stuff.
	mGoatDetonationTime = -1;

	// Load the sounds.
	mExplosionBuffer.loadFromFile("assets/explosion.wav");
	mExplosionSound.setBuffer(mExplosionBuffer);
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
		mGoatDetonationTime = -1;
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
				// CHANGE SCORE DEPENDING ON TIME LENGTH
				mHollywood->deleteActor(*i);
				mVictims.remove(*i);
				i--;
				mScore += 100;
				updateScore();
				if(mVictims.size() == 0)
				{
					mIsWon = true;
					return 0;
				}
			}
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
		mJumpVelocity = 350;
	if(key == sf::Keyboard::S && mHasGoat && mGoatDetonationTime > 0)
		plantGoat();
	// TODO REPLACE GET GOAT KEY FROM S TO NUMBERS
	if(key == sf::Keyboard::S && !mHasGoat && mGoatDetonationTime < 0 && 
		mPlayer->getBounds().intersects(mGoatShed->getBounds()))
	{
		mHasGoat = true;
		mGoatDetonationTime = 5;
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

void Level::loadConfig(sf::String configFile)
{
	// TODO FILL THIS IN WITH FILE.
	mBackgroundTexture.loadFromFile("assets/magma/background.png");
	mBackgroundSprite.setTexture(mBackgroundTexture);
	
	mGoatShed->setPosition(500, 465);
	mGoat->setPosition(570, 470);

	Actor *platform = mHollywood->createPlatform("assets/grass/platform.png");
	platform->setPosition(500, 500);
	mPhysicsActors.push_back(platform);

	platform = mHollywood->createPlatform("assets/magma/platform.png");
	platform->setPosition(500, 700);
	mPhysicsActors.push_back(platform);

	platform = mHollywood->createPlatform("assets/magma/platform.png");
	platform->setPosition(700, 600);
	mPhysicsActors.push_back(platform);

	Actor *vic = mHollywood->createVictim("assets/victims/rich.png");
	vic->setPosition(700, 565);
	mVictims.push_back(vic);

	mPlayer->setPosition(500, 0);
}

void Level::plantGoat()
{
	// TODO PLANT GOAT
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