#include "Sprite.hlsli"

Texture2D textureMap : register(t0);
Texture2D noiseTexture : register(t2);
SamplerState samplerState : register(s0);

cbuffer ChargeConstants : register(b3)
{
    float value;
}

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = textureMap.Sample(samplerState, psIn.texcoord) * psIn.color;
    
    float noise = noiseTexture.Sample(samplerState, psIn.texcoord).r;
    
    float alpha = step(value, noise);
    alpha = max(alpha, 0.5f);
    
    //float edgeValue = step(noise - dissolveValue, dissolveValue) * step(dissolveValue, noise) * step(noise, dissolveValue + edgeThrehold);
    
    //color.rgb += edgeColor.rgb * edgeValue;
    //alpha = saturate(alpha + edgeValue);
    
    color.a = alpha;
    
    return color;
}