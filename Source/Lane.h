#pragma once
#include "SFML/Graphics.hpp"
#include "Car.h"
#include "Log.h"
#include "Player.h"

enum class LaneType
{
	GRASS,
	ROAD,
	RIVER
};

class Lane
{
public:
	LaneType type = LaneType::GRASS;
	sf::Sprite sprite;

	Lane();
	Lane(float y, LaneType type, sf::Texture& texture);
	~Lane();
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
	void SetupCarTexture(std::vector<sf::Texture>& textureList);
	void SetupLogTexture(std::vector<sf::Texture>& textureList);
	void SetupLilypad(sf::Texture& texture);
	bool CheckCollision(Player& player);
	bool Transition(float percentage);

	std::vector<Log> logList;
	std::vector<sf::Sprite> lilypadList;
	int lilypadComplete[3] = { 0, 0, 0 };

private:
	std::vector<Car> carList;

	int carDirection = 0;

	sf::Vector2f prevPosition;
};