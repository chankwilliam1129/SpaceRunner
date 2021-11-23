#include "../GameLib/shader.h"
#include "camera.h"
#include "transform.h"
#include "light.h"

void CameraManager::Initialize()
{
	name = "CameraManager";
	sceneConstantBuffer = std::make_shared<ConstantBuffer<CbScene>>(GetSystemManager->device.Get());

	ShaderManager::GetInstance()->vsData["Mesh"]->cbData[0] = sceneConstantBuffer.get();
	ShaderManager::GetInstance()->vsData["SkinMesh"]->cbData[0] = sceneConstantBuffer.get();
	ShaderManager::GetInstance()->vsData["Texture"]->cbData[0] = sceneConstantBuffer.get();
}

void CameraManager::Update()
{
#ifdef USE_IMGUI
	ImGui::Begin(name.c_str());
#endif	
	for (auto& it : ObjectList)
	{		
		it->Update();		

#ifdef USE_IMGUI
		if (ImGui::CollapsingHeader(it->name.c_str()))
		{
			for (auto& com : it->ComponentList)
			{
				com->ImGuiUpdate();
			}
			ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
		}
#endif
	}
#ifdef USE_IMGUI
	ImGui::End();
#endif	
}

void CameraManager::Release()
{
	ObjectList.clear();
}

Object* CameraManager::Instance(std::string name, Vector3 pos, Vector3 rotation)
{
	Object* obj = new Object;
	obj->name = name;
	obj->AddComponent<Transform>();
	obj->GetComponent<Transform>()->position = pos;
	obj->GetComponent<Transform>()->rotation = rotation;
	ObjectList.push_back(obj);
	return obj;
}

void Camera::Start()
{
	fieldOfView = DirectX::XMConvertToRadians(30.0f);
	aspect = ViewPortManager::GetInstance()->screenWidth / ViewPortManager::GetInstance()->screenHeight;
	farClipPlane = 1000.0f;
	nearClipPlane = 0.01f;
	projection = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspect, nearClipPlane, farClipPlane);
}

void Camera::Update()
{
	DirectX::XMVECTOR pos, target, up;
	pos = object->GetComponent<Transform>()->GetWorldPosition().ToXMVECTOR();
	target = (object->GetComponent<Transform>()->Forward() + object->GetComponent<Transform>()->position).ToXMVECTOR();
	up = Vector3(sinf(object->GetComponent<Transform>()->rotation.z * 0.01745f), cosf(object->GetComponent<Transform>()->rotation.z * 0.01745f), 0.0f).ToXMVECTOR();

	LightManager::GetInstance()->sceneConstantBuffer->data.lightDirection = DirectX::XMFLOAT4(-sinf(object->GetComponent<Transform>()->rotation.z * 0.01745f), -cosf(object->GetComponent<Transform>()->rotation.z * 0.01745f), 1.0f, 1.0f);

	DirectX::XMFLOAT4X4 viewProjection;
	DirectX::XMStoreFloat4x4(&viewProjection, DirectX::XMMatrixLookAtLH(pos, target, up) * projection);
	CameraManager::GetInstance()->sceneConstantBuffer->data.viewProjection = viewProjection;
	CameraManager::GetInstance()->sceneConstantBuffer->UpdateSubresource(GetSystemManager->context.Get());
}