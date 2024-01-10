#pragma once

#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "../Graphics/Descriptor.h"
#include "../Graphics/ConstantBuffer.h"

class Sprite
{
public:
    Sprite(const char* filename = nullptr, const char* imguiName = "Sprite",
        const char* emissiveFilename = nullptr);
    ~Sprite();

    void Initialize();
    void Update(const float& elapsedTime);
    void Render(ID3D12GraphicsCommandList* commandList, 
        const int& noiseTextureType = 0/*NoiseTexture Class �œǂݍ���ł�e�N�X�`���w��*/);
    void DrawDebug();

    void SetChargeValue(const float& value) { charge_.value_ = value; }

public:
    // ----- �s�N�Z���V�F�[�_�[�؂�ւ��p�֐� -----
    void UseDefaultPixelShader();
    void UseDissolvePixelShader();
    void UseEmissivePixelShader();    
    void UseChargePixelShader();

private:// Transform
#pragma region Transform
    struct VibrationTransform
    {
    private:
        DirectX::XMFLOAT2 oldVibration_ = {};   // �O��̐U���ʕۑ��p
        float volume_ = 1.0f;                   // �U�����x
        float breakTime_ = 0.1f;                // �U���ƐU���̊Ԃ̎���
        float vibrationBreakTimer_ = 0.0f;      // �����x�e�̃^�C�}�[

    public:
        // ----- ImGui�p -----
        void DrawDebug();

        // ----- �O��̐U���� -----
#pragma region �O��̐U����
        DirectX::XMFLOAT2 GetOldVibration() { return oldVibration_; }
        void SetOldVibration(DirectX::XMFLOAT2 oldVibration) { oldVibration_ = oldVibration; }

#pragma endregion

        // ----- �U�����x -----
#pragma region �U�����x
        float GetVolume() { return volume_; }
        void SetVolume(float volume) { volume_ = volume; }

#pragma endregion

        // ----- �U���ƐU���̊Ԃ̎��� -----
#pragma region �U���ƐU���̊Ԃ̎���
        float GetBreakTime() { return breakTime_; }
        void SetBreakTime(float breakTime) { breakTime_ = breakTime; }

#pragma endregion

        // ----- �����x�e�̃^�C�}�[ -----
#pragma region �����x�e�̃^�C�}�[
        float GetVibrationBreakTimer() { return vibrationBreakTimer_; }
        void SetVibrationBreakTimer(float timer) { vibrationBreakTimer_ = timer; }
        void SubtractVibrationBreakTimer(float time) { vibrationBreakTimer_ -= time; }

#pragma endregion
    };


    struct Transform
    {
    private:
        DirectX::XMFLOAT2 position_ = { 0, 0 };         // �ʒu
        DirectX::XMFLOAT2 size_     = { 100, 100 };     // �T�C�Y
        DirectX::XMFLOAT4 color_    = { 1, 1, 1, 1 };   // �F
        float             angle_    = 0.0f;             // �p�x
        DirectX::XMFLOAT2 texPos_   = { 0, 0 };         // �e�N�X�`�����W
        DirectX::XMFLOAT2 texSize_  = { size_ };        // �e�N�X�`���T�C�Y

        DirectX::XMFLOAT2 originalSize_ = { 0, 0 };     // �e�N�X�`���̃I���W�i���T�C�Y

        // ----- �U�� -----
        VibrationTransform vibration_;

        // ----- ImGui�p -----
        bool isSizeFactor_ = false;

    public:
        // ----- ImGui�p -----
        void DrawDebug();

        // ----- �ʒu -----
#pragma region �ʒu ( position )
        // --- Get ---
        DirectX::XMFLOAT2 GetPos() { return position_; }
        float GetPosX() { return position_.x; }
        float GetPosY() { return position_.y; }

        // --- Set ---
        void SetPos(const DirectX::XMFLOAT2& pos) { position_ = pos; }
        void SetPos(const float& x, const float& y) { position_ = { x, y }; }
        void SetPosX(const float& posX) { position_.x = posX; }
        void SetPosY(const float& posY) { position_.y = posY; }

        // --- Add ---
        void AddPos(const DirectX::XMFLOAT2& pos)
        {
            position_.x += pos.x;
            position_.y += pos.y;
        }
        void AddPos(const float& posX, const float& posY)
        {
            position_.x += posX;
            position_.y += posY;
        }
        void AddPosX(const float& posX) { position_.x += posX; }
        void AddPosY(const float& posY) { position_.y += posY; }

        // --- Subtract ---
        void SubtractPos(const DirectX::XMFLOAT2& pos)
        {
            position_.x -= pos.x;
            position_.y -= pos.y;
        }
        void SubtractPos(const float& posX, const float& posY)
        {
            position_.x -= posX;
            position_.y -= posY;
        }
        void SubtractPosX(const float& posX) { position_.x -= posX; }
        void SubtractPosY(const float& posY) { position_.y -= posY; }


