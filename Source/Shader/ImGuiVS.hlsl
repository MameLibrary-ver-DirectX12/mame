#include "ImGui.hlsli"

PSIn main(float4 position : POSITION, float2 texcoord : TEXCOORD, float4 color : COLOR)
{
    PSIn vsOut;
    vsOut.position = mul(position, wvp);
    vsOut.texcoord = texcoord;
    vsOut.color = color;

    return vsOut;
}