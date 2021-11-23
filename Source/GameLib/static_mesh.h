#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <wrl.h>
#include <vector>

class StaticMesh
{
protected:
    struct Vertex
    {
        DirectX::XMFLOAT3 position = {};
        DirectX::XMFLOAT3 normal   = {};
        DirectX::XMFLOAT2 texcoord = {};
    };

    struct Cbuffer
    {
        DirectX::XMFLOAT4X4 worldViewProjection = {};
        DirectX::XMFLOAT4X4 world = {};
        DirectX::XMFLOAT4   materialColor = {};
        DirectX::XMFLOAT4   lightDirection = {};
    };

    struct Subset //共通の頂点バッファを使用し、マテリアル単位で描画するためのメッシュ情報
    {
        std::wstring usemtl;
        u_int indexStart = 0; // start number of index buffer
        u_int indexCount = 0; // number of vertices (indices)
    };
    std::vector<Subset> subsets;
   /*   ①.obj から usemtl を探す。
        ②usemtl単位でサブセットデータを作る。
        ③usemtl の名前と.mtl の newmtl の名前が一致したマテリアルを関連付ける。*/
    struct Material
    {
        std::wstring newmtl;
        DirectX::XMFLOAT3 Ka = { 0.2f, 0.2f, 0.2f };
        DirectX::XMFLOAT3 Kd = { 0.8f, 0.8f, 0.8f };
        DirectX::XMFLOAT3 Ks = { 1.0f, 1.0f, 1.0f };
        u_int illum = 1;
        std::wstring map_Kd;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
    };
    std::vector<Material> materials;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer    = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixelShader  = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>  inputLayout  = nullptr;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerStates[2] = { nullptr };
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState   = nullptr;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>       samplerState       = nullptr;

public:
	StaticMesh(ID3D11Device* device, const wchar_t* obj_fileName, bool flipping_v_coordinates);
    ~StaticMesh();
    void Initialize(ID3D11Device*);
    void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& worldViewProjection, const DirectX::XMFLOAT4X4& world, 
        const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& materialColor, bool wireframe = false);

private:
    void CreateBuffers(ID3D11Device* device, Vertex* vertices, int numVertices, u_int* indices, int numIndices);
};