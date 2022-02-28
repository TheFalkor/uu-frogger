// Henrik Nilsson 2021-01-15
#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game(unsigned int &screenWidth, unsigned int &screenHeight, const char* &title)
{
	screenWidth = 640;
	screenHeight = 640;
	title = "Frogger";

	srand(time(0));

	// in-game HUD
	if (!HUDTexture.loadFromFile("../Assets/Sprites/HUD_Background.png"))
		system("pause");

	if (!timeTexture.loadFromFile("../Assets/Sprites/HUD_Time.png"))
		system("pause");

	if (!scoreTexture.loadFromFile("../Assets/Sprites/HUD_Score.png"))
		system("pause");

	if (!topTexture.loadFromFile("../Assets/Sprites/HUD_Top.png"))
		system("pause");

	if (!heartTexture.loadFromFile("../Assets/Sprites/HUD_Heart.png"))
		system("pause");

	if (!font.loadFromFile("../Assets/Font/arial.ttf"))
		system("pause");

	HUD.setTexture(HUDTexture);
	timeSprite.setTexture(timeTexture);
	timeSprite.setPosition(12, 12);
	timeText.setFont(font);
	timeText.setCharacterSize(24);
	timeText.setPosition(46, 9);
	timeText.setString("0");

	scoreSprite.setTexture(scoreTexture);
	scoreSprite.setPosition(12, 44);
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setPosition(46, 41);
	scoreText.setString("0");

	topSprite.setTexture(topTexture);
	topSprite.setPosition(640 - 200, 44);
	topText.setFont(font);
	topText.setCharacterSize(24);
	topText.setPosition(640 - 200 + 34, 41);
	topText.setString("- - -");

	heartSprite.setTexture(heartTexture);

	
	// Loading grass texture
	if (!grassTexture.loadFromFile("../Assets/Sprites/GrassLane.png"))
		system("pause");

	// Loading road texture
	if (!roadTexture.loadFromFile("../Assets/Sprites/RoadLane.png"))
		system("pause");

	// Loading Water texture
	if (!waterTexture.loadFromFile("../Assets/Sprites/WaterLane.png"))
		system("pause");

	// Loading car textures
	for (int i = 0; i < 3; i++)
	{
		carTextureList.push_back(sf::Texture());
	}
	for (int i = 0; i < carTextureList.size(); i++)
	{
		if (!carTextureList[i].loadFromFile("../Assets/Sprites/Car_" + std::to_string(i) + ".png"))
			system("pause");
	}

	// Loading log textures
	for (int i = 0; i < 2; i++)
	{
		logTextureList.push_back(sf::Texture());
	}
	for (int i = 0; i < logTextureList.size(); i++)
	{
		if (!logTextureList[i].loadFromFile("../Assets/Sprites/Log_" + std::to_string(i) + ".png"))
			system("pause");
	}

	// Loading lilypad texture
	if (!lilypadTexture.loadFromFile("../Assets/Sprites/Lilypad.png"))
		system("pause");
	if (!goldenLilypadTexture.loadFromFile("../Assets/Sprites/GoldenLilypad.png"))
		system("pause");


	// Menu
	if (!menuTexture.loadFromFile("../Assets/Sprites/MenuBackground.png"))
		system("pause");

	menuBackground.setTexture(menuTexture);
	boxHeader.setFont(font);
	boxHeader.setCharacterSize(35);
	boxHeader.setPosition(180, 330);
	boxHeader.setString("- POINT TABLE -");

	for (int i = 0; i < 5; i++)
	{
		boxText[i].setFont(font);
		boxText[i].setCharacterSize(22);
		boxText[i].setPosition(150, 380 + i * 32);
		if(i < 4)
			boxText[i].setString(pointTable[i]);
	}

	// End screen
	if (!endTexture.loadFromFile("../Assets/Sprites/EndBackground.png"))
		system("pause");

	endBackground.setTexture(endTexture);


	if (!selectorArrowTexture.loadFromFile("../Assets/Sprites/SelectorArrow.png"))
		system("pause");

	selectorArrow.setTexture(selectorArrowTexture);
	selectorArrow.setPosition(95, 370);
	for (int i = 0; i < 4; i++)
	{
		newNameText[i].setFont(font);
		newNameText[i].setCharacterSize(95);
		newNameText[i].setPosition(115 + i * 110, 400);
	}


	yourScore.setFont(font);
	yourScore.setCharacterSize(32);
	yourScore.setPosition(120, 290);

	endGuide.setFont(font);
	endGuide.setCharacterSize(25);
	endGuide.setPosition(105, 580);
	endGuide.setString("Press [SPACEBAR] to start game");
	
	GetHighscore();
	topText.setString(std::to_string(highscorePoints[0]));


	// Audio
	if (!drownBuffer.loadFromFile("../Assets/Audio/Drown.wav"))
		system("pause");
	drownSound.setBuffer(drownBuffer);

	if (!crashBuffer.loadFromFile("../Assets/Audio/Crash.wav"))
		system("pause");
	crashSound.setBuffer(crashBuffer);

	if (!lilypadBuffer.loadFromFile("../Assets/Audio/Lilypad.wav"))
		system("pause");
	lilypadSound.setBuffer(lilypadBuffer);

	if (!music.openFromFile("../Assets/Audio/Forest.wav"))
		system("pause");

	music.setLoop(true);
	music.play();
};

