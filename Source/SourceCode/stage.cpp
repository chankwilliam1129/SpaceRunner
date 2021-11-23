#include "stage.h"
#include "stage_block.h"
#include "load_mesh.h"
#include "player.h"
#include "time.h"
#include "glow.h"
#include "scene_game.h"

void StageManager::Initialize()
{
	name = "StageManager";

	moveSpeed = 1.0f;
	moveSpeedScale = 1.0f;
	nowStageTimer = .0f;
	nowStage = 0;
	index = 0;
	randNum = 3;
	adjustNum = 0;

	SetStageData();

	for (int j = 0; j < StageListSize; j++)
	{
		StageGroup* group = Instance("StageGroup")->AddComponent<StageGroup>();
		group->position = j;
		group->index = GetIndex();
		nowStage++;
	}
}

void StageManager::Update()
{
	GameObjectManager::Update();
	if (moveSpeedScale > 1.0f) { moveSpeedScale *= 0.9f; }
	else { moveSpeedScale = 1.0f; }

	if (moveSpeed > 2.0f) { moveSpeed *= 0.95f; }
	nowStageTimer += GetTimeManager->ScaleDeltaTime() * moveSpeed * moveSpeedScale;
	if (nowStageTimer >= 1.0f)
	{
		nowStageTimer -= 1.0f;
		PlayerManager::GetInstance()->score += 10 * StageManager::GetInstance()->moveSpeed * moveSpeedScale;
	}

	debug::SetString("Stage: %02d , Timer: %02f", nowStage, nowStageTimer);
}

void StageManager::Render() 
{
	GlowManager::GetInstance()->cbGlow->data.glowStrength = StageGlowStrength;
	GlowManager::GetInstance()->cbGlow->UpdateSubresource(GetSystemManager->context.Get());
	GameObjectManager::Render();
}

