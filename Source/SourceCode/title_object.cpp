#include "title_object.h"
#include "game_score.h"

void TitleManager::Initialize()
{
	name = "TitleManager";
	Instance("Player", Vector3(.0f, .0f, .0f), Vector3(.0f, 130.0f, .0f), Vector3(0.1f, 0.1f, 0.1f))->AddComponent<TitlePlayer>();
	Instance("GameScore", Vector3(550.0f, 600.0f, 0.0f))->AddComponent<GameScore>();
	Instance("TitleName", Vector3(100.0f, 100.0f, 0.0f))->AddComponent<TitleName>();
	Instance("Text")->AddComponent<TitleText>();
	explanatoryText = Instance("ExplanatoryText")->AddComponent<ExplanatoryText>();
	explanatoryText2 = Instance("ExplanatoryText2")->AddComponent<ExplanatoryText2>();
}

void TitleManager::Update()
{
	if (SceneManager::GetInstance()->scene->state == 1) { explanatoryText->object->isActive = true; }
	else { explanatoryText->object->isActive = false; }
	if (SceneManager::GetInstance()->scene->state == 2) { explanatoryText2->object->isActive = true; }
	else { explanatoryText2->object->isActive = false; }
	GameObjectManager::Update();
}

void TitlePlayer::Start()
{
	object->AddComponent<SkinMeshRender>();

	object->GetComponent<SkinMeshRender>()->skinMesh = new SkinMesh(MeshManager::Data["Robot"]);
	object->GetComponent<SkinMeshRender>()->vertexShader = ShaderManager::GetInstance()->vsData["SkinMesh"];
	object->GetComponent<SkinMeshRender>()->pixelShader = ShaderManager::GetInstance()->psData["Mesh"];

	object->GetComponent<SkinMeshRender>()->skinMesh->PlayAnimation(0, true);
}

void TitlePlayer::Update()
{
	object->GetComponent<SkinMeshRender>()->skinMesh->UpdateAnimation(GetTimeManager->ScaleDeltaTime() * 1.0f);
}


void TitleName::Start()
{
	object->AddComponent<SpriteRender>();
	object->GetComponent<SpriteRender>()->vertexShader = ShaderManager::GetInstance()->vsData["Sprite"];
	object->GetComponent<SpriteRender>()->pixelShader = ShaderManager::GetInstance()->psData["Sprite"];
	object->GetComponent<SpriteRender>()->texture->tData[0] = TextureManager::GetInstance()->tData["Title"];
	object->GetComponent<SpriteRender>()->Setup(Vector2(520.0f, 250.0f), Vector2(520.0f, 250.0f), SpriteRender::PivotType::UpLeft);
}

void TitleName::Update()
{

}

void TitleText::Start()
{
	object->AddComponent<TextRender>()->AddText(L" PUSH SPACE ", Vector2(780.0f, 600.0f));
	object->isActive = true;
}

void ExplanatoryText::Start()
{
	text = object->AddComponent<TextRender>();
	text->object->GetComponent<TextRender>()->AddText(
		L" 移動キー : 移動 \n SPACE離し：ジャンプ\n SPACE長押し スライディング & 加速 (スピードが速いほど高得点)",
		Vector2(0.0f, 330.0f));
}

void ExplanatoryText2::Start()
{
	text = object->AddComponent<TextRender>();
	text->object->GetComponent<TextRender>()->AddText(
		L" ジャンプ後 SPACE : 空中ダッシュ　& 最大速度に",
		Vector2(0.0f, 330.0f));
}