Game::~Game()
{
}

bool Game::Update(float deltaTime)
{
	if (screen == Screen::MENU)
	{
		menuBoxTimer += deltaTime;

		if (menuBoxTimer >= 5)
		{
			showingHighscore = !showingHighscore;
			menuBoxTimer = 0;
			if (showingHighscore)
			{
				boxHeader.setString("- HIGHSCORES -");
				for (int i = 0; i < 5; i++)
				{
					boxText[i].setString("#" + std::to_string(i + 1) + ": " + highscoreName[i] + " :: " + std::to_string(highscorePoints[i]));
				}
			}
			else
			{
				boxHeader.setString("- POINT TABLE -");
				for (int i = 0; i < 5; i++)
				{
					if (i < 4)
						boxText[i].setString(pointTable[i]);
					else
						boxText[i].setString("");
				}
			}

		}

		menuCooldown -= deltaTime;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && menuCooldown < 0)
		{
			menuCooldown = 0;
			NextLevel();
			screen = Screen::GAME;
		}
		return true;
	}
	if (screen == Screen::END)
	{
		menuCooldown -= deltaTime;
		if (menuCooldown > 0)
			return true;

		if (!scoreChecked)
		{
			yourScore.setString("YOUR SCORE: " + std::to_string(score));
			endGuide.setString("Press [SPACEBAR] to continue");
			for (int i = 0; i < 5; i++)
			{
				if (score > highscorePoints[i])
				{
					newHighscore = true;
					for (int i = 0; i < 4; i++)
						newNameText[i].setString("A");
					for (int i = 0; i < 4; i++)
						nameIndex[i] = 0;
					letterSelectorIndex = 0;
					selectorArrow.setPosition(95, 370);
					yourScore.setString("NEW HIGHSCORE: " + std::to_string(score));
					endGuide.setString("Press [SPACEBAR] to save highscore");
					break;
				}
			}

			scoreChecked = true;
		}

		if (newHighscore)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				nameIndex[letterSelectorIndex]--;
				if (nameIndex[letterSelectorIndex] < 0)
					nameIndex[letterSelectorIndex] = 25;
				menuCooldown = 0.15f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				nameIndex[letterSelectorIndex]++;
				if (nameIndex[letterSelectorIndex] > 25)
					nameIndex[letterSelectorIndex] = 0;
				menuCooldown = 0.15f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				letterSelectorIndex--;
				if (letterSelectorIndex < 0)
					letterSelectorIndex = 3;
				menuCooldown = 0.15f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				letterSelectorIndex++;
				if (letterSelectorIndex > 3)
					letterSelectorIndex = 0;
				menuCooldown = 0.15f;
			}

			if (menuCooldown > 0)
			{
				selectorArrow.setPosition(95 + letterSelectorIndex * 110, 370);
				newNameText[letterSelectorIndex].setString(letters[nameIndex[letterSelectorIndex]]);
			}

		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (newHighscore)
			{
				std::string name = letters[nameIndex[0]] + letters[nameIndex[1]] + letters[nameIndex[2]] + letters[nameIndex[3]];
				SaveHighscore(name, score);
				GetHighscore();
			}
			topText.setString(std::to_string(highscorePoints[0]));

			newHighscore = false;
			scoreChecked = false;
			showingHighscore = true;
			menuBoxTimer = 5;
			score = 0; 
			rowSteppedOn = 0;
			lilypadsCompleted = 0;
			totalTime = 0;
			levelTime = 0;
			level = 0;
			player.health = 3;
			endGuide.setString("Press [SPACEBAR] to start game");

			scoreText.setString("0");

			laneList.clear();

			menuCooldown = 1;
			screen = Screen::MENU;
		}

		return true;
	}


	for (int i = 0; i < laneList.size(); i++)
	{
		laneList[i].Update(deltaTime);
	}

	if (transitionTimer > 0)
	{
		transitionTimer -= deltaTime;

		if (transitionTimer < 0)
			transitionTimer = 0;

		float percentage = 1 - transitionTimer / timeForTransition;
		
		player.Transition(percentage);
		
		for (int i = 0; i < laneList.size(); i++)
		{
			if (laneList[i].Transition(percentage))
			{
				laneList.erase(laneList.begin() + i);
				i--;
			}
		}
		return true;
	}

	levelTime += deltaTime;

	CheckInput();
	player.Update(deltaTime);



	for (int i = 0; i < laneList.size(); i++)
	{
		if (laneList[i].CheckCollision(player))
		{
			player.Die();
			crashSound.play();
		}


		if (laneList[i].type == LaneType::RIVER)
		{
			if (player.grounded && !player.onLog)
			{
				for (int j = 0; j < laneList[i].logList.size(); j++)
				{
					if (player.hitbox.getGlobalBounds().intersects(laneList[i].logList[j].sprite.getGlobalBounds()))
					{
						player.onLog = true;
						player.currentLog = &laneList[i].logList[j];
					}
				}
			}

			if (player.grounded && !player.onLog)
			{
				for (int j = 0; j < laneList[i].lilypadList.size(); j++)
				{
					if (player.hitbox.getGlobalBounds().intersects(laneList[i].lilypadList[j].getGlobalBounds()))
					{
						if (laneList[i].lilypadComplete[j] == 0)
						{
							lilypadsCompleted += 1;
							float tempTime = levelTime / lilypadsCompleted;
							if (tempTime < 10)
								tempTime = 10;

							if (tempTime > 50)
								tempTime = 50;

							IncreaseScore(500 - (500 * (tempTime - 10) / 50.0f));
							laneList[i].lilypadList[j].setTexture(goldenLilypadTexture);
							laneList[i].lilypadComplete[j] = 1;
							int sum = 0;

							for (int k = 0; k < 3; k++)
							{
								sum += laneList[i].lilypadComplete[k];
							}
							if (sum == 3)
							{
								player.onLilypad = true;
								if (i != 0)
								{
									IncreaseScore(1000);
									lilypadSound.play();
									NextLevel();
								}
							}
							else if (i != 0)
							{
								lilypadSound.play();
								player.Respawn();
							}
							else
								player.onLilypad = true;
						}
						else
						{
							player.onLilypad = true;
						}

						

					}
				}
			}
			
			
			if (player.hitbox.getGlobalBounds().intersects(laneList[i].sprite.getGlobalBounds()) && player.grounded)
			{
				if (!player.onLog && !player.onLilypad)
				{
					player.Die();
					drownSound.play();
				}


			}
		}
	}


	if (player.onLog)
	{
		if (player.sprite.getPosition().x < 0 || player.sprite.getPosition().x > 640)
		{
			player.Die();
			drownSound.play();
		}
	}

	if (620 - rowSteppedOn * 40 > player.sprite.getPosition().y)
	{
		rowSteppedOn += 1;
		IncreaseScore(50);
	}

	timeText.setString(std::to_string((int)totalTime) + " +" + std::to_string((int)levelTime));


	if (player.health <= 0)
	{
		screen = Screen::END;
	}

	return true;
}

