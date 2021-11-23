#include "mesh_render.h"
#include "transform.h"
#include "glow.h"

void MeshRender::Draw()
{
	DepthStencil::Set(DepthStencil::DEPTH_3D, DepthStencil::STENCIL_3D);
	texture->Activate();
	vertexShader->Activate(GetSystemManager->context.Get());
	pixelShader->Activate(GetSystemManager->context.Get());
	mesh->CalculateLocalTransform();
	mesh->CalculateWorldTransform(object->GetComponent<Transform>()->GetWorldMatrix());
	mesh->Render(GetSystemManager->context.Get());
}

void MeshRender::TextureDraw()
{
	Glow* glow = object->GetComponent<Glow>();
	if (glow != nullptr)
	{
		DepthStencil::Set(DepthStencil::DEPTH_3D, DepthStencil::STENCIL_3D);
		glow->renderTexture->Activate();
		glow->texture->Activate();
		glow->vertexShader->Activate(GetSystemManager->context.Get());
		glow->pixelShader->Activate(GetSystemManager->context.Get());

		mesh->CalculateLocalTransform();
		mesh->CalculateWorldTransform(object->GetComponent<Transform>()->GetWorldMatrix());
		mesh->Render(GetSystemManager->context.Get());
	}
}

void MeshRender::SetMaterialColor(Vector4 color)
{
	mesh->subsetConstantBuffer->data.materialColor = color;
}

void SkinMeshRender::Draw()
{
	DepthStencil::Set(DepthStencil::DEPTH_3D, DepthStencil::STENCIL_3D);
	texture->Activate();
	vertexShader->Activate(GetSystemManager->context.Get());
	pixelShader->Activate(GetSystemManager->context.Get());
	skinMesh->CalculateLocalTransform();
	skinMesh->CalculateWorldTransform(object->GetComponent<Transform>()->GetWorldMatrix());
	skinMesh->Render(GetSystemManager->context.Get());
}

void SkinMeshRender::TextureDraw()
{
	Glow* glow = object->GetComponent<Glow>();
	if (glow != nullptr)
	{
		DepthStencil::Set(DepthStencil::DEPTH_3D, DepthStencil::STENCIL_3D);
		glow->renderTexture->Activate();
		glow->texture->Activate();
		glow->vertexShader->Activate(GetSystemManager->context.Get());
		glow->pixelShader->Activate(GetSystemManager->context.Get());

		skinMesh->CalculateLocalTransform();
		skinMesh->CalculateWorldTransform(object->GetComponent<Transform>()->GetWorldMatrix());
		skinMesh->Render(GetSystemManager->context.Get());
	}
}