#pragma once
#include "scene_title.h"
#include "game_object.h"

class ExplanatoryText;
class ExplanatoryText2;
class TitleManager :public GameObjectManager, public Singleton<TitleManager>
{
public:
	ExplanatoryText* explanatoryText;
	ExplanatoryText2* explanatoryText2;

public:
	void Initialize();
	void Update();
};

class TitlePlayer :public Component
{
public:
	void Start();
	void Update();
};

class TitleName :public Component
{
public:
	void Start();
	void Update();
};

class TitleText :public Component
{
public:
	void Start();
};

class ExplanatoryText :public Component
{
public:
	TextRender* text;

public:
	void Start();
};

class ExplanatoryText2 :public Component
{
public:
	TextRender* text;

public:
	void Start();
};