#pragma once

#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Lane.h"
#include <vector>
#include "SFML/Audio.hpp"

enum class Screen
{
	MENU,
	GAME,
	END
};

class Game
{
public:
	Game(unsigned int &screenWidth, unsigned int &screenHeight, const char* &title);
	~Game();
	/// Return Value
	/// true = Continue to run program
	/// false = Quits program
	bool Update(float deltaTime); 
	void Draw(sf::RenderWindow& window);
private:
	void CheckInput();
	void NextLevel();
	void IncreaseScore(int amount);
	void SaveHighscore(std::string name, int score);
	void GetHighscore();
	std::string highscoreName[5];
	int highscorePoints[5];


	Screen screen = Screen::MENU;
	float menuCooldown = 0;
	float menuBoxTimer = 0;
	bool showingHighscore = false;

	bool scoreChecked = false;
	bool newHighscore = false;

	sf::Text newNameText[4];
	sf::Texture selectorArrowTexture;
	sf::Sprite selectorArrow;

	std::string letters[26] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
	int letterSelectorIndex = 0;
	int nameIndex[4] = { 0, 0, 0, 0 };

	int level = 0;
	int score = 0;

	float totalTime = 0;
	float levelTime = 0;
	int lilypadsCompleted = 0;

	int rowSteppedOn = 0;

	float transitionTimer = 0;
	float timeForTransition = 3;

	Player player;
	std::vector<Lane> laneList;

	sf::Texture grassTexture;
	sf::Texture roadTexture;
	sf::Texture waterTexture;

	std::vector<sf::Texture> carTextureList;
	std::vector<sf::Texture> logTextureList;
	sf::Texture lilypadTexture;
	sf::Texture goldenLilypadTexture;

	sf::Texture HUDTexture;
	sf::Sprite HUD;

	sf::Texture timeTexture;
	sf::Texture scoreTexture;
	sf::Texture topTexture;
	sf::Texture heartTexture;

	sf::Sprite timeSprite;
	sf::Sprite scoreSprite;
	sf::Sprite topSprite;
	sf::Sprite heartSprite;

	sf::Font font;
	
	sf::Text timeText;
	sf::Text scoreText;
	sf::Text topText;

	// 0 = Grass
	// 1 = Road
	// 2 = River
	int maps[4][13] = { { 1, 1, 1, 0, 1, 1, 1, 0, 2, 2, 2, 2, 2 }, 
						{ 1, 1, 0, 2, 2, 2, 0, 1, 1, 1, 0, 1, 2 },
						{ 2, 2, 0, 1, 1, 1, 1, 0, 2, 2, 0, 2, 2 },
						{ 1, 1, 1, 1, 1, 0, 2, 2, 2, 2, 1, 2, 2}};


	sf::Texture menuTexture;
	sf::Sprite menuBackground;
	sf::Text boxHeader;
	sf::Text boxText[5];
	std::string pointTable[4] = { "50 PTS PER STEP", "500 PTS PER LILYPAD", "LILYPAD PTS LOWER BY TIME", "1000 PTS PER LEVEL" };

	sf::Texture endTexture;
	sf::Sprite endBackground;
	sf::Text yourScore;
	sf::Text endGuide;

	
	sf::Music music;

	sf::SoundBuffer crashBuffer;
	sf::SoundBuffer drownBuffer;
	sf::SoundBuffer lilypadBuffer;

	sf::Sound crashSound;
	sf::Sound drownSound;
	sf::Sound lilypadSound;
};