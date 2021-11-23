#pragma once
#include <map>
#include <string>
#include "../GameLib/system.h"
#include "../GameLib/input.h"
#include"../GameLib/singleton.h"
#include "../GameLib/constant_buffer.h"
#include "component.h"

class Camera :public Component
{
public:
	float fieldOfView;
	float aspect;
	float farClipPlane;
	float nearClipPlane;

	DirectX::XMMATRIX projection;

public:
	void Start();
	void Update();
};

class CameraManager : public Singleton<CameraManager>
{
private:
	struct  CbScene
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};

public:
	std::string name;
	std::shared_ptr<ConstantBuffer<CbScene>> sceneConstantBuffer;
	std::list<Object*> ObjectList;

public:
	void Initialize();
	void Update();
	void Release();
	Object* Instance(std::string name, Vector3 pos = Vector3(0, 0, 0), Vector3 rotation = Vector3(0, 0, 0));
};