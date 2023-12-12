#include "TitleScene.h"

#include <wrl.h>

#include "../Input/Input.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Shader.h"
#include "../Other/Misc.h"

#include "GameScene.h"
#include "LoadingScene.h"
#include "SceneManager.h"

#include "../Resource/ModelResourceManager.h"

// ----- �R���X�g���N�^ -----
TitleScene::TitleScene()
{
    CreateRootSignature();

    //CreateGraphicsPipeline();
}

// ----- ���\�[�X���� -----
void TitleScene::CreateResource()
{
    backSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/White.png", "Back");
    titleLogoSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/Emma.png", "Emma");
    pressAnyButtonSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/PressAnyButton.png", "PressAnyButton");
    loadGameSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/LoadGame.png", "LoadGame");
    quitGameSprite_ = std::make_unique<Sprite>("./Resources/Image/Title/QuitGame.png", "QuitGame");

    //model_ = std::make_unique<Model>("./Resources/Model/Book.fbx");
    model_ = ModelResourceManager::Instance().LoadModelResource("./Resources/Model/sword.fbx", true);
    //model_ = std::make_unique<Model>("./Resources/Model/player.fbx");
    //model_ = std::make_unique<Model>("./Resources/Model/cube.fbx");
    //sprite_ = std::make_unique<Sprite>("./Resources/Image/Emma.png");
}

// ----- ������ -----
void TitleScene::Initialize()
{
    backSprite_->GetTransform()->SetColorBlack();

    pressAnyButtonSprite_->GetTransform()->SetPos(480, 450);
    pressAnyButtonSprite_->GetTransform()->SetSize(300, 50);
    pressAnyButtonSprite_->GetTransform()->SetColorA(1.0f);

    loadGameSprite_->GetTransform()->SetPos(475, 420);
    loadGameSprite_->GetTransform()->SetSize(300, 50);
    loadGameSprite_->GetTransform()->SetColorA(1.0f);

    quitGameSprite_->GetTransform()->SetPos(475, 480);
    quitGameSprite_->GetTransform()->SetSize(300, 50);
    quitGameSprite_->GetTransform()->SetColorA(1.0f);    

    Camera::Instance().Initialize();
}

// ----- �I���� -----
void TitleScene::Finalize()
{
}

// ----- �X�V -----
void TitleScene::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    backSprite_->Update(elapsedTime);
    titleLogoSprite_->Update(elapsedTime);
    pressAnyButtonSprite_->Update(elapsedTime);
    loadGameSprite_->Update(elapsedTime);
    quitGameSprite_->Update(elapsedTime);

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {        
        SceneManager::Instance().ChangeScene(new LoadingScene(new GameScene));
        return;
    }
}

// ----- �`�� -----
void TitleScene::Render(ID3D12GraphicsCommandList* commandList)
{
    Camera& camera = Camera::Instance();
    Graphics::SceneConstants sceneConstants = {};

    camera.SetPerSpectiveFov();
    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection_, camera.GetViewMatrix() * camera.GetProjectionMatrix());

    //commandList->SetPipelineState(pipelineState_.Get());
    //commandList->SetGraphicsRootSignature(rootSignature_.Get());

    // --- Model ---
    {
        Graphics::Instance().GetShader()->SetLambertShader(commandList);

        model_->Render(commandList);
    }

    //backSprite_->Render(commandList);
    //titleLogoSprite_->Render(commandList);
    //pressAnyButtonSprite_->Render(commandList);
    //loadGameSprite_->Render(commandList);
    //quitGameSprite_->Render(commandList);
}

// ----- ImGui�p -----
void TitleScene::DrawDebug()
{
    ImGui::Begin("TitleScene");
    
    model_->DrawDebug();

    Camera::Instance().DrawDebug();

    if (ImGui::BeginMenu("Sprite"))
    {
        backSprite_->DrawDebug();
        titleLogoSprite_->DrawDebug();
        pressAnyButtonSprite_->DrawDebug();
        loadGameSprite_->DrawDebug();
        quitGameSprite_->DrawDebug();

        ImGui::EndMenu();
    }

    ImGui::End();
}

