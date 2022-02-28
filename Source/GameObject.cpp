#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetPosition(float x, float y)
{
	sprite.setPosition(x, y);
}

void GameObject::SetTexture(sf::Texture& texture)
{
	sprite.setTexture(texture);
}

void GameObject::DrawSprite(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void GameObject::Move(float deltaTime)
{
	sprite.move(velocity * deltaTime);
	if(doWorldWrap)
		WorldWrap();
}

void GameObject::VerticalTransition(float percentage)
{
	sprite.setPosition(sprite.getPosition().x, spawnY + 520 * percentage);

	if (percentage == 1)
	{
		spawnY = sprite.getPosition().y;
	}
}

void GameObject::WorldWrap()
{
	if (sprite.getPosition().x > 700)
		sprite.setPosition(-60, sprite.getPosition().y);

	if (sprite.getPosition().x < -60)
		sprite.setPosition(700, sprite.getPosition().y);
}
