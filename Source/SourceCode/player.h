#pragma once
#include <DirectXMath.h>
#include "game_object.h"

class PlayerManager :public GameObjectManager ,public Singleton<PlayerManager>
{
public:
	int score = 0;
public:
	void Initialize();
};

class Player :public Component
{
public:
	enum State
	{
		Run,
		SlidingStart,
		Sliding,
		JumpShort,
		JumpMedium,
		JumpLarge,
		Dash,
	};

public:
	const float JumpTimeShort = 0.5f;
	const float JumpTimeMideum = 0.9f;
	const float JumpTimeLarge = 1.16;
	const float dashTimeLarge = 0.5f;
	const float MoveTime = 0.7f;

public:
	State nowState = Run;
	int nowHit;
	float stateTimeCounter = 0.0f;

	float height = 0.0f;
	int nowStageArea = 0;

	float angle = 0.0f;
	float targetAngle =0.0f;

	float cameraAngle =180.0f;
	float cameraTargetAngle=0.0f;

	bool isturn;
public:
	void Start();
	void Update();

public:
	void StateUpdate();
	void PositionUpdate();
	void CameraUpdate();
	void TransformUpdate();
	void SetState(State state);
	void HitCheck();
};

#define GetPlayer (PlayerManager::GetInstance()->ObjectList.begin()._Ptr->_Myval->GetComponent<Player>())