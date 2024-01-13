#include "FbxModel.hlsli"

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState samplerState : register(s0);

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = colorMap.Sample(samplerState, psIn.texcoord); // * psIn.color;

    const float GAMMA = 2.2f;
    color.rgb = pow(color.rgb, GAMMA);
    
    float3 N = normalize(psIn.worldNormal.xyz);
    float3 L = normalize(-lightDirection.xyz);
    float irradiance = 0.5f * dot(N, L) + 0.5f;
    color.rgb *= irradiance;
    
    color.rgb += 0.2f;
    
    return float4(color.rgb, 1.0f);
}