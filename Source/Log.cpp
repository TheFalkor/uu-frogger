#include "Log.h"

Log::Log()
{
}

Log::Log(sf::Texture& texture, float x, float y, float velocity)
{
	SetTexture(texture);
	sprite.setPosition(x, y);
	sprite.setOrigin(texture.getSize().x / 2, 13);

	this->doWorldWrap = true;
	this->velocity = sf::Vector2f(velocity, 0);

	spawnY = y;
}

Log::~Log()
{
}

void Log::Update(float deltaTime)
{
	Move(deltaTime);
}
