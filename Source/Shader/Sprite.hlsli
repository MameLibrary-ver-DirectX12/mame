// register ( texture ) ‚É‚Â‚¢‚Ä
// 0  : baseColor
// 1  : emissive
// 2~ : noise

struct PSIn
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer Dissolve : register(b0)
{
    float dissolveValue;
    float edgeThrehold;
    float2 dummy;
    float4 edgeColor;
};