// ----- ���[�g�V�O�l�`���̐��� -----
void TitleScene::CreateRootSignature()
{
    HRESULT result = S_OK;

    // ----- ���[�g�V�O�l�`���̐��� -----
#pragma region ���[�g�V�O�l�`���̐���
    // --- �f�B�X�N���v�^�����W�̐ݒ� ---
    D3D12_DESCRIPTOR_RANGE descriptorRange = {};
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // �����W���
    descriptorRange.NumDescriptors = 1;                             // �f�B�X�N���v�^��
    descriptorRange.BaseShaderRegister = 0;                         // �擪���W�X�^�ԍ�
    descriptorRange.RegisterSpace = 0;                              // ���܂����킹�邽�߂̃X�y�[�X
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // --- ���[�g�p�����[�^�̐ݒ� ---
    D3D12_ROOT_PARAMETER rootParameter = {};
    rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // �p�����[�^���
    rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // �ǂ̃V�F�[�_�[���痘�p�\��
    rootParameter.DescriptorTable.NumDescriptorRanges = 1;                      // �f�B�X�N���v�^�����W��
    rootParameter.DescriptorTable.pDescriptorRanges = &descriptorRange;         // �f�B�X�N���v�^�����W�̃A�h���X

    // --- �T���v���[�̐ݒ� ---
    D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = 0;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.ShaderRegister = 0;
    samplerDesc.RegisterSpace = 0;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    // --- ���[�g�V�O�l�`���̐ݒ� ---
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = 1;
    rootSignatureDesc.pParameters = &rootParameter;
    rootSignatureDesc.NumStaticSamplers = 1;
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // --- �V���A���C�Y ---
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

    result = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        signatureBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- ���[�g�V�O�l�`���𐶐� ---
    result = Graphics::Instance().GetDevice()->CreateRootSignature(
        0,
        signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

#pragma endregion
}

// ----- �O���t�B�b�N�X�p�C�v���C���쐬 -----
void TitleScene::CreateGraphicsPipeline()
{
    HRESULT result = S_OK;

    // --- ���[�g�V�O�l�`�� ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- �u�����h�X�e�[�g ---
    graphicsPipeline_.BlendState.AlphaToCoverageEnable = false;
    graphicsPipeline_.BlendState.IndependentBlendEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendEnable = true;
    graphicsPipeline_.BlendState.RenderTarget[0].LogicOpEnable = false;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    graphicsPipeline_.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    graphicsPipeline_.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    graphicsPipeline_.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // --- �[�x�X�e���V���X�e�[�g ---
    graphicsPipeline_.DepthStencilState.DepthEnable = true;
    graphicsPipeline_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    graphicsPipeline_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    graphicsPipeline_.DepthStencilState.StencilEnable = false;

    // --- ���X�^���C�U�[�X�e�[�g ---
    graphicsPipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    graphicsPipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    graphicsPipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    graphicsPipeline_.RasterizerState.FrontCounterClockwise = false;
    graphicsPipeline_.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    graphicsPipeline_.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    graphicsPipeline_.RasterizerState.DepthClipEnable = true;
    graphicsPipeline_.RasterizerState.MultisampleEnable = false;
    graphicsPipeline_.RasterizerState.AntialiasedLineEnable = false;
    graphicsPipeline_.RasterizerState.ForcedSampleCount = 0;
    graphicsPipeline_.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // --- �v���~�e�B�u�g�|���W�[ ---
    graphicsPipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // --- �X�g���b�v���̃J�b�g�l ---
    graphicsPipeline_.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    // --- �����_�[�^�[�Q�b�g�� ---
    graphicsPipeline_.NumRenderTargets = 1;
    graphicsPipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    graphicsPipeline_.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // --- �}���`�T���v�����O ---
    graphicsPipeline_.SampleDesc.Count = 1;
    graphicsPipeline_.SampleDesc.Quality = 0;

    // --- �A�_�v�^ ---
    graphicsPipeline_.NodeMask = 0;

    result = Graphics::Instance().GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipeline_,
        IID_PPV_ARGS(&pipelineState_)
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
}
