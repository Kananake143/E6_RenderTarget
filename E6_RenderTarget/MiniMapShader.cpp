#include "MiniMapShader.h"

MiniMapShader::MiniMapShader(ID3D11Device* device, HWND hwnd) 
    : BaseShader(device, hwnd)
{
    initShader(L"minimap_vs.cso", L"minimap_ps.cso");
}

MiniMapShader::~MiniMapShader()
{
    if (sampleState) {
        sampleState->Release();
        sampleState = 0;
    }
    if (matrixBuffer) {
        matrixBuffer->Release();
        matrixBuffer = 0;
    }
    if (playerBuffer) {  // เพิ่ม
        playerBuffer->Release();
        playerBuffer = 0;
    }
    if (layout) {
        layout->Release();
        layout = 0;
    }
    BaseShader::~BaseShader();
}

void MiniMapShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
    D3D11_BUFFER_DESC matrixBufferDesc;
    D3D11_BUFFER_DESC playerBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;

    loadVertexShader(vsFilename);
    loadPixelShader(psFilename);

    // Matrix buffer (เหมือนเดิม)
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;
    renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

    // Player buffer (ใหม่) - สำหรับส่งข้อมูลตำแหน่งผู้เล่น
    playerBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    playerBufferDesc.ByteWidth = sizeof(PlayerBufferType);
    playerBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    playerBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    playerBufferDesc.MiscFlags = 0;
    playerBufferDesc.StructureByteStride = 0;
    renderer->CreateBuffer(&playerBufferDesc, NULL, &playerBuffer);

    // Sampler (เหมือนเดิม)
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    renderer->CreateSamplerState(&samplerDesc, &sampleState);
}

void MiniMapShader::setShaderParameters(
    ID3D11DeviceContext* deviceContext,
    const XMMATRIX &worldMatrix,
    const XMMATRIX &viewMatrix,
    const XMMATRIX &projectionMatrix,
    ID3D11ShaderResourceView* texture,
    XMFLOAT2 playerPosition,
    XMFLOAT2 mapSize,
    float markerSize)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // ส่ง matrix (เหมือนเดิม)
    XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
    XMMATRIX tview = XMMatrixTranspose(viewMatrix);
    XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

    MatrixBufferType* matrixPtr;
    result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    matrixPtr = (MatrixBufferType*)mappedResource.pData;
    matrixPtr->world = tworld;
    matrixPtr->view = tview;
    matrixPtr->projection = tproj;
    deviceContext->Unmap(matrixBuffer, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

    // ส่งข้อมูล player (ใหม่)
    PlayerBufferType* playerPtr;
    deviceContext->Map(playerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    playerPtr = (PlayerBufferType*)mappedResource.pData;
    playerPtr->playerPos = playerPosition;
    playerPtr->mapSize = mapSize;
    playerPtr->markerSize = markerSize;
    playerPtr->padding = XMFLOAT3(0, 0, 0);
    deviceContext->Unmap(playerBuffer, 0);
    deviceContext->PSSetConstantBuffers(0, 1, &playerBuffer);  // ส่งไปที่ pixel shader

    // ส่ง texture
    deviceContext->PSSetShaderResources(0, 1, &texture);
    deviceContext->PSSetSamplers(0, 1, &sampleState);
}
