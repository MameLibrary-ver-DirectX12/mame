#include "FbxModel.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState samplerState : register(s0);

float4 main(PSIn psIn) : SV_TARGET
{

    float4 color = colorMap.Sample(samplerState, psIn.texcoord);
    

    return float4(color.rgb, 1.0f);
}