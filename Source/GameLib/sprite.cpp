#include <assert.h>
#include "sprite.h"

Sprite::Sprite()
{
    ID3D11Device* device = GetSystemManager->device.Get();

	HRESULT	hr = S_OK;

	//	仮頂点定義
	Vertex vertices[] = 
	{ 
		{ DirectX::XMFLOAT3(-0.5, +0.5, 0), DirectX::XMFLOAT4(1, 1, 1, 1) },   
		{ DirectX::XMFLOAT3(+0.5, +0.5, 0), DirectX::XMFLOAT4(1, 0, 0, 1) },   
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0), DirectX::XMFLOAT4(0, 1, 0, 1) },    
		{ DirectX::XMFLOAT3(+0.5, -0.5, 0), DirectX::XMFLOAT4(0, 0, 1, 1) }, 
	};

	//	頂点バッファ作成
	D3D11_BUFFER_DESC bufferDesc    = {};
	bufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth            = sizeof(vertices) * 4;
	bufferDesc.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem                = vertices;
	hr = device->CreateBuffer(&bufferDesc, &InitData, verteBuffer.GetAddressOf());
	if (FAILED(hr))	assert(0 && "頂点バッファ作成失敗");
}

Sprite::~Sprite()
{
    if (verteBuffer.Get()) verteBuffer->Release();
};

