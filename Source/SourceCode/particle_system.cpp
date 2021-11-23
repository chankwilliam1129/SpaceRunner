#include "particle_system.h"
#include "glow.h"

DirectX::XMMATRIX Particle::GetMatrix()
{
	DirectX::XMMATRIX S, R, T, M;
	M = DirectX::XMMatrixIdentity();

	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	R = Quaternion::ToMatrix(Quaternion::Euler(rotation));
	DirectX::XMStoreFloat4x4(&orientation, R);

	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	M = S * R * T;
	return	M;
};

Vector3 Particle::Forward()
{
	return { orientation._31, orientation._32,orientation._33 };
}

void ParticleSystem::Start()
{
	vertexShader = ShaderManager::GetInstance()->vsData["Mesh"];
	pixelShader = ShaderManager::GetInstance()->psData["Mesh"];
	mesh = new Mesh(MeshManager::Data["Cube"]);

	particleList.clear();
}

Particle* ParticleSystem::Spawn()
{
	Particle* p = new Particle;
	p->life = startLifeTime;
	p->position = object->GetComponent<Transform>()->GetWorldPosition();
	p->rotation = object->GetComponent<Transform>()->GetWorldRotation();
	p->scale = object->GetComponent<Transform>()->GetWorldScale() * startSize;
	particleList.push_back(p);
	return p;
}

void ParticleSystem::Update()
{

	TimeCounter += GetTimeManager->ScaleDeltaTime();
	while (TimeCounter > Frequency)
	{
		Spawn();
		TimeCounter -= Frequency;
		if (Frequency <= 0)Frequency = 0.01f;
	}

	for (auto p : particleList)
	{
		p->matrix = p->GetMatrix();
		p->position += p->Forward() * startSpeed;
		p->scale.x *= 0.82f;
		p->life -= GetTimeManager->ScaleDeltaTime();
	}
	particleList.remove_if([](Particle* p) {return p->life <= 0; });
}

void ParticleSystem::Draw()
{
	DepthStencil::Set(DepthStencil::DEPTH_3D, DepthStencil::STENCIL_3D);
	vertexShader->Activate(GetSystemManager->context.Get());
	pixelShader->Activate(GetSystemManager->context.Get());


	Blender::Set(Blender::ADD);
	for (auto p : particleList)
	{
		if (mesh) 
		{
			mesh->subsetConstantBuffer->data.materialColor = color;
			mesh->subsetConstantBuffer->data.materialColor.z = p->life / startLifeTime;
			mesh->CalculateLocalTransform();
			mesh->CalculateWorldTransform(p->matrix);
			mesh->Render(GetSystemManager->context.Get());
		}
	}
	Blender::Set(Blender::ALPHA);
}

void ParticleSystem::TextureDraw()
{
	Glow* glow = object->GetComponent<Glow>();
	if (glow != nullptr)
	{
		DepthStencil::Set(DepthStencil::DEPTH_3D, DepthStencil::STENCIL_3D);
		glow->renderTexture->Activate();
		glow->texture->Activate();
		glow->vertexShader->Activate(GetSystemManager->context.Get());
		glow->pixelShader->Activate(GetSystemManager->context.Get());

		Blender::Set(Blender::ALPHA);
		for (auto p : particleList)
		{
			if (mesh)
			{
				mesh->subsetConstantBuffer->data.materialColor = color;
				mesh->CalculateLocalTransform();
				mesh->CalculateWorldTransform(p->matrix);
				mesh->Render(GetSystemManager->context.Get());
			}
		}
		Blender::Set(Blender::ALPHA);
	}
}

void ParticleSystem::ImGuiUpdate()
{
#ifdef USE_IMGUI
	if (ImGui::TreeNode("ParticleSystem"))
	{
		ImGui::BulletText("NumberOfParticle : %d", particleList.size());
		ImGui::DragFloat("Frequency", &Frequency, 0.1f);
		ImGui::DragFloat("startLifeTime", &startLifeTime, 0.1f);
		ImGui::DragFloat("startSpeed", &startSpeed, 0.1f);
		ImGui::DragFloat("startSize", &startSize, 0.1f);
		ImGui::ColorEdit4("Color", &color.x);
		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
#endif
}


