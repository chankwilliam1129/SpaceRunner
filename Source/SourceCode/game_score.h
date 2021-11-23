#pragma once
#include <DirectXMath.h>
#include "game_object.h"

class ScoreNumber : public Component
{
public:
	int number = 0;
public:
	void Start();
	void Update();
};

class GameScore :public Component
{
public:
	void Start();
	void Update();
};
