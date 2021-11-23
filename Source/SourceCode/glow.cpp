#include "glow.h"
#include "blur.h"
#include "stage.h"

void Glow::Start()
{
	renderTexture = GlowManager::GetInstance()->glowTexture;
	vertexShader = ShaderManager::GetInstance()->vsData["Mesh"];
	pixelShader = ShaderManager::GetInstance()->psData["Mesh"];
}


void GlowManager::Initialize()
{
	glowTexture = new RenderTexture(GetSystemManager->device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	cbGlow = new ConstantBuffer<CbGlow>(GetSystemManager->device.Get());
	ShaderManager::GetInstance()->psData["Glow"]->cbData[4] = cbGlow;
	ShaderManager::GetInstance()->psData["Glow2"]->cbData[4] = cbGlow;
	Instance("GlowBlur")->AddComponent<GlowBlur>();
}

void GlowManager::Update()
{
	glowTexture->ClearRenderTarget(0, 0, 0, 1);
	ImguiUpdate();
}

void GlowManager::TextureRender()
{
	GameObjectManager::Render();
	WindowManager::GetInstance()->SetRenderTarget();
	ViewPortManager::GetInstance()->SetViewPorts(1, ViewPortManager::GetInstance()->viewPorts);
}

void GlowManager::ImguiUpdate()
{
#ifdef USE_IMGUI
	if (ImGui::TreeNode("StageManager"))
	{
		ImGui::DragFloat("StageGlowStrength", &StageManager::GetInstance()->StageGlowStrength, 0.1f);
		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
#endif
}




