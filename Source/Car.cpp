#include "Car.h"

Car::Car()
{
}

Car::Car(sf::Texture& texture, float x, float y, float velocity)
{
	SetTexture(texture);
	sprite.setPosition(x, y);
	sprite.setOrigin(texture.getSize().x / 2, 13);

	if (velocity < 0)
		sprite.setRotation(180);

	this->doWorldWrap = true;
	this->velocity = sf::Vector2f(velocity, 0);

	spawnY = y;
}

Car::~Car()
{
}

void Car::Update(float deltaTime)
{
	Move(deltaTime);
}
