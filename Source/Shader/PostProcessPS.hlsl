#include "PostProcess.hlsli"

SamplerState samplerState : register(s0);
Texture2D textureMap : register(t0);
float4 main(PSIn psIn) : SV_TARGET
{   
    float4 color = textureMap.Sample(samplerState, psIn.texcoord);
    
    // ビネット
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
    
#if 0 // ぼかし処理
    float w, h, levels;
    textureMap.GetDimensions(0, w, h, levels);
    float dx = 1.0f / w;
    float dy = 1.0f / h;
    float4 bokeColor = { 0, 0, 0, 1 };
    
    int pixcel = 2;
    int yokoPixcel = 2;
    
#if 0
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, -pixcel * dy));   // 左上
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, -pixcel * dy));              // 上
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, -pixcel * dy));    // 右上
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-yokoPixcel * dx, 0));          // 左
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord);                                        // 自分
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(yokoPixcel * dx, 0));           // 右
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, pixcel * dy));    // 左下
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, pixcel * dy));               // 下
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, pixcel * dy));     // 右下
#else  
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, -pixcel * dy))*  2;   // 左上
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, -pixcel * dy))           *  1;   // 上
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, -pixcel * dy)) *  0;   // 右上
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-yokoPixcel * dx, 0))       *  1;   // 左
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord);                                            // 自分
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(yokoPixcel * dx, 0))        * -1;   // 右
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(-pixcel * dx, pixcel * dy)) *  0;   // 左下
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(0, pixcel * dy))            * -1;   // 下
    bokeColor += textureMap.Sample(samplerState, psIn.texcoord + float2(pixcel * dx, pixcel * dy))  * -2;   // 右下
    
    bokeColor /= 9.0f;
    
    color.rgb = bokeColor.rgb;
#endif
#endif
    
    return color;
}