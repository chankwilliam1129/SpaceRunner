#pragma once
#include <DirectXMath.h>
#include "component.h"

class PositionVibrate :public Component
{
public:
	bool isTurnOn = false;
	Vector3 offset = {};

private:
	float time = 0;
	float timeCounter =0;
	float frequency = 1;

	Vector3 offsetMax = {};
	Vector3 offsetMin = {};

public:
	void Start();
	void Update();
	void ImGuiUpdate();

private:
	void VibrateEnd();

public:
	void SetVibrate(Vector3 offsetMax, Vector3 offsetMin, float time = 1.0f, float frequency = 0.2f);
};

class ScaleVibrate :public Component
{
public:
	bool isTurnOn = false;
	Vector3 offset = { 1,1,1 };

private:
	float time = 0;
	float timeCounter = 0;
	float frequency = 1;

	Vector3 offsetMax = {};
	Vector3 offsetMin = {};

public:
	void Start();
	void Update();
	void ImGuiUpdate();

private:
	void VibrateEnd();

public:
	void SetVibrate(Vector3 offsetMax, Vector3 offsetMin, float time = 1.0f, float frequency = 0.2f);
};