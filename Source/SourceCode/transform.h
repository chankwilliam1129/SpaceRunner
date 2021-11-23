#pragma once
#include <DirectXMath.h>
#include "component.h"

static float(* const ToRadian)(float) = DirectX::XMConvertToRadians;
static float(* const ToDegree)(float) = DirectX::XMConvertToDegrees;

class Transform :public Component
{
public:
	Vector3 position = {};
	Vector3 rotation = {};
	Vector3 scale = { 1.0f,1.0f ,1.0f };

public:
	Transform() {};

public:
	DirectX::XMMATRIX GetWorldMatrix();

	Vector3 Right();
	Vector3 Up();
	Vector3 Forward();

	Vector3 GetWorldPosition();
	Vector2 GetWorldPosition2D();
	Vector3 GetWorldRotation();
	Vector3 GetWorldScale();

public:
	void Update();
	void ImGuiUpdate();
};

void NormalizeRotation(float& rotation);
