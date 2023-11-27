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
    matrix world;           // ���[���h�s��
    matrix viewProjection;  // �r���[�v���W�F�N�V�����s��
}

Output main(VSIn vsIn)
{
    Output output;
    output.pos = mul(mul(viewProjection, world), vsIn.pos);
    output.normal.w = 0;                        // ���s�ړ������𖳌��ɂ���
    output.normal = mul(world, vsIn.normal);    // �@���ɂ����[���h�ϊ����s��
    output.texcoord = vsIn.texcoord;
    return output;
}