#pragma once
#include "component.h"

class Collider :public Component
{
public:

	static std::list<Collider*> ColliderList;
	
	std::map<Collider*, int>  ColliderMap;

public:
	Collider();
	void Start();
	void Update();

public:
	//virtual bool ColliderCheck(Collider* col) {};
};
