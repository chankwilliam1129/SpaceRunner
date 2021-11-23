#pragma once
#include "../GameLib/render_texture.h"
#include "../GameLib/constant_buffer.h"
#include "component.h"

class OrthoWindow :public Component
{
public:
	RenderTexture* renderTexture;
	SpriteRender* spriteRender;

public:
	virtual void Draw() 
	{
		renderTexture->ClearRenderTarget(0, 0, 0, 1);
		renderTexture->SetRenderTarget();
	}
};

class DebugOrthoWindow :public Component
{
public:
	void Draw()
	{
		WindowManager::GetInstance()->SetRenderTarget();
		ViewPortManager::GetInstance()->SetViewPorts(1, ViewPortManager::GetInstance()->viewPorts);
	}
};


class Sampling :public OrthoWindow
{
public:
	void Start();
};

class DebugSampling :public DebugOrthoWindow
{
public:
	SpriteRender* spriteRender;
public:
	void Start();
	void Draw() 
	{
		Blender::Set(Blender::ADD);
		DebugOrthoWindow::Draw();
	};
};

class HorizontalBlur :public OrthoWindow
{
private:
	struct CbObject 
	{
		float screenWidth;
		int padding[3];
	};
	ConstantBuffer<CbObject>* cbObject;

public:
	void Start();
};

class VerticalBlur :public OrthoWindow
{
private:
	struct CbObject 
	{
		float screenHeight;
		int padding[3];
	};
	ConstantBuffer<CbObject>* cbObject;

public:
	void Start();
};

class GausianBlur :public OrthoWindow
{
protected:
	struct CbObject 
	{
		int SampleCount;
		int Dummy[3];
		Vector4 Offset[16];   
	};
	ConstantBuffer<CbObject>* cbObject;

	float GaussianDistribution(const Vector2& pos, float rho)
	{
		return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
	}

	CbObject CalcBlurParam(int width, int height, Vector2 dir, float deviation, float multiply);
};

class GausianHBlur : public GausianBlur 
{
public:
	void Start();
};

class GausianVBlur : public GausianBlur
{
public:
	void Start();
};
