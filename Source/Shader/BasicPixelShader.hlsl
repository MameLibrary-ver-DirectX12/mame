#include "BasicShader.hlsli"

Texture2D<float4> textureMap : register(t0);
SamplerState samplerState : register(s0);

float4 main(Output psIn) : SV_TARGET
{
    return float4(textureMap.Sample(samplerState, psIn.texcoord));

}