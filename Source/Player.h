#pragma once

#include "GameObject.h"
#include "Log.h"
#include "SFML/Audio.hpp"

enum class Direction {
	UP,
	LEFT,
	DOWN,
	RIGHT
};

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Update(float deltaTime);
	void Jump(Direction dir);
	void Respawn();
	void Die();
	void Transition(float percentage);

	sf::CircleShape hitbox;
	int health = 3;
	bool grounded = true;
	bool onLog = false;
	bool onLilypad = false;
	Log* currentLog = nullptr;

private:
	sf::Vector2f destination;
	sf::Vector2f prevPosition;
	float currentJumpTimer = 0;
	float jumpTimer = 0.4f;
	float cooldown = 0;

	sf::Texture texture;
	sf::SoundBuffer jumpBuffer;
	sf::Sound jumpSound;
};