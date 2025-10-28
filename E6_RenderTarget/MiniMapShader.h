#pragma once
#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class MiniMapShader : public BaseShader
{
private:
    struct PlayerBufferType
    {
        XMFLOAT2 playerPos;
        XMFLOAT2 mapSize;
        float markerSize;
        XMFLOAT3 padding;
    };

public:
    MiniMapShader(ID3D11Device* device, HWND hwnd);
    ~MiniMapShader();

    void setShaderParameters(
        ID3D11DeviceContext* deviceContext,
        const XMMATRIX &world, 
        const XMMATRIX &view, 
        const XMMATRIX &projection,
        ID3D11ShaderResourceView* texture,
        XMFLOAT2 playerPosition,
        XMFLOAT2 mapSize,
        float markerSize
    );

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11Buffer* playerBuffer;  // buffer ใหม่
    ID3D11SamplerState* sampleState;
};
