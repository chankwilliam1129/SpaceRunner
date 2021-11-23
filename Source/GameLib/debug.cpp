#include "debug.h"
#include "system.h"
#include "util.h"

Debug::Debug(ID3D11Device* device, const wchar_t* fileName, size_t maxInstance, int scr_h)
    : SpriteBatch(device, fileName, maxInstance), yPos(0), screenHeight(scr_h)
{}

void Debug::SetString(const char* str)
{
    std::string s(str);
    debugQueue.push(s);
}

void Debug::Display(ID3D11DeviceContext* context, float r, float g, float b, float sx, float sy)
{
    yPos = 0;
    Begin(context);
    while (!debugQueue.empty())
    {
        std::string t = debugQueue.front();
        yPos += Textout(t, Vector2(0, yPos), Vector2(sx, sy), Vector4(r, g, b, 1));
        debugQueue.pop();

        if (yPos > screenHeight)
        {
            while (!debugQueue.empty())	debugQueue.pop();
            break;
        }
    }
    End(context);
}
    
// DebugManager
void DebugManager::SetDebug(const wchar_t* fileName, size_t maxInstance, int scr_h)
{
    debug_ = std::make_unique<Debug>(GetSystemManager->device.Get(), fileName, maxInstance, scr_h);
}

void DebugManager::Release()
{
}

namespace debug
{
    void SetString(const char* format, ...)
    {
        char str[256];
        vsprintf_s(str, format, (char*)(&format + 1));
        DebugManager::GetInstance()->debug_->SetString(str);
    }

    void Display(float r, float g, float b, float sx, float sy)
    {
        DebugManager::GetInstance()->debug_->Display(GetSystemManager->context.Get(), r, g, b, sx, sy);
    }
}