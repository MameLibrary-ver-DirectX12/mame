#include "Shader.h"

#include "Graphics.h"

// --- �R���X�g���N�^ ---
Shader::Shader()
{
    lambertShader_ = std::make_unique<LambertShader>();
}
