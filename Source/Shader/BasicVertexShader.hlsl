#include "BasicShader.hlsli"

struct VSIn
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    uint2 boneNo : BONE_NO;
    uint weight : WEIGHT;
    uint edgeFlg : EDGE_FLG;
};

cbuffer constantBuffer : register(b0)
{
    matrix world;           // ワールド行列
    matrix viewProjection;  // ビュープロジェクション行列
}

Output main(VSIn vsIn)
{
    Output output;
    output.pos = mul(mul(viewProjection, world), vsIn.pos);
    output.normal.w = 0;                        // 平行移動成分を無効にする
    output.normal = mul(world, vsIn.normal);    // 法線にもワールド変換を行う
    output.texcoord = vsIn.texcoord;
    return output;
}