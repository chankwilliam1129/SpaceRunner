#pragma once
#include "shader.h"
#include "lambert_shader.h"
#include "singleton.h"

class ShaderManager :public Singleton<ShaderManager>
{
public:
	ShaderManager();
	void Release();

	LambertShader*  lambertShader;
};