void Sprite::Render(
    ID3D11DeviceContext* context,
    float dx, float dy,
    float dw, float dh,
    float sx, float sy,
    float sw, float sh,
    float tw, float th, 
    float cx, float cy,
	float angle, 
	DirectX::XMFLOAT4 color
)
{
	//	単位矩形定義
	Vertex vertices[4] = 
	{
		{ DirectX::XMFLOAT3(0.0f, 0.0f, .0f), color, DirectX::XMFLOAT2(sx,      sy) },
		{ DirectX::XMFLOAT3(1.0f, 0.0f, .0f), color, DirectX::XMFLOAT2(sx + sw, sy) },
		{ DirectX::XMFLOAT3(0.0f, 1.0f, .0f), color, DirectX::XMFLOAT2(sx,      sy + sh) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, .0f), color, DirectX::XMFLOAT2(sx + sw, sy + sh) },
	};

	//	(dw,dh)の拡大
	for (int i = 0; i < 4; i++) 
	{
        vertices[i].position.x *= dw * sw;
        vertices[i].position.y *= dh * sh;
	}

	//	回転軸の変更（矩形中央へ）
    float mx = (sw * dw) / sw * cx;
    float my = (sh * dh) / sh * cy;
	for (int i = 0; i < 4; i++) 
	{
		vertices[i].position.x -= mx;
		vertices[i].position.y -= my;
	}

	//	angle度の回転
	float degree = DirectX::XMConvertToRadians(angle);
	float sn = sinf(degree);
	float cs = cosf(degree);
	for (int i = 0; i < 4; i++)
	{
		float bx = vertices[i].position.x;
		float by = vertices[i].position.y;
		vertices[i].position.x = bx * cs - by * sn;
		vertices[i].position.y = bx * sn + by * cs;
	}

	//	回転軸の変更（元の位置へ）
	for (int i = 0; i < 4; i++) 
	{
		vertices[i].position.x += mx;
		vertices[i].position.y += my;
	}

	//	(dx,dy)の平行移動
	for (int i = 0; i < 4; i++) 
	{
        vertices[i].position.x += (dx - dw * cx);
		vertices[i].position.y += (dy - dh * cy);
	}

    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    context->RSGetViewports(&numViewports, &viewport);

	Vertex verticesNDC[4] = {};
	for (int n = 0; n < 4; n++)
	{
		//	NDC変換
		verticesNDC[n].position.x = (vertices[n].position.x / viewport.Width) * 2.0f - 1.0f;
		verticesNDC[n].position.y = -(vertices[n].position.y / viewport.Height) * 2.0f + 1.0f;
		verticesNDC[n].position.z = .0f;
		verticesNDC[n].color = vertices[n].color;

		//	UV座標正規化
		verticesNDC[n].texcoord.x = vertices[n].texcoord.x / tw;
		verticesNDC[n].texcoord.y = vertices[n].texcoord.y / th;
	}

	D3D11_MAPPED_SUBRESOURCE subResource = {};
	context->Map(verteBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, verticesNDC, sizeof(verticesNDC));
	context->Unmap(verteBuffer.Get(), 0);

	// 頂点バッファのバインド
	u_int stride = sizeof(Vertex);
	u_int offset = 0;
	context->IASetVertexBuffers(0, 1, verteBuffer.GetAddressOf(), &stride, &offset);

	//	プリミティブモードの設定
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//	プリミティブの描画
	context->Draw(4, 0);
}

    SpriteBatch::SpriteBatch(ID3D11Device* device, const wchar_t* fileName, size_t maxInstance)
    {
        HRESULT	hr = S_OK;

        MAX_INSTANCES = maxInstance;

        //VertexBufferの作成
        Vertex vertices[4] =
        {
            { Vector3(0, 0, 0), Vector2(0, 0) },
            { Vector3(1, 0, 0), Vector2(1, 0) },
            { Vector3(0, 1, 0), Vector2(0, 1) },
            { Vector3(1, 1, 0), Vector2(1, 1) },
        };

        D3D11_BUFFER_DESC bufferDesc   = {};
        bufferDesc.ByteWidth           = sizeof(vertices);
        bufferDesc.Usage               = D3D11_USAGE_IMMUTABLE;
        bufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags      = 0;
        bufferDesc.MiscFlags           = 0;
        bufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem                = vertices;
        subresourceData.SysMemPitch            = 0; //Not use for vertex buffers.
        subresourceData.SysMemSlicePitch       = 0; //Not use for vertex buffers.

        hr = device->CreateBuffer(&bufferDesc, &subresourceData, buffer.GetAddressOf());
        if (FAILED(hr))	assert(0 && "頂点バッファ作成失敗(SpriteBatch)");

        D3D11_INPUT_ELEMENT_DESC layout[] = 
        {
            { "POSITION",           0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
            { "TEXCOORD",           0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
            { "NDC_TRANSFORM",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM",      1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM",      2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM",      3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "TEXCOORD_TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "COLOR",              0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        };

        const char* vsName = "Data/Shaders/CSO/sprite_batch_vs.cso";
        const char* psName = "Data/Shaders/CSO/sprite_batch_ps.cso";

        //	頂点シェーダーオブジェクト & 入力レイアウトの生成
        ResourceManager::CreateVertexShaders(device, vsName, layout, ARRAYSIZE(layout), vertexShader.GetAddressOf(), inputLayout.GetAddressOf());

        //  ピクセルシェーダーオブジェクトの作成
        ResourceManager::CreatePixelShaders(device, psName, pixelShader.GetAddressOf());

        Instance* inst = new Instance[MAX_INSTANCES];
        {
            D3D11_BUFFER_DESC bufferDesc = {};
            D3D11_SUBRESOURCE_DATA subresourceData = {};

            bufferDesc.ByteWidth             = sizeof(Instance) * MAX_INSTANCES;
            bufferDesc.Usage                 = D3D11_USAGE_DYNAMIC;
            bufferDesc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.CPUAccessFlags        = D3D11_CPU_ACCESS_WRITE;
            bufferDesc.MiscFlags             = 0;
            bufferDesc.StructureByteStride   = 0;
            subresourceData.pSysMem          = inst;
            subresourceData.SysMemPitch      = 0; //Not use for vertex buffers.mm 
            subresourceData.SysMemSlicePitch = 0; //Not use for vertex buffers.

            hr = device->CreateBuffer(&bufferDesc, &subresourceData, instanceBuffer.GetAddressOf());
            if (FAILED(hr)) assert(0 && "頂点バッファ作成失敗(SpriteBatch)");
        }
        delete[] inst;

        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode              = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
        rasterizerDesc.CullMode              = D3D11_CULL_NONE; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
        rasterizerDesc.FrontCounterClockwise = FALSE;
        rasterizerDesc.DepthBias             = 0;
        rasterizerDesc.DepthBiasClamp        = 0;
        rasterizerDesc.SlopeScaledDepthBias  = 0;
        rasterizerDesc.DepthClipEnable       = FALSE;
        rasterizerDesc.ScissorEnable         = FALSE;
        rasterizerDesc.MultisampleEnable     = FALSE;
        rasterizerDesc.AntialiasedLineEnable = FALSE;

        hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
        if (FAILED(hr)) assert(0 && "ラスタライザー作成失敗(SpriteBatch)");

        //	テクスチャ読み込み
        ResourceManager::CreateShaderResourceView(device, fileName, shaderResourceView.GetAddressOf(), texture2dDesc);

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter             = D3D11_FILTER_MIN_MAG_MIP_POINT;
        samplerDesc.AddressU           = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV           = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW           = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.MipLODBias         = 0;
        samplerDesc.MaxAnisotropy      = 16;
        samplerDesc.ComparisonFunc     = D3D11_COMPARISON_ALWAYS;
        for (int i = 0; i < 4; i++)
        {
            samplerDesc.BorderColor[i] = 0;
        }
        samplerDesc.MinLOD             = 0;
        samplerDesc.MaxLOD             = D3D11_FLOAT32_MAX;

        hr = device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
        if (FAILED(hr))	assert(0 && "サンプラーステート作成失敗(SpriteBatch)");

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc     = {};
        depthStencilDesc.DepthEnable                  = false;
        depthStencilDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
        depthStencilDesc.DepthFunc                    = D3D11_COMPARISON_ALWAYS;
        depthStencilDesc.StencilEnable                = false;
        depthStencilDesc.StencilReadMask              = 0xFF;
        depthStencilDesc.StencilWriteMask             = 0xFF;
        depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
        depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
        depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

        hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
        if (FAILED(hr)) assert(0 && "深度ステンシルState作成失敗(SpriteBatch)");
    }

    SpriteBatch::~SpriteBatch()
    {
        if (buffer.Get()) buffer->Release();

        if (vertexShader.Get()) vertexShader->Release();
        if (pixelShader.Get()) pixelShader->Release();
        if (inputLayout.Get()) inputLayout->Release();

        if (rasterizerState.Get()) rasterizerState->Release();;
        if (depthStencilState.Get()) depthStencilState->Release();

        if (shaderResourceView.Get()) shaderResourceView->Release();
        if (samplerState.Get()) samplerState->Release();

        if (instanceBuffer.Get()) instanceBuffer->Release();
    }

    void SpriteBatch::Begin(ID3D11DeviceContext* context)
    {
        HRESULT hr = S_OK;

        UINT strides[2] = { sizeof(Vertex), sizeof(Instance) };
        UINT offsets[2] = { 0, 0 };
        ID3D11Buffer* vbs[2] = { buffer.Get(), instanceBuffer.Get() };

        context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout.Get());
        context->OMSetDepthStencilState(depthStencilState.Get(), 1);
        context->RSSetState(rasterizerState.Get());
        context->VSSetShader(vertexShader.Get(), nullptr, 0);
        context->PSSetShader(pixelShader.Get(), nullptr, 0);
        context->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
        context->PSSetSamplers(0, 1, samplerState.GetAddressOf());

        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
        D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
        hr = context->Map(instanceBuffer.Get(), 0, map, 0, &mappedBuffer);
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
        instances = static_cast<Instance*>(mappedBuffer.pData);

        instanceCount = 0;
    }

    void SpriteBatch::Render(
        const Vector2& position, const Vector2& scale,
        const Vector2& texPos, const Vector2& texSize,
        const Vector2& center, float angle/*radian*/,
        const Vector4& color)
    {
        if (instanceCount >= MAX_INSTANCES)
        {
            assert(!"Number of instances must be less than MAX_INSTANCES.(SpriteBatch)");
            return;
        }

        if (scale.x == 0.0f || scale.y == 0.0f) return;
        float tw = texSize.x;
        float th = texSize.y;
        if (texSize.x <= 0.0f || texSize.y <= 0.0f)
        {
            tw = (float)texture2dDesc.Width;
            th = (float)texture2dDesc.Height;
        }

        //	float cx = dw*0.5f, cy = dh*0.5f; /*Center of Rotation*/
        float cx = center.x;
        float cy = center.y;
        float sx = scale.x;
        float sy = scale.y;

#ifdef GAMELIB_PLUS_UP
        sy = -sy; // Y軸上が正ならスケール反転
        cy = texSize.y - cy;
#endif
        cx *= sx;
        cy *= sy;

#if 0
        DirectX::XMVECTOR scaling = DirectX::XMVectorSet(dw, dh, 1.0f, 0.0f);
        DirectX::XMVECTOR origin = DirectX::XMVectorSet(cx, cy, 0.0f, 0.0f);
        DirectX::XMVECTOR translation = DirectX::XMVectorSet(dx, dy, 0.0f, 0.0f);
        DirectX::XMMATRIX M = DirectX::XMMatrixAffineTransformation2D(scaling, origin, angle * 0.01745f, translation);
        DirectX::XMMATRIX N(
            2.0f / viewport.Width, 0.0f, 0.0f, 0.0f,
            0.0f, -2.0f / viewport.Height, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f);
        XMStoreFloat4x4(&instances[count_instance].ndc_transform, DirectX::XMMatrixTranspose(M * N)); //column_major
#else
        FLOAT c = cosf(angle);
        FLOAT s = sinf(angle);
        FLOAT w = 2.0f / viewport.Width;
        FLOAT h = -2.0f / viewport.Height;

        instances[instanceCount].ndcTransform._11 = w * sx * tw * c;
        instances[instanceCount].ndcTransform._21 = h * sx * tw * s;
        instances[instanceCount].ndcTransform._31 = 0.0f;
        instances[instanceCount].ndcTransform._41 = 0.0f;
        instances[instanceCount].ndcTransform._12 = w * sy * th * -s;
        instances[instanceCount].ndcTransform._22 = h * sy * th * c;
        instances[instanceCount].ndcTransform._32 = 0.0f;
        instances[instanceCount].ndcTransform._42 = 0.0f;
        instances[instanceCount].ndcTransform._13 = 0.0f;
        instances[instanceCount].ndcTransform._23 = 0.0f;
        instances[instanceCount].ndcTransform._33 = 1.0f;
        instances[instanceCount].ndcTransform._43 = 0.0f;
        instances[instanceCount].ndcTransform._14 = w * (-cx * c + -cy * -s + cx * 0 + position.x) - 1.0f;
        instances[instanceCount].ndcTransform._24 = h * (-cx * s + -cy * c + cy * 0 + position.y) + 1.0f;
        instances[instanceCount].ndcTransform._34 = 0.0f;
        instances[instanceCount].ndcTransform._44 = 1.0f;
#ifdef GAMELIB_PLUS_UP
        instances[instanceCount].ndcTransform._21 *= -1;
        instances[instanceCount].ndcTransform._22 *= -1;
        instances[instanceCount].ndcTransform._24 *= -1;
#endif
#endif
        float tex_width = static_cast<float>(texture2dDesc.Width);
        float tex_height = static_cast<float>(texture2dDesc.Height);

        // UV座標の調整
        float u = tw * 0.99994f / tex_width;
        float v = th * 0.99994f / tex_height;

        instances[instanceCount].texcoordTransform = Vector4(texPos.x / tex_width, texPos.y / tex_height, u, v);
        instances[instanceCount].color = color;

        instanceCount++;
    }

    float SpriteBatch::Textout(std::string str,
        const Vector2& pos, const Vector2& scale,
        const Vector4& color,
        bool world)
    {
        float tw = static_cast<float>(texture2dDesc.Width / 16.0);
        float th = static_cast<float>(texture2dDesc.Height / 16.0);
        float cursor = 0.0f;

        for (const auto& c : str)
        {
            Render(Vector2(pos.x + cursor, pos.y), scale, Vector2(tw * (c & 0x0F), th * (c >> 4)),
                Vector2(tw, th), Vector2(0, 0), 0, color);
            cursor += tw * scale.x;
        }
        return th * scale.y;
        
    }

    void SpriteBatch::End(ID3D11DeviceContext* context)
    {
        context->Unmap(instanceBuffer.Get(), 0);
        context->DrawInstanced(4, instanceCount, 0, 0);
    }