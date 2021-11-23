#include "player.h"
#include "stage.h"
#include "load_mesh.h"
#include "component_list.h"
#include "camera.h"
#include "time.h"
#include "game_score.h"
#include "glow.h"
#include "scene_title.h"

float ParabolicCurve(float time, float maxTime,float maxValue)
{
	float result = ((maxTime / 2.0f) - abs(maxTime / 2.0f - time)) / (maxTime / 2.0f);
	return result * result * maxValue;
}

void PlayerManager::Initialize()
{
	score = 0;
	name = "PlayerManager";
	Instance("Player",Vector3(0.0f,-9.5f,0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.1f, 0.1f, 0.1f))->AddComponent<Player>();
	Instance("GameScore", Vector3(1230.0f, 40.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.4f, 0.4f, 1.0f))->AddComponent<GameScore>();
}

void Player::Start()
{
	object->AddComponent<SkinMeshRender>();
	object->AddComponent<Glow>();

	object->GetComponent<SkinMeshRender>()->skinMesh     = new SkinMesh(MeshManager::Data["Robot"]);
	object->GetComponent<SkinMeshRender>()->vertexShader = ShaderManager::GetInstance()->vsData["SkinMesh"];
	object->GetComponent<Glow>()->vertexShader = ShaderManager::GetInstance()->vsData["SkinMesh"];
	object->GetComponent<SkinMeshRender>()->pixelShader  = ShaderManager::GetInstance()->psData["Glow2"];

	SetState(State::Run);
	nowStageArea = 0;
	object->AddComponent<ParticleSystem>();
}

void Player::Update()
{
	object->GetComponent<SkinMeshRender>()->skinMesh->UpdateAnimation(GetTimeManager->ScaleDeltaTime() * StageManager::GetInstance()->moveSpeed);
	StateUpdate();
	HitCheck();
	PositionUpdate();
	CameraUpdate();
	TransformUpdate();
	debug::SetString("PlayerAngle: %02.1f, Area:%2d", angle, nowStageArea);
}

void Player::StateUpdate()
{
	stateTimeCounter += GetTimeManager->ScaleDeltaTime() * StageManager::GetInstance()->moveSpeed;

	switch (nowState)
	{
	case State::Run:
		height = 0.0f;
		if (StageManager::GetInstance()->moveSpeed != 1.0f)
		{
			StageManager::GetInstance()->moveSpeed -= (StageManager::GetInstance()->moveSpeed - 1.0f) * 0.1f;
		}
		if (kb.PushTrigger(VK_SPACE)|| kb.Push(VK_SPACE)) { SetState(State::SlidingStart); }
		break;

	case State::SlidingStart:
		height = 0.0f;
		if (stateTimeCounter > 0.166f)
		{
			SetState(State::Sliding);
		}
		if (kb.ReleaseTrigger(VK_SPACE))
		{
			SoundManager::GetInstance()->Data["Jump"]->Play(false);
			SetState(State::JumpShort);
		}
		break;

	case State::Sliding:
		height = 0.0f;
		if (StageManager::GetInstance()->moveSpeed < 2.0f)
		{
			StageManager::GetInstance()->moveSpeed += 0.1f * GetTimeManager->ScaleDeltaTime();
		}
		if (kb.ReleaseTrigger(VK_SPACE))
		{
			SoundManager::GetInstance()->Data["Jump"]->Play(false);
			if (stateTimeCounter > 0.3f) { SetState(State::JumpMedium); }
			else { SetState(State::JumpShort); }
		}
		break;
	case State::JumpShort:
		height = ParabolicCurve(stateTimeCounter, JumpTimeShort, 3.0f);
		if (stateTimeCounter > JumpTimeShort) { SetState(State::Run); }
		if (kb.PushTrigger(VK_SPACE))
		{
			SoundManager::GetInstance()->Data["SpeedUp"]->Play(false);
			SetState(State::Dash);
			height += 2.0f;
		}
		break;

	case State::JumpMedium:
		height = ParabolicCurve(stateTimeCounter, JumpTimeMideum, 3.0f);
		if (stateTimeCounter > JumpTimeMideum) { SetState(State::Run); }
		if (kb.PushTrigger(VK_SPACE)) 
		{ 
			SoundManager::GetInstance()->Data["SpeedUp"]->Play(false);
			SetState(State::Dash); 
			height += 2.0f;
		}
		break;

	case State::JumpLarge:
		height = ParabolicCurve(stateTimeCounter, JumpTimeLarge, 9.0f);
		if (!isturn && stateTimeCounter >= JumpTimeLarge * 0.33f)
		{
			angle += 180.0f;
			cameraTargetAngle += 180.0f;
			nowStageArea += StageManager::StageSize *0.5f;
			isturn = true;
		}
		if (stateTimeCounter > JumpTimeLarge) 
		{ 
			SetState(State::Run); 
			isturn = false;
		}
		break;

	case State::Dash:
		StageManager::GetInstance()->moveSpeedScale =1.5f;
		height -= 3.0f * GetTimeManager->ScaleDeltaTime() / dashTimeLarge;
		StageManager::GetInstance()->moveSpeed = 2.0f;
		if (stateTimeCounter > dashTimeLarge)
		{
			SetState(State::Run);
		}
		break;
	}
}

void Player::PositionUpdate()
{
	if (kb.PushTrigger(VK_RIGHT)) 
	{
		SoundManager::GetInstance()->Data["Move"]->Play(false);
		targetAngle -= 22.5f;
		cameraTargetAngle -= 22.5f;
		nowStageArea -= 1;
	}
	if (kb.PushTrigger(VK_LEFT)) 
	{
		SoundManager::GetInstance()->Data["Move"]->Play(false);
		targetAngle += 22.5f;
		cameraTargetAngle += 22.5f; 
		nowStageArea += 1;
	}

	if (targetAngle != 0.0f)
	{
		angle += targetAngle < 1.0f && targetAngle > -1.0f ? targetAngle : targetAngle * 0.2f;
		targetAngle -= targetAngle < 1.0f && targetAngle > -1.0f ? targetAngle : targetAngle * 0.2f;
	}

	if (nowStageArea >= StageManager::StageSize)nowStageArea -= StageManager::StageSize;
	if (nowStageArea < 0)nowStageArea += StageManager::StageSize;
}

void Player::CameraUpdate()
{
	if (targetAngle == 0.0f && cameraTargetAngle != 0.0f)
	{
		cameraAngle += cameraTargetAngle < 1.0f && cameraTargetAngle > -1.0f ? cameraTargetAngle : cameraTargetAngle * 0.32f;
		cameraTargetAngle -= cameraTargetAngle < 1.0f && cameraTargetAngle > -1.0f ? cameraTargetAngle : cameraTargetAngle * 0.32f;
	}

	CameraManager::GetInstance()->ObjectList.begin()._Ptr->_Myval->GetComponent<Transform>()->rotation.z = cameraAngle;
	Vector3 cameraPos = Vector3(sinf(ToRadian(cameraAngle)) * -3.5f, cosf(ToRadian(cameraAngle)) * -3.5f, -50.0f);
	CameraManager::GetInstance()->ObjectList.begin()._Ptr->_Myval->GetComponent<Transform>()->position = cameraPos;
}

void Player::TransformUpdate()
{
	object->GetComponent<Transform>()->position.x = sinf(ToRadian(angle)) * (9.5f - height);
	object->GetComponent<Transform>()->position.y = cosf(ToRadian(angle)) * (9.5f - height);
	object->GetComponent<Transform>()->rotation.z = angle + 180.0f;
}

void Player::SetState(State state)
{
	nowState = state;
	stateTimeCounter = 0.0f;
	switch (state)
	{
	case SlidingStart:
		object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(1, true);
		break;
	case Sliding:
		object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(2, true);
		break;
	case JumpShort:
		object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(3, true);
		break;
	case JumpMedium:
		object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(4, true);
		break;
	case JumpLarge:
		object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(5, true);
		isturn = false;
		break;
	case Dash:
		object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(6, true);
		break;
	default:
		object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(0, true);
	}
}

void Player::HitCheck()
{
	debug::SetString("nowhit %d", nowHit);
	switch (nowHit)
	{
	case 0:
		break;
	case 4:
		SetState(State::JumpLarge);
		PlayerManager::GetInstance()->score += 100;
		SoundManager::GetInstance()->Data["Stage"]->Play(false);
		break;
	case 5:
		PlayerManager::GetInstance()->score += 100;
		StageManager::GetInstance()->moveSpeed = 3.0f;
		StageManager::GetInstance()->moveSpeedScale = 1.5f;
		SoundManager::GetInstance()->Data["Stage"]->Play(false);
		break;
	default:
		SceneManager::GetInstance()->scene->SetScene(std::make_shared<SceneTitle>());
		break;
	}
	nowHit = 0;
}