void Game::Draw(sf::RenderWindow& window)
{
	if (screen == Screen::MENU)
	{
		window.draw(menuBackground);
		window.draw(boxHeader);
		window.draw(endGuide);

		for (int i = 0; i < 5; i++)
		{
			window.draw(boxText[i]);
		}
	}
	else if (screen == Screen::GAME)
	{
		for (int i = 0; i < laneList.size(); i++)
		{
			laneList[i].Draw(window);
		}

		player.DrawSprite(window);

		window.draw(HUD);
		window.draw(timeSprite);
		window.draw(scoreSprite);
		window.draw(topSprite);
	
		window.draw(timeText);
		window.draw(scoreText);
		window.draw(topText);

		for (int i = 0; i < player.health; i++)
		{
			heartSprite.setPosition(640 - 30 * i - 36, 12);
			window.draw(heartSprite);
		}
	}
	else if (screen == Screen::END)
	{
		window.draw(endBackground);
		window.draw(yourScore);
		window.draw(endGuide);
		if (newHighscore)
		{
			for(int i = 0; i < 4; i++)
				window.draw(newNameText[i]);
			window.draw(selectorArrow);
		}
	}
	
}

void Game::CheckInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player.Jump(Direction::UP);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		player.Jump(Direction::LEFT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		player.Jump(Direction::DOWN);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		player.Jump(Direction::RIGHT);
	}
}

