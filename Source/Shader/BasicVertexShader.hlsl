#include "BasicShader.hlsli"

Output main(float4 pos : POSITION, float2 texcoord : TEXCOORD)
{
    Output output;
    output.pos = pos;
    output.texcoord = texcoord;
    return output;
}