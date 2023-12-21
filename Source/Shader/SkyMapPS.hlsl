#include "SkyMap.hlsli"

cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
    row_major float4x4 invViewProjection;
};

Texture2D skyBox : register(t0);
SamplerState samplerState : register(s0);

float4 sampleSkyBox(float3 v, float roughness)
{
    const float PI = 3.14159265358979f;
    
    uint width, height, numberOfLevels;
    skyBox.GetDimensions(0, width, height, numberOfLevels);
    
    float lod = roughness * float(numberOfLevels - 1);
    v = normalize(v);
    
    float2 samplePoint;
    samplePoint.x = (atan2(v.z, v.x) + PI) / (PI * 2.0f);
    samplePoint.y = 1.0f - ((asin(v.y) + PI * 0.5f) / PI);
    return skyBox.SampleLevel(samplerState, samplePoint, lod);
}

float4 main(PSIn psIn) :SV_TARGET
{
    float4 ndc;
    ndc.x = (psIn.texcoord.x * 2.0f) - 1.0f;
    ndc.y = 1.0f - (psIn.texcoord.y * 2.0f);
    ndc.z = 1.0f;
    ndc.w = 1.0f;
    
    float4 R = mul(ndc, invViewProjection);
    R /= R.w;
    const float roughness = 0;
    return sampleSkyBox(R.xyz, roughness);
}