void Game::NextLevel()
{
	rowSteppedOn = 0;


	int offset = 560;
	int index;

	if (level == 0)
	{
		laneList.push_back(Lane(600, LaneType::GRASS, grassTexture));
		index = 0;
		offset = 40;
	}

	else
		index = rand() % (sizeof(maps) / (4 * 13));


	for (int i = 0; i < 13; i++)
	{
		if (maps[index][i] == 0)
		{
			laneList.push_back(Lane(600 - 40 * i - offset, LaneType::GRASS, grassTexture));
		}
		else if (maps[index][i] == 1)
		{
			laneList.push_back(Lane(600 - 40 * i - offset, LaneType::ROAD, roadTexture));
			laneList.back().SetupCarTexture(carTextureList);
		}
		else if (maps[index][i] == 2)
		{
			laneList.push_back(Lane(600 - 40 * i - offset, LaneType::RIVER, waterTexture));
			if (i < 12)
				laneList.back().SetupLogTexture(logTextureList);
			else
				laneList.back().SetupLilypad(lilypadTexture);
		}
	}

	totalTime += levelTime;
	levelTime = 0;

	timeText.setString(std::to_string(totalTime));

	level++;
	if (level == 1)
		return;

	transitionTimer = timeForTransition;
	lilypadsCompleted = 0;
}

void Game::IncreaseScore(int amount)
{
	score += amount;

	scoreText.setString(std::to_string(score));
}

void Game::SaveHighscore(std::string name, int score)
{
	std::ifstream input("../highscore.txt");

	bool added = false;
	int count = 0;
	std::string newtext = "";

	while (!input.eof())
	{
		std::string templine;
		std::getline(input, templine);
		
		int highscore = 0;

		if (templine != "")
		{
			std::string test = templine.substr(4, templine.length() - 4);
			highscore = std::stoi(test);
		}

		if (score > highscore && !added)
		{
			if (count != 0)
				newtext += "\n";

			newtext += name + " " + std::to_string(score);
			added = true;
			count++;
		}

		if (count < 5 && templine != "")
		{
			if (count != 0)
				newtext += "\n";
			newtext += templine;
			count++;
		}
	}

	if (!added && count < 5)
	{
		if (count != 0)
			newtext += "\n";
		newtext += name + " " + std::to_string(score);
	}


	std::ofstream output("../highscore.txt");
	output << newtext;
}

void Game::GetHighscore()
{
	std::ifstream input("../highscore.txt");
	int count = 0;
	while (!input.eof())
	{
		std::string temp;
		std::getline(input, temp);

		if (temp != "")
		{
			highscoreName[count] = temp.substr(0, 4);
			highscorePoints[count] = std::stoi(temp.substr(4, temp.length() - 4));
			count++;
		}


		if (count == 5)
			break;
	}

	for (int i = count; i < 5; i++)
	{
		highscoreName[i] = "- - -";
	}
}
