#include "ImGui.hlsli"

Texture2D textureMap : register(t0);
SamplerState samplerState : register(s0);

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = textureMap.Sample(samplerState, psIn.texcoord) * psIn.color;
    
    return color;
}
