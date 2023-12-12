struct VSIn
{
    float4 position     : POSITION;
    float4 normal       : NORMAL;
    float4 tangent      : TANGENT;
    float2 texcoord     : TEXCOORD;
    float4 boneWeights  : WEIGHTS;
    uint4 boneIndices   : BONES;
};

struct PSIn
{
    float4 position         : SV_POSITION;
    float4 worldPosition    : POSITION;
    float4 worldNormal      : NORMAL;
    float4 worldTangent     : TANGENT;
    float2 texcoord         : TEXCOORD;
    float4 color            : COLOR;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
    row_major float4x4 lightViewProjection;
};

static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4  worldMatrix;
    float4              materialColor;
    row_major float4x4  boneTransforms[MAX_BONES];
    
    // EMISSIVE
    float4  emissiveColor;
    float   emissiveIntensity;
    float3  emissiveDummy;
};