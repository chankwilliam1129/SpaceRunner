#pragma once
#include <DirectXMath.h>
#include <stdio.h>
#include <string>
#include "system.h"
#include "resource_manager.h"
#include "misc.h"
#include "vector.h"
#include "texture.h"

class Sprite 
{
private:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcoord;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> verteBuffer = nullptr;

public:
    static const D3D11_INPUT_ELEMENT_DESC* CreateInputElementDesc(size_t& numElements)
    {
        static const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },          
        };
        numElements = _countof(inputElementDesc);
        return inputElementDesc;
    }

public:
    Sprite();
    ~Sprite();
    void Render(
        ID3D11DeviceContext* context,
        float dx, float dy, //position 
        float dw, float dh, //scale
        float sx, float sy, //texposition
        float sw, float sh, //drawtexsize
        float tw, float th, //texsize
        float cx, float cy, //center
        float angle,
        DirectX::XMFLOAT4 color
    );
};

class SpriteBatch
{
private:

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixelShader  = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>  inputLayout  = nullptr;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState>    rasterizerState   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  depthStencilState = nullptr;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>       samplerState = nullptr;

    D3D11_TEXTURE2D_DESC  texture2dDesc = {};

    size_t MAX_INSTANCES;

    struct Instance
    {
        DirectX::XMFLOAT4X4 ndcTransform = {};
        Vector4 texcoordTransform = {};
        Vector4 color = {};
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuffer;

public:
    struct Vertex
    {
        Vector3 position = {};
        Vector2 texcoord = {};
    };

public:
    SpriteBatch(ID3D11Device*, const wchar_t*, size_t);
    ~SpriteBatch();

public:
    void Begin(ID3D11DeviceContext*);
    void Render(
        const  Vector2&, const  Vector2&,
        const  Vector2&, const  Vector2&,
        const  Vector2&, float angle,
        const  Vector4&);
    float Textout(std::string, const  Vector2&,
        const  Vector2&, const  Vector4&, bool world = (false));
    void End(ID3D11DeviceContext*);

private:

    D3D11_VIEWPORT viewport = {};

    u_int instanceCount  = 0;
    Instance* instances  = nullptr;
};