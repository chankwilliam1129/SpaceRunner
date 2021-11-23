#pragma once
#include "stage.h"

class Block :public Component
{
public:
	float size = 10.0f;

public:
	void Start();
	void Update();

	bool IsSamePosition();
	bool IsSameArea();

	virtual void Setup() = 0;
	virtual bool IsHit() = 0;
};

class Block01 :public Block
{
public:
	void Setup();
	bool IsHit();
};

class Block02 :public Block
{
public:
	void Setup();
	bool IsHit();
};

class Block03 :public Block
{
public:
	void Setup();
	bool IsHit();
};

class Block04 :public Block
{
public:
	void Setup();
	bool IsHit();
};

class Block05 :public Block
{
public:
	void Setup();
	bool IsHit();
};