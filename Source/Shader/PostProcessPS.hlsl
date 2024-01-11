#include "PostProcess.hlsli"

SamplerState samplerState : register(s0);
Texture2D textureMap : register(t0);
float4 main(PSIn psIn) : SV_TARGET
{    
    return textureMap.Sample(samplerState, psIn.texcoord);
}