#include "PostProcess.hlsli"

SamplerState samplerState : register(s0);
Texture2D textureMap : register(t0);
float4 main(PSIn psIn) : SV_TARGET
{   
    float4 color = textureMap.Sample(samplerState, psIn.texcoord);
    
    float2 samplePoint = psIn.texcoord;
    float vignette = length(float2(0.5, 0.5) - psIn.texcoord);
    //vignette = clamp(vignette - vignetteValue, 0, 1);
    vignette = clamp(vignette - 0.2, 0, 1);
    color.rgb -= vignette;
    
    return color;
}