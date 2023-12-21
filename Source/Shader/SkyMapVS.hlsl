#include "SkyMap.hlsli"

PSIn main(in uint vertexID : SV_VERTEXID)
{
    PSIn vsOut;
    const float2 position[4] = { { -1, +1 }, { +1, +1 }, { -1, -1 }, { +1, -1 } };
    const float2 texcoord[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    
    vsOut.position = float4(position[vertexID], 0, 1);
    vsOut.texcoord = texcoord[vertexID];
    return vsOut;
}