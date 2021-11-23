#pragma once
#include "component.h"
#include "../GameLib/mesh.h"
#include "../GameLib/model_resource.h"
#include "../GameLib/shader.h"
#include "../GameLib/texture.h"
#include "../GameLib/render_texture.h"
#include "game_object.h"

class Particle
{
public:
	Vector3 position = {};
	Vector3 rotation = {};
	Vector3 scale = { 1.0f,1.0f ,1.0f };

	DirectX::XMMATRIX matrix = {};
	DirectX::XMFLOAT4X4 orientation = {};

	float life = 0;


	DirectX::XMMATRIX GetMatrix();
	Vector3 Forward();
};


class ParticleSystem :public Component
{
public:
	std::list<Particle*> particleList;

	float TimeCounter = 0.0f;
	float Frequency = 0.02f;

	float startLifeTime = 1.0f;
	float startSpeed = -3.8f;
	float startSize = 2;
	Vector4 color = { 0.0f,0.2f,0.9f,1.0f };

public:
	Shader* pixelShader = nullptr;
	Shader* vertexShader = nullptr;
	Mesh* mesh = nullptr;
	Texture* texture = nullptr;

public:
	void Start();
	void Update();
	void Draw();
	void TextureDraw();
	void ImGuiUpdate();
public:
	Particle* Spawn();
};