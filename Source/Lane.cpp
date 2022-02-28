#include "Lane.h"
#include <random>
#include <time.h>

Lane::Lane()
{
}

Lane::Lane(float y, LaneType type, sf::Texture& texture)
{
	this->type = type;
	sprite.setTexture(texture);

	sprite.setPosition(0, y);

	prevPosition = sprite.getPosition();
}

Lane::~Lane()
{
}

void Lane::Update(float deltaTime)
{

	if (type == LaneType::ROAD)
	{
		for (int i = 0; i < carList.size(); i++)
		{
			carList[i].Update(deltaTime);
		}
	}
	if (type == LaneType::RIVER)
	{

		for (int i = 0; i < logList.size(); i++)
		{
			logList[i].Update(deltaTime);
		}

		for (int i = 0; i < lilypadList.size(); i++)
		{
			if (lilypadComplete[i] == 1)
			{
				lilypadList[i].rotate(60 * deltaTime);
			}
		}
	}
}

void Lane::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	if (type == LaneType::ROAD)
	{
		for (int i = 0; i < carList.size(); i++)
		{
			carList[i].DrawSprite(window);
		}
	}
	if (type == LaneType::RIVER)
	{
		for (int i = 0; i < logList.size(); i++)
		{
			logList[i].DrawSprite(window);
		}

		for (int i = 0; i < lilypadList.size(); i++)
		{
			window.draw(lilypadList[i]);
		}
	}
	
}

void Lane::SetupCarTexture(std::vector<sf::Texture>& textureList)
{
	int carCount = 5 + rand() % 3;

	float velocity = 50 + rand() % 75;
	int y = sprite.getPosition().y;

	if (y % 80 == 0)
		velocity *= -1;

	int offset = 0;
	for (int i = 0; i < carCount; i++)
	{
		int randomTexture = rand() % textureList.size();

		offset += 50 + rand() % 150;
		if (offset + textureList[randomTexture].getSize().x > 700)
			break;

		carList.push_back(Car(textureList[randomTexture], offset, y + 20, velocity));
		offset += textureList[randomTexture].getSize().x;
	}
}

void Lane::SetupLogTexture(std::vector<sf::Texture>& textureList)
{
	int logCount = 4 + rand() % 3;

	float velocity = 50 + rand() % 75;
	int y = sprite.getPosition().y;

	if (y % 80 == 0)
		velocity *= -1;

	int offset = -60;
	for (int i = 0; i < logCount; i++)
	{
		int randomTexture = rand() % textureList.size();

		offset += 50 + rand() % 100;
		if (offset + textureList[randomTexture].getSize().x > 700)
			break;

		logList.push_back(Log(textureList[randomTexture], offset, y + 20, velocity));
		offset += textureList[randomTexture].getSize().x;
	}
}

void Lane::SetupLilypad(sf::Texture& texture)
{
	for (int i = 0; i < 3; i++)
	{
		lilypadList.push_back(sf::Sprite());
		lilypadList.back().setTexture(texture);
		lilypadList.back().setPosition(140 + i * 200, sprite.getPosition().y + 20);
		lilypadList.back().setOrigin(20, 20);

		float rotation = rand() % 360;
		lilypadList.back().setRotation(rotation);

	}
}

bool Lane::CheckCollision(Player& player)
{
	for (int i = 0; i < carList.size(); i++)
	{
		if (carList[i].sprite.getGlobalBounds().intersects(player.hitbox.getGlobalBounds()))
		{
			carList.erase(carList.begin() + i);
			return true;
		}

	}

	return false;
}

bool Lane::Transition(float percentage)
{
	sprite.setPosition(prevPosition.x, prevPosition.y + 520 * percentage);

	if (type == LaneType::RIVER)
	{
		for (int i = 0; i < logList.size(); i++)
			logList[i].VerticalTransition(percentage);

		for (int i = 0; i < lilypadList.size(); i++)
			lilypadList[i].setPosition(lilypadList[i].getPosition().x, sprite.getPosition().y + 20);
	}
	if (type == LaneType::ROAD)
	{
		for (int i = 0; i < carList.size(); i++)
			carList[i].VerticalTransition(percentage);
	}


	if (sprite.getPosition().y >= 640)
		return true;

	if (percentage == 1)
		prevPosition = sprite.getPosition();

	return false;
}
