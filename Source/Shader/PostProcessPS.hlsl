#include "PostProcess.hlsli"

SamplerState samplerState : register(s0);
Texture2D textureMap : register(t0);
float4 main(PSIn psIn) : SV_TARGET
{   
    float4 color = textureMap.Sample(samplerState, psIn.texcoord);
    
    // �r�l�b�g
    {
#if 0
        float2 samplePoint = psIn.texcoord;
        float vignette = length(float2(0.5, 0.5) - psIn.texcoord);
        //vignette = clamp(vignette - vignetteValue, 0, 1);
        vignette = clamp(vignette - 0.7, 0, 1);
        color.rgb -= vignette;
        //color.r += vignette;
#endif
    }
    
#if 0 // �ڂ�������
    float w, h, levels;
    textureMap.GetDimensions(0, w, h, levels);
    float dx = 1.0f / w;
    float dy = 1.0f / h;
    float4 bokeColor = { 0, 0, 0, 1 };
    
    int pixcel = 2;
    int yokoPixcel = 2;
    
#if 0
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, -pixcel * dy));   // ����
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, -pixcel * dy));              // ��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, -pixcel * dy));    // �E��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-yokoPixcel * dx, 0));          // ��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord);                                        // ����
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(yokoPixcel * dx, 0));           // �E
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, pixcel * dy));    // ����
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, pixcel * dy));               // ��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, pixcel * dy));     // �E��
#else  
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, -pixcel * dy))*  2;   // ����
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, -pixcel * dy))           *  1;   // ��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, -pixcel * dy)) *  0;   // �E��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-yokoPixcel * dx, 0))       *  1;   // ��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord);                                            // ����
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(yokoPixcel * dx, 0))        * -1;   // �E
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, pixcel * dy)) *  0;   // ����
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, pixcel * dy))            * -1;   // ��
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, pixcel * dy))  * -2;   // �E��
    
    bokeColor /= 9.0f;
    
    color.rgb = bokeColor.rgb;
#endif
#endif
    
    return color;
}