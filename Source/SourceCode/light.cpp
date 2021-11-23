#include "../GameLib/shader.h"
#include "light.h"

XMFLOAT4 Light::lightDir(1, 1, 1, 1);
XMFLOAT4 Light::dirLightColor(1, 1, 1, 1);
XMFLOAT4 Light::ambient(1, 1, 1, 1);
PointLight Light::pointLight[PointMAX];
SpotLight Light::spotLight[SpotMAX];

void Light::Initialize()
{
	ZeroMemory(pointLight, sizeof(PointLight) * PointMAX);
	ZeroMemory(spotLight, sizeof(SpotLight) * SpotMAX);
}

void Light::SetDirLight(XMFLOAT3 dir, XMFLOAT3 color)
{
	float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

	if (d > 0) 
	{ 
		dir.x /= d; 
		dir.y /= d; 
		dir.z /= d; 
	}

	lightDir = XMFLOAT4(dir.x, dir.y, dir.z, 0);
	dirLightColor = XMFLOAT4(color.x, color.y, color.z, 1);
}

void Light::SetAmbient(XMFLOAT3 amb)
{
	ambient = { amb.x,amb.y,amb.z,0 };
}

void Light::SetPointLight(int index, XMFLOAT3 pos, XMFLOAT3 color, float range)
{
	if (index < 0) { return; }
	if (index >= PointMAX) { return; }
	pointLight[index].index = (float)index;
	pointLight[index].range = range;
	pointLight[index].type  = 1.0f;
	pointLight[index].dumy  = 0.0f;
	pointLight[index].pos   = XMFLOAT4(pos.x, pos.y, pos.z, 0);
	pointLight[index].color = XMFLOAT4(color.x, color.y, color.z, 0);
}

void Light::SetSpotLight(int index, XMFLOAT3 pos, XMFLOAT3 color, XMFLOAT3 dir,
	float range, float innerCorn, float outerCorn)
{
	if (index < 0) { return; }
	if (index >= SpotMAX) { return; }
	float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (d > 0) 
	{
		dir.x /= d; 
		dir.y /= d; 
		dir.z /= d;
	}

	spotLight[index].index = (float)index;
	spotLight[index].range = range;
	spotLight[index].type = 1.0f;
	spotLight[index].innerCorn = innerCorn;
	spotLight[index].outerCorn = outerCorn;
	spotLight[index].dumy0 = 0.0f;
	spotLight[index].dumy1 = 0.0f;
	spotLight[index].dumy2 = 0.0f;

	spotLight[index].pos = XMFLOAT4(pos.x, pos.y, pos.z, 0);
	spotLight[index].color = XMFLOAT4(color.x, color.y, color.z, 0);
	spotLight[index].dir = XMFLOAT4(dir.x, dir.y, dir.z, 0);
}

void LightManager::Initialize()
{
	sceneConstantBuffer = std::make_shared<ConstantBuffer<CbScene>>(GetSystemManager->device.Get());
	ShaderManager::GetInstance()->vsData["Mesh"]->cbData[1] = sceneConstantBuffer.get();
	ShaderManager::GetInstance()->vsData["SkinMesh"]->cbData[1] = sceneConstantBuffer.get();
}