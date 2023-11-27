#include "BasicShader.hlsli"

Texture2D<float4> textureMap : register(t0);
SamplerState samplerState : register(s0);

float4 main(Output psIn) : SV_TARGET
{
    float3 light = normalize(float3(1, -1, 1));
    float brightness = dot(-light, psIn.normal.xyz);
    
    return float4(brightness.xxx, 1.0f);
}