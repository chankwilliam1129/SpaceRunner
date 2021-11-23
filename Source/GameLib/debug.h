#pragma once
#include <memory>
#include <queue>
#include "sprite.h"
#include "singleton.h"

class Debug : public SpriteBatch
{
public:
    static const int INSTANCE_NUM = 8192U; // �f�o�b�O�����̍ő�\����

private:
    std::queue<std::string> debugQueue;
    float yPos;
    int screenHeight;

public:
    Debug(ID3D11Device*, const wchar_t*, size_t maxInstance = (8192)/*�����ɕ\������ő啶����*/, int scr_h = (480)/*�X�N���[���̍���*/);
    ~Debug() {};
    void SetString(const char*);
    void Display(ID3D11DeviceContext*, float r = (1), float g = (1), float b = (1), float sx = (1), float sy = (1)); //	�f�o�b�O������̕\��
};

class DebugManager :public Singleton<DebugManager>
{
public:
    std::unique_ptr<Debug> debug_ = nullptr;

public:
    void SetDebug(const wchar_t* fileName, size_t maxInstance, int scr_h);
    void Release();
};

namespace debug
{
    void SetString(const char* format, ...);
    void Display(float r, float g, float b, float sx, float sy);
}