        // �����̈ʒu���摜�̒��S�Ƃ��āA�摜�̈ʒu��ݒ肷��
        void SetSpriteCenterPos(DirectX::XMFLOAT2 pos)
        {
            pos.x -= GetSizeX() / 2;
            pos.y -= GetSizeY() / 2;
            position_ = pos;
        }
        void SetSpriteCenterPos(float x, float y)
        {
            x -= GetSizeX() / 2;
            y -= GetSizeY() / 2;
            position_ = { x, y };
        }

        // �����̈ʒu���摜�̍���Ƃ����Ƃ��́A�摜�̈ʒu��ݒ肷��
        // ���ʏ�̐ݒ�̊��o�Ŏg�������ꍇ���������g�����Ƃ������߂���
        void SetSpriteCenterPos(float x, float y, float size)
        {
            x += size / 2;
            y += size / 2;
            x -= GetSizeX() / 2;
            y -= GetSizeY() / 2;
            position_ = { x, y };
        }

#pragma endregion

        // ----- �傫�� -----
#pragma region �傫�� ( size )
        // --- Get ---
        DirectX::XMFLOAT2 GetSize() { return size_; }
        float GetSizeX() { return size_.x; }
        float GetSizeY() { return size_.y; }

        // --- Set ---
        void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
        void SetSize(const float& x, const float& y)
        {
            size_.x = x;
            size_.y = y;
        }
        void SetSizeX(const float& sizeX) { size_.x = sizeX; }
        void SetSizeY(const float& sizeY) { size_.y = sizeY; }
        void SetSize(const float& size) { size_ = { size,size }; }

#pragma endregion

        // ----- �F -----
#pragma region �F ( color )
        // --- Get ---
        DirectX::XMFLOAT4 GetColor() { return color_; }
        float GetColorR() { return color_.x; }
        float GetColorG() { return color_.y; }
        float GetColorB() { return color_.z; }
        float GetColorA() { return color_.w; }

        // --- Set ---
        void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; }
        void SetColor(const float& r, const float& g, const float& b, const float& a)
        {
            color_.x = r;
            color_.y = g;
            color_.z = b;
            color_.w = a;
        }
        void SetColor(const float& r, const float& g, const float& b)
        {
            color_.x = r;
            color_.y = g;
            color_.z = b;
        }
        void SetColorR(const float& r) { color_.x = r; }
        void SetColorG(const float& g) { color_.y = g; }
        void SetColorB(const float& b) { color_.z = b; }
        void SetColorA(const float& a) { color_.w = a; }
        void SetColorWhite() { color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, color_.w); }
        void SetColorBlack() { color_ = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, color_.w); }

#pragma endregion

        // ----- �p�x -----
#pragma region �p�x ( angle )
        // --- Get ---
        float GetAngle() { return angle_; }

        // --- Set ---
        void SetAngle(const float& angle) { angle_ = angle; }

        // --- Add ---
        void AddAngle(const float& angle) { angle_ += angle; }

#pragma endregion

        // ----- �e�N�X�`�����W -----
#pragma region �e�N�X�`�����W ( texPos )
        // --- Get ---
        DirectX::XMFLOAT2 GetTexPos() { return texPos_; }
        float GetTexPosX() { return texPos_.x; }
        float GetTexPosY() { return texPos_.y; }

        // --- Set ---
        void SetTexPos(const DirectX::XMFLOAT2& texPos) { texPos_ = texPos; }
        void SetTexPos(const float& texPosX, const float& texPosY)
        {
            texPos_.x = texPosX;
            texPos_.y = texPosY;
        }
        void SetTexPosX(const float& texPosX) { texPos_.x = texPosX; }
        void SetTexPosY(const float& texPosY) { texPos_.y = texPosY; }
#pragma endregion

        // ----- �e�N�X�`���T�C�Y -----
#pragma region �e�N�X�`���T�C�Y ( texSize )
        // --- Get ---
        DirectX::XMFLOAT2 GetTexSize() { return texSize_; }
        float GetTexSizeX() { return texSize_.x; }
        float GetTexSizeY() { return texSize_.y; }

        // --- Set ---
        void SetTexSize(const DirectX::XMFLOAT2& texSize) { texSize_ = texSize; }
        void SetTexSize(const float& texSizeX, const float& texSizeY)
        {
            texSize_.x = texSizeX;
            texSize_.y = texSizeY;
        }
        void SetTexSizeX(const float& texSizeX) { texSize_.x = texSizeX; }
        void SetTexSizeY(const float& texSizeY) { texSize_.y = texSizeY; }

