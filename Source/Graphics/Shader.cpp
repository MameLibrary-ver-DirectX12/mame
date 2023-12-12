#include "Shader.h"

#include "Graphics.h"

// --- コンストラクタ ---
Shader::Shader()
{
    lambertShader_ = std::make_unique<LambertShader>();
}
