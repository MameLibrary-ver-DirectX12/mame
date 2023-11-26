#include "BasicShader.hlsli"

cbuffer constantBuffer : register(b0)
{
    matrix mat; // •ÏŠ·s—ñ
}

Output main(float4 pos : POSITION, float2 texcoord : TEXCOORD)
{
    Output output;
    output.pos = mul(mat, pos);
    output.texcoord = texcoord;
    return output;
}