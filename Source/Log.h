#pragma once
#include "GameObject.h"

class Log : public GameObject
{
public:
	Log();
	Log(sf::Texture& texture, float x, float y, float velocity);
	~Log();
	void Update(float deltaTime);
private:

};