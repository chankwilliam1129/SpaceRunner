#include "player.h"
#include "game_score.h"
#include "glow.h"

void ScoreNumber::Start()
{
	object->AddComponent<SpriteRender>();
	object->GetComponent<SpriteRender>()->vertexShader = ShaderManager::GetInstance()->vsData["Sprite"];
	object->GetComponent<SpriteRender>()->pixelShader = ShaderManager::GetInstance()->psData["Sprite"];
	object->GetComponent<SpriteRender>()->texture->tData[0] = TextureManager::GetInstance()->tData["Number"];
	object->GetComponent<SpriteRender>()->Setup(Vector2(100.0f, 135.0f), Vector2(1000.0f, 135.0f), SpriteRender::PivotType::Center);
	number = 0;
}

void ScoreNumber::Update()
{
	object->GetComponent<SpriteRender>()->texturePosition.x = 100.0f * number;
}

void GameScore::Start()
{
	for (int i = 0; i < 6; i++)
	{
		object->AddChild("Number", Vector3(i * -100.0f, 0.0f, 0.0f))->AddComponent<ScoreNumber>()->number = i;
	}
}

void GameScore::Update()
{
	int nowScore = PlayerManager::GetInstance()->score;
	for (auto num : object->ChildList)
	{
		num->GetComponent<ScoreNumber>()->number = nowScore % 10;
		nowScore /= 10;
	}
}