#pragma endregion

        // ----- �I���W�i���T�C�Y -----
        DirectX::XMFLOAT2 GetOriginalSize() { return originalSize_; }
        float GetOriginalSizeX() { return originalSize_.x; }
        float GetOriginalSizeY() { return originalSize_.y; }
        void SetOriginalSize(DirectX::XMFLOAT2 size) { originalSize_ = size; }


        // ----- �U�� -----
#pragma region �U��
        // --- Get ---
        VibrationTransform* GetVibration() { return &vibration_; }
        
        void Vibration(const float& elapsedTime);

#pragma endregion
    };


    struct Dissolve
    {
    private:
        float dissolveValue_ = 0;     // �f�B�]���u�K����

        float edgeThrehold_ = 0.1f; // ����臒l

        DirectX::XMFLOAT2 dummy_ = {};

        DirectX::XMFLOAT4 edgeColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };  // ���̐F

    public:
        // ----- ImGui�p -----
        void DrawDebug();

        float GetDissolveValue() { return dissolveValue_; }
        float GetEdgeThrehold() { return edgeThrehold_; }
        DirectX::XMFLOAT4 GetEdgeColor() { return edgeColor_; }
    };

#pragma endregion Transform

public:// �擾�E�ݒ�
#pragma region [Get, Set] Function
    // --- Transform ---
    Transform* GetTransform() { return &transform_; }
    VibrationTransform* GetVibration() { return transform_.GetVibration(); }


#pragma endregion


private:// �ϐ�
    // ---------- Transform -------------------
    Transform transform_;
    Dissolve dissolve_;

    // ---------- ImGui�p ---------------------
    bool isVibration_ = false;  // �U��
    bool isDissolve_  = false;  // dissolve

    // ---------- Shader ----------------------
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob_;
    Microsoft::WRL::ComPtr<ID3DBlob> psDissolveBlob_;
    Microsoft::WRL::ComPtr<ID3DBlob> psEmissiveBlob_;
    Microsoft::WRL::ComPtr<ID3DBlob> psChargeBlob_;

    // ---------- �\���Ɋւ��邠�ꂱ�� ----------
#pragma region �\���Ɋւ��邠�ꂱ��
    struct Vertex
    {
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT4 color_;
        DirectX::XMFLOAT2 texcoord_;
    };

    struct DissolveConstants
    {
        float dissolveValue_ = 0;     // �f�B�]���u�K����
        float edgeThrehold_ = 0.1f; // ����臒l
        DirectX::XMFLOAT2 dummy_;

        DirectX::XMFLOAT4 edgeColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };  // ���̐F        
    };

    struct FrameResource
    {
        Microsoft::WRL::ComPtr<ID3D12Resource>  vertexBuffer_;
        Microsoft::WRL::ComPtr<ID3D12Resource>  indexBuffer_;
        D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_   = {};
        D3D12_INDEX_BUFFER_VIEW                 indexBufferView_    = {};
        Vertex*                                 vertexData_         = nullptr;
    };

    std::vector<FrameResource> frameResource_;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
    Microsoft::WRL::ComPtr<ID3D12Resource> texture_;

    Descriptor* shaderResourceViewDescriptor_ = nullptr;

    // --- emissive�p ---
    Microsoft::WRL::ComPtr<ID3D12Resource> emissiveTexture_;
    Descriptor* emissiceShaderResourceViewDescriptor_ = nullptr;
    
    // --- dissolve�萔�p ---
    Descriptor* dissolveConstantsDescriptor_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> dissolveConstantBuffer_;
    
    // ���ō���Ă�
    std::unique_ptr<ConstantBuffer> chargeConstants_;
    struct ChargeConstant
    {
        float value_ = 0;
        float dummy_[3] = {};
    }charge_;


    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeline_ = {};

#pragma endregion

    // ---------- ImGui -----------------------
#pragma region ImGui
public:// ----- ImGui�p ----- //
    static int nameNum;
    const char* GetName() { return name_.c_str(); }
    void SetName(const char* n) { name_ = n; }
    
private:// ----- ImGui�p ----- //
    std::string name_;
    static const int MAX_PIXEL_SHADER_NUM = 3;

    std::string list[MAX_PIXEL_SHADER_NUM] =
    {
        reinterpret_cast<const char*>(u8"�x�[�X"),
        reinterpret_cast<const char*>(u8"�G�~�b�V�u"),
        reinterpret_cast<const char*>(u8"�f�B�]���u"),
    };

    // ----- ImGui�Ŏg���֐��|�C���^ -----
    void (Sprite::* usePSFunc[MAX_PIXEL_SHADER_NUM])() =
    {
        &Sprite::UseDefaultPixelShader,
        &Sprite::UseEmissivePixelShader,
        &Sprite::UseDissolvePixelShader,
    };

    const char* currentItem = nullptr;

#pragma endregion
};

