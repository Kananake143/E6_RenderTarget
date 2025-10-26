// minimap pixel shader
// แสดง texture พร้อมจุดบอกตำแหน่งผู้เล่น

Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

// TODO: เพิ่ม constant buffer สำหรับเก็บตำแหน่งผู้เล่น
cbuffer PlayerBuffer : register(b0)
{
    float2 playerPos;      // ตำแหน่ง player ใน world (x, z)
    float2 mapSize;        // ขนาดของแผนที่
    float markerSize;      // ขนาดของจุดบอกตำแหน่ง
    float3 padding;        // padding ให้ครบ 16 bytes
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
    // ดึงสีจาก texture
    float4 textureColor = texture0.Sample(Sampler0, input.tex);
    
    
    
    
    float2 pixelWorldPos = (input.tex - 0.5) * mapSize;
    float dist = distance(pixelWorldPos, playerPos);
    if (dist < markerSize) { 
      return float4(1, 0, 0, 1); 
    }
    
    return textureColor;
}
