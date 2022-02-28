#pragma once
#include "GameObject.h"

class Car : public GameObject
{
public:
	Car();
	Car(sf::Texture& texture, float x, float y, float velocity);
	~Car();
	void Update(float deltaTime);
private:

};