void StageManager::SetStageData()
{
	StageSet.clear();

	//EASY
	StageSet[0] = new StageData(1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1);
	StageSet[1] = new StageData(1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0);
	StageSet[2] = new StageData(1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0);
	StageSet[3] = new StageData(0, 0, 2, 0, 0, 1, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0);
	StageSet[4] = new StageData(0, 1, 1, 1, 0, 2, 2, 2, 0, 1, 1, 1, 0, 2, 2, 2);
	StageSet[5] = new StageData(1, 1, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0);
	StageSet[6] = new StageData(1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0);
	StageSet[7] = new StageData(2, 2, 2, 1, 1, 0, 0, 2, 2, 2, 1, 1, 0, 0, 1, 2);
	StageSet[8] = new StageData(2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0);
	StageSet[9] = new StageData(1, 2, 1, 2, 0, 0, 1, 0, 2, 1, 2, 1, 0, 0, 1, 0);

	//NORMAL
	StageSet[10] = new StageData(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5);
	StageSet[11] = new StageData(5, 5, 0, 0, 1, 1, 0, 0, 5, 5, 0, 0, 1, 1, 0, 0);
	StageSet[12] = new StageData(5, 2, 5, 2, 1, 5, 1, 5, 5, 2, 5, 2, 1, 5, 1, 5);
	StageSet[13] = new StageData(1, 2, 5, 5, 2, 1, 1, 2, 5, 0, 0, 0, 0, 5, 2, 1);
	StageSet[14] = new StageData(0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1);
	StageSet[15] = new StageData(1, 1, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0);
	StageSet[16] = new StageData(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0);
	StageSet[17] = new StageData(2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 5, 5, 5, 5);
	StageSet[18] = new StageData(2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0);
	StageSet[19] = new StageData(5, 5, 1, 2, 5, 5, 2, 1, 2, 1, 5, 5, 1, 2, 1, 2);

	//HARD
	StageSet[20] = new StageData(5, 5, 4, 4, 5, 5, 4, 4, 5, 5, 4, 4, 5, 5, 4, 4);
	StageSet[21] = new StageData(5, 5, 4, 4, 1, 1, 4, 4, 5, 5, 4, 4, 1, 1, 4, 4);
	StageSet[22] = new StageData(5, 2, 5, 2, 1, 5, 1, 5, 5, 2, 5, 2, 1, 5, 1, 5);
	StageSet[23] = new StageData(1, 2, 5, 5, 2, 1, 1, 2, 5, 4, 4, 4, 4, 5, 2, 1);
	StageSet[24] = new StageData(0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 1);
	StageSet[25] = new StageData(1, 1, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0);
	StageSet[26] = new StageData(1, 1, 1, 4, 1, 1, 1, 5, 1, 1, 1, 4, 0, 0, 0, 5);
	StageSet[27] = new StageData(2, 5, 2, 2, 1, 5, 1, 1, 0, 5, 0, 0, 4, 4, 4, 4);
	StageSet[28] = new StageData(2, 2, 2, 2, 4, 4, 4, 4, 2, 2, 2, 2, 4, 4, 4, 4);
	StageSet[29] = new StageData(1, 1, 1, 1, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1);

	//SUBARASHI
	StageSet[30] = new StageData(3, 0, 0, 0, 3, 3, 0, 0, 0, 3, 3, 0, 0, 0, 0, 3);
	StageSet[31] = new StageData(3, 1, 1, 1, 3, 3, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3);
	StageSet[32] = new StageData(3, 2, 2, 2, 3, 3, 2, 2, 2, 3, 3, 2, 2, 2, 2, 3);
	StageSet[33] = new StageData(3, 4, 4, 4, 3, 3, 4, 4, 4, 3, 3, 4, 4, 4, 4, 3);
	StageSet[34] = new StageData(3, 5, 5, 5, 3, 3, 5, 5, 5, 3, 3, 5, 5, 5, 5, 3);
	StageSet[35] = new StageData(3, 1, 1, 1, 1, 1, 1, 3, 3, 4, 4, 4, 4, 4, 4, 3);
	StageSet[36] = new StageData(3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 5, 5, 5, 5, 3);
	StageSet[37] = new StageData(3, 1, 1, 1, 4, 4, 1, 1, 1, 3, 3, 5, 5, 5, 5, 3);
	StageSet[38] = new StageData(3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 5, 5, 5, 5, 3);
	StageSet[39] = new StageData(3, 1, 4, 1, 4, 1, 4, 1, 4, 3, 3, 5, 5, 5, 5, 3);


	//OTHER
	StageSet[50] = new StageData(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	StageSet[51] = new StageData(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	StageSet[52] = new StageData(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
	StageSet[53] = new StageData(3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0);
	StageSet[54] = new StageData(4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4);
	StageSet[55] = new StageData(5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);
}				   

int StageManager::GetIndex()
{
	if (nowStage > 200) return rand() % 32 + 10;
	else if (nowStage > 150) return rand() % 40;
	else if (nowStage > 120) return rand() % 30;
	else if (nowStage > 100) return rand() % 20;
	else if (nowStage > 80)  return rand() % 14;
	else if (nowStage > 60)  return rand() % 10;

	else if (nowStage < 4) return 50;
	else if (nowStage == 4) return 0;
	else if (nowStage == 5) return 1;
	else if (nowStage == 6) return 2;
	else if (nowStage == 7) return 51;
	else if (nowStage == 8) return 50;
	else if (nowStage == 9) return 51;

	else if (nowStage == 10) return 18;
	else if (nowStage == 11) return 50;
	else if (nowStage == 12) return 52;

	else if (nowStage == 13) return 50;
	else if (nowStage == 14) return 55;
	else if (nowStage == 15) return 55;
	else if (nowStage == 16) return 55;
	else if (nowStage == 17) return 50;

	else if (nowStage == 18) return 50;
	else if (nowStage == 19) return 54;
	else if (nowStage == 20) return 54;
	else if (nowStage == 21) return 54;
	else if (nowStage == 22) return 50;

	else if (nowStage == 23) return 50;
	else if (nowStage == 24) return 53;
	else if (nowStage == 25) return 50;
	else if (nowStage == 26) return 53;
	else if (nowStage == 27) return 50;
	else if (nowStage == 28) return 50;

	else { return rand() % 10; }
}

void StageGroup::Start()
{
	for (int i = 0; i < StageManager::StageSize; i++)
	{
		object->AddChild("Stage")->AddComponent<Stage>()->SetType(StageManager::GetInstance()->StageSet[StageManager::GetInstance()->GetIndex()]->Get(i), i);
	}
}

void StageGroup::Update()
{
	position -= GetTimeManager->ScaleDeltaTime() * StageManager::GetInstance()->moveSpeed * StageManager::GetInstance()->moveSpeedScale;

	if (position < -2.5f)
	{
		position += StageManager::StageListSize * 1.0f;
		ResetChild();
	}

	TransformUpdate();
}

void StageGroup::ResetChild()
{
	index = 0;

	int num = rand() % StageManager::StageSize;
	for (auto stage : object->ChildList)
	{
		stage->ChildList.clear();
		Stage* stageCom = stage->GetComponent<Stage>();
		stageCom->SetType(StageManager::GetInstance()->StageSet[StageManager::GetInstance()->GetIndex()]->Get(num), stageCom->area);
		num++;
	}
	StageManager::GetInstance()->nowStage++;
}

void StageGroup::TransformUpdate()
{
	if (position >= 0.1f)
	{
		object->GetComponent<Transform>()->position.z = size * scale.z * position * (1 + position * 0.2f);
		object->GetComponent<Transform>()->scale.z = scale.z * (1.0f + position * 0.4f);
	}
	else
	{
		object->GetComponent<Transform>()->position.z = size * scale.z * position * (1 - position * 0.2f);
		object->GetComponent<Transform>()->scale.z = scale.z * (1.0f - position * 0.4f);
	}
}


void Stage::SetType(int type, int area)
{
	this->area = area;
	this->type = type;
	switch (type)
	{
	case 1:
		object->AddChild("block01")->AddComponent<Block01>();
		break;
	case 2:
		object->AddChild("block02")->AddComponent<Block02>();
		break;
	case 3:
		object->AddChild("block03")->AddComponent<Block03>();
		break;
	case 4:
		object->AddChild("block04")->AddComponent<Block04>();
		break;
	case 5:
		object->AddChild("block05")->AddComponent<Block05>();
		break;
	default:
		break;
	}
	object->GetComponent<Transform>()->rotation = Vector3(0.0f, 0.0f, (area + 4) * 22.5f);
}

void Stage::Start()
{
	Mesh* mesh = new Mesh(MeshManager::Data["Stage"]);
	MeshRender* meshRender = object->AddComponent<MeshRender>();
	meshRender->vertexShader = ShaderManager::GetInstance()->vsData["Texture"];
	meshRender->pixelShader = ShaderManager::GetInstance()->psData["Glow2"];
	meshRender->texture->tData[1] = GlowManager::GetInstance()->upSampling;
	meshRender->mesh = mesh;
	object->AddComponent<Glow>();
}