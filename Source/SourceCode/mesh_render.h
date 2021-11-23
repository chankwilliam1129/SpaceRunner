#pragma once
#include "component.h"
#include "../GameLib/mesh.h"
#include "../GameLib/model_resource.h"
#include "../GameLib/shader.h"
#include "../GameLib/texture.h"
#include "../GameLib/render_texture.h"

class MeshRender :public Component
{
public:
	Shader* pixelShader  = nullptr;
	Shader* vertexShader = nullptr;
	Mesh*   mesh         = nullptr;
	Texture* texture     = nullptr;

public:
	MeshRender() { texture = new Texture; };
	void SetMaterialColor(Vector4 color);
public:
	void Draw();
	void TextureDraw();
};

class SkinMeshRender :public Component
{
public:
	Shader*   pixelShader  = nullptr;
	Shader*   vertexShader = nullptr;
	SkinMesh* skinMesh     = nullptr;
	Texture*  texture      = nullptr;

public:
	SkinMeshRender() { texture = new Texture; };

public:
	void Draw();
	void TextureDraw();
};


