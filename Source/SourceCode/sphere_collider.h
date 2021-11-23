#pragma once
#include "component.h"
#include "collider.h"

class SphereCollider :public Collider
{
public:
	Vector3 center = {};
	float   radius = .0f;

public:

};