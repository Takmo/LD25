#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <list>

#include "cinema.hpp"
#include "hollywood.hpp"

class Level : public CinemaViewer
{
public:
	Level(std::string levelName, Cinema *cinema, Hollywood *hollywood);

	~Level();

	void drawInterface();

	void drawLevel();

	/*
		Return 0 if nothing, 1 if beaten, or -1 if dead.
	*/
	int tick(float time);

	void keyPressed(sf::Keyboard::Key key);

	void lostFocus();

private:

	void applyGravity(float time);

	void applyJump(float time);

	void loadConfig(std::string levelName);

	// Plant the goat to explode.
	void plantGoat();

	// Check for collisions, boundaries, etc.
	void tryMovement(float x, float y);

	void updateScore();

	void updateTimer();

	Cinema *mCinema;
	Hollywood *mHollywood;

	bool mIsPaused;	// Is it paused?
	sf::Texture mPauseTexture;
	sf::Sprite mPauseSprite;

	bool mIsDead;	// Is the player dead?
	bool mIsWon;	// Has the player won?
	bool mHasConfirmed;	// Has the player confirmed it?

	sf::Font mFont;
	sf::Text mDeadText;
	sf::Text mDeadTextConfirm;
	sf::Text mPausedText;
	sf::Text mPausedVibrationText;
	sf::Text mScoreText;
	int mScore;
	sf::Text mTimerText;
	sf::Text mWinText;
	sf::Text mWinTextConfirm;

	std::vector<Actor*> mPhysicsActors;	// Check these for collisions.
	std::list<Actor*> mVictims;	// Check these for explosions.

	Actor *mGoat;	// This is the goat. Lol.
	Actor *mGoatShed;	// Dispenses goats.
	Actor *mPlayer;	// This is the player.
	float mJumpVelocity;	// The velocity of jump. Gravity decreases.
	bool mHasGoat;	// True if player is holding goat.

	sf::Sprite mBackgroundSprite;	// The background sprite.
	sf::Texture mBackgroundTexture;	// The background texture.

	float mGoatDetonationTime;	// Time before goat explodes. Negative = disarmed.
	float mGoatTimer;	// Current time.

	sf::SoundBuffer mExplosionBuffer;	// Explosion sound data.
	sf::Sound mExplosionSound;	// Actual explosion sound.
	sf::SoundBuffer mJumpBuffer;
	sf::Sound mJumpSound;
	sf::SoundBuffer mGetGoatBuffer;
	sf::Sound mGetGoatSound;
	sf::SoundBuffer mSetGoatBuffer;
	sf::Sound mSetGoatSound;
};