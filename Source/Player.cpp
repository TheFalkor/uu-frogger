#include "Player.h"
#include <math.h>

Player::Player()
{
	if (!texture.loadFromFile("../Assets/Sprites/Frog.png"))
		system("pause");

	if (!jumpBuffer.loadFromFile("../Assets/Audio/Jump.wav"))
		system("pause");
	jumpSound.setBuffer(jumpBuffer);

	SetTexture(texture);
	SetPosition(340, 620);
	sprite.setOrigin(20, 20);

	destination = sprite.getPosition();
	prevPosition = sprite.getPosition();

	hitbox = sf::CircleShape(10);
	hitbox.setOrigin(10, 10);
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	if (cooldown > 0)
	{
		cooldown -= deltaTime;
		return;
	}

	if (onLog)
	{
		sprite.move(currentLog->velocity * deltaTime);
		destination += currentLog->velocity * deltaTime;
		prevPosition += currentLog->velocity * deltaTime;
	}

	if (currentJumpTimer > 0)
	{
		currentJumpTimer -= deltaTime;
		float deltaX = destination.x - prevPosition.x;
		float deltaY = destination.y - prevPosition.y;

		float a = 1 - currentJumpTimer / jumpTimer;
		float test = sin(3.14f * a) * 0.5f;
		sprite.setScale(1 + test, 1 + test);
		sprite.setPosition(destination.x - deltaX * currentJumpTimer/jumpTimer, destination.y - deltaY * currentJumpTimer/jumpTimer);

		if (currentJumpTimer <= 0)
		{
			sprite.setPosition(destination);
			sprite.setScale(1, 1);
			prevPosition = sprite.getPosition();
			grounded = true;
			onLog = false;
			onLilypad = false;
		}
	}

	hitbox.setPosition(sprite.getPosition());
}

void Player::Jump(Direction dir)
{
	if (currentJumpTimer > 0 || cooldown > 0)
		return;

	destination = prevPosition;
	switch (dir)
	{
	case Direction::UP:
		if (destination.y - 40 >= 80)
		{
			sprite.setRotation(0);
			destination.y -= 40;
		}
		break;
	case Direction::LEFT:		
		if (destination.x - 40 >= 0)
		{
			sprite.setRotation(270);
			destination.x -= 40;
		}
		break;
	case Direction::DOWN:
		if (destination.y + 40 <= 620)
		{
			sprite.setRotation(180);
			destination.y += 40;
		}
			
		break;
	case Direction::RIGHT:		
		if (destination.x + 40 <= 620)
		{
			sprite.setRotation(90);
			destination.x += 40;
		}
		break;
	}
	if (destination != prevPosition)
	{
		jumpSound.play();
		currentJumpTimer = jumpTimer;
		grounded = false;
	}

}

void Player::Respawn()
{
	sprite.setPosition(340, 620);
	hitbox.setPosition(sprite.getPosition());
	prevPosition = sprite.getPosition();
	destination = sprite.getPosition();

	sprite.setScale(1, 1);
	currentJumpTimer = 0;

	cooldown = 0.5f;
}

void Player::Die()
{
	onLog = false;
	Respawn();
	health--;
}

void Player::Transition(float percentage)
{
	sprite.setPosition(prevPosition.x, prevPosition.y + 520 * percentage);

	if (percentage == 1)
		prevPosition = sprite.getPosition();
}
