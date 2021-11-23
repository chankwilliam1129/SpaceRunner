#include "stage_block.h"
#include "player.h"
#include "glow.h"

void Block::Start()
{
	object->AddComponent<MeshRender>();
	object->GetComponent<MeshRender>()->vertexShader = ShaderManager::GetInstance()->vsData["Texture"];
	object->GetComponent<MeshRender>()->pixelShader = ShaderManager::GetInstance()->psData["Glow2"];
	object->GetComponent<MeshRender>()->texture->tData[1] = GlowManager::GetInstance()->upSampling;
	Setup();
	object->GetComponent<MeshRender>()->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	object->AddComponent<Glow>();
}

void Block::Update()
{
	if (IsSamePosition() && IsSameArea() && IsHit())
	{
		GetPlayer->nowHit = object->parent->GetComponent<Stage>()->type;
		object->GetComponent<MeshRender>()->SetMaterialColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		object->GetComponent<MeshRender>()->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

bool Block::IsSamePosition()
{
	float worldPosition = object->GetComponent<Transform>()->GetWorldPosition().z;
	return (worldPosition < size&& worldPosition>-size);
}

bool Block::IsSameArea()
{
	int StateArea = object->parent->GetComponent<Stage>()->area;
	return (GetPlayer->nowStageArea == StateArea);
}

void Block01::Setup()
{
	object->GetComponent<MeshRender>()->mesh = new Mesh(MeshManager::Data["Block01"]);
}

bool Block01::IsHit()
{
	switch (GetPlayer->nowState)
	{
	case Player::Run: 
	case Player::SlidingStart:
	case Player::Sliding: return true;
	case Player::JumpShort:
	case Player::JumpMedium:
	case Player::JumpLarge:
	case Player::Dash:
	default: return false;
	}
}

void Block02::Setup()
{
	object->GetComponent<MeshRender>()->mesh = new Mesh(MeshManager::Data["Block02"]);
}

bool Block02::IsHit()
{
	switch (GetPlayer->nowState)
	{
	case Player::Run: return true;
	case Player::SlidingStart:
	case Player::Sliding: return false;
	case Player::JumpShort:
	case Player::JumpMedium: return true;
	case Player::JumpLarge: return false;
	case Player::Dash:return true;
	default: return false;
	}
}

void Block03::Setup()
{
	object->GetComponent<MeshRender>()->mesh = new Mesh(MeshManager::Data["Block03"]);
	size = 340.0f;
}

bool Block03::IsHit()
{
	switch (GetPlayer->nowState)
	{
	case Player::Run:
	case Player::SlidingStart:
	case Player::Sliding:
	case Player::JumpShort:
	case Player::JumpMedium:
	case Player::JumpLarge:
	case Player::Dash:
	default: return true;
	}
}

void Block04::Setup()
{
	object->GetComponent<MeshRender>()->mesh = new Mesh(MeshManager::Data["Block04"]);
}

bool Block04::IsHit()
{
	switch (GetPlayer->nowState)
	{
	case Player::Run:
	case Player::SlidingStart:
	case Player::Sliding: return true;
	case Player::JumpShort:
	case Player::JumpMedium:
	case Player::JumpLarge:
	case Player::Dash:
	default: return false;
	}
}

void Block05::Setup()
{
	object->GetComponent<MeshRender>()->mesh = new Mesh(MeshManager::Data["Block05"]);
}

bool Block05::IsHit()
{
	switch (GetPlayer->nowState)
	{
	case Player::Run:
	case Player::SlidingStart:
	case Player::Sliding: return true;
	case Player::JumpShort:
	case Player::JumpMedium:
	case Player::JumpLarge:
	case Player::Dash:
	default: return false;
	}
}