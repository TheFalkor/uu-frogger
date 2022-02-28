#pragma once

#include "SFML/Graphics.hpp"

class GameObject
{
public:
	GameObject();
	~GameObject();
	void SetPosition(float x, float y);
	void SetTexture(sf::Texture& texture);
	void DrawSprite(sf::RenderWindow& window);

	void Move(float deltaTime);
	void VerticalTransition(float percentage);

	float spawnY = 0;

	sf::Sprite sprite;
	sf::Vector2f velocity;

	bool doWorldWrap = false;
private:
	void WorldWrap();
};