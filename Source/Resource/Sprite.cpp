#include "Sprite.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>

#include "../Graphics/Graphics.h"

#include "../Other/Misc.h"

#include "../Graphics/NoiseTexture.h"


//---ImGui���O���Ԃ�h�~�p---//
int Sprite::nameNum = 0;
//---ImGui���O���Ԃ�h�~�p---//

// ----- �R���X�g���N�^ -----
Sprite::Sprite(const char* filename, const char* imguiName, const char* emissiveFilename)
{
    HRESULT result = { S_OK };
    Graphics& graphics = Graphics::Instance();

    // --- ���[�g�V�O�l�`���̐��� ---
#pragma region ���[�g�V�O�l�`���̐���
    const int MAX_DATA_NUM = 5;

    // --- �f�B�X�N���v�^�����W�̐ݒ� ---
    D3D12_DESCRIPTOR_RANGE descriptorRange[MAX_DATA_NUM] = {};
    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // �����W���
    descriptorRange[0].NumDescriptors = 1;                             // �f�B�X�N���v�^��
    descriptorRange[0].BaseShaderRegister = 0;                         // �擪���W�X�^�ԍ�
    descriptorRange[0].RegisterSpace = 0;                              // ���܂����킹�邽�߂̃X�y�[�X
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // �����W���
    descriptorRange[1].NumDescriptors = 1;                             // �f�B�X�N���v�^��
    descriptorRange[1].BaseShaderRegister = 1;                         // �擪���W�X�^�ԍ�
    descriptorRange[1].RegisterSpace = 0;                              // ���܂����킹�邽�߂̃X�y�[�X
    descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // �����W���
    descriptorRange[2].NumDescriptors = 1;                             // �f�B�X�N���v�^��
    descriptorRange[2].BaseShaderRegister = 2;                         // �擪���W�X�^�ԍ�
    descriptorRange[2].RegisterSpace = 0;                              // ���܂����킹�邽�߂̃X�y�[�X
    descriptorRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // �����W���
    descriptorRange[3].NumDescriptors = 1;                             // �f�B�X�N���v�^��
    descriptorRange[3].BaseShaderRegister = 0;                         // �擪���W�X�^�ԍ�
    descriptorRange[3].RegisterSpace = 0;                              // ���܂����킹�邽�߂̃X�y�[�X
    descriptorRange[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;    // �����W���
    descriptorRange[4].NumDescriptors = 1;                             // �f�B�X�N���v�^��
    descriptorRange[4].BaseShaderRegister = 3;                         // �擪���W�X�^�ԍ�
    descriptorRange[4].RegisterSpace = 0;                              // ���܂����킹�邽�߂̃X�y�[�X
    descriptorRange[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // --- ���[�g�p�����[�^�̐ݒ� ---
    D3D12_ROOT_PARAMETER rootParameter[MAX_DATA_NUM] = {};
    rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // �p�����[�^���
    rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // �ǂ̃V�F�[�_�[���痘�p�\��
    rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;                      // �f�B�X�N���v�^�����W��
    rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];      // �f�B�X�N���v�^�����W�̃A�h���X

    rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // �p�����[�^���
    rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // �ǂ̃V�F�[�_�[���痘�p�\��
    rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;                      // �f�B�X�N���v�^�����W��
    rootParameter[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];      // �f�B�X�N���v�^�����W�̃A�h���X

    rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // �p�����[�^���
    rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // �ǂ̃V�F�[�_�[���痘�p�\��
    rootParameter[2].DescriptorTable.NumDescriptorRanges = 1;                      // �f�B�X�N���v�^�����W��
    rootParameter[2].DescriptorTable.pDescriptorRanges = &descriptorRange[2];      // �f�B�X�N���v�^�����W�̃A�h���X

    rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // �p�����[�^���
    rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // �ǂ̃V�F�[�_�[���痘�p�\��
    rootParameter[3].DescriptorTable.NumDescriptorRanges = 1;                      // �f�B�X�N���v�^�����W��
    rootParameter[3].DescriptorTable.pDescriptorRanges = &descriptorRange[3];      // �f�B�X�N���v�^�����W�̃A�h���X

    rootParameter[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // �p�����[�^���
    rootParameter[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             // �ǂ̃V�F�[�_�[���痘�p�\��
    rootParameter[4].DescriptorTable.NumDescriptorRanges = 1;                      // �f�B�X�N���v�^�����W��
    rootParameter[4].DescriptorTable.pDescriptorRanges = &descriptorRange[4];      // �f�B�X�N���v�^�����W�̃A�h���X

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
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // --- ���[�g�V�O�l�`���̐ݒ� ---
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = MAX_DATA_NUM;
    rootSignatureDesc.pParameters = rootParameter;
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
    result = graphics.GetDevice()->CreateRootSignature(
        0,
        signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(rootSignature_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    rootSignature_->SetName(L"SpriteRootSignature");

#pragma endregion ���[�g�V�O�l�`���̐���
    

    // --- �V�F�[�_�[�I�u�W�F�N�g�쐬 ---
#pragma region �V�F�[�_�[�I�u�W�F�N�g�쐬
    
    // ----- VertexShader -----
    result = D3DCompileFromFile(L"./Source/Shader/SpriteVS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &vsBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // ----- PixelShader -----

    // --- Default ---
    result = D3DCompileFromFile(L"./Source/Shader/SpritePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- Dissolve ---
    result = D3DCompileFromFile(L"./Source/Shader/SpriteDissolvePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psDissolveBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- Emissive ---
    result = D3DCompileFromFile(L"./Source/Shader/SpriteEmissivePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psEmissiveBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // --- Charge ---
    result = D3DCompileFromFile(L"./Source/Shader/SpriteChargePS.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &psChargeBlob_, &errorBlob);

    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    

#pragma endregion �V�F�[�_�[�I�u�W�F�N�g�쐬

    // --- �p�C�v���C���X�e�[�g�̐��� ---
#pragma region �p�C�v���C���X�e�[�g�̐���

    UseDefaultPixelShader();

    pipelineState_->SetName(L"SpriteGraphicsPipelineState");

#pragma endregion �p�C�v���C���X�e�[�g�̐���


    // --- �e�N�X�`���̐��� ---
#pragma region �e�N�X�`���̐���
    // --- BaseColor ---
#pragma region baseColor
    if (filename != nullptr)
    {
        // --- �e�N�X�`���̓ǂݍ��� ---
        result = Graphics::Instance().LoadTexture(filename, texture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }
    else
    {
        // --- �_�~�[�e�N�X�`���쐬 ---
        result = Graphics::Instance().CreateDummyTexture(texture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }

    // --- �f�B�X�N���v�^�̎擾 ---
    shaderResourceViewDescriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    // --- �V�F�[�_�[���\�[�X�r���[�̐��� ---
    D3D12_RESOURCE_DESC resourceDesc = texture_->GetDesc();

    // --- �V�F�[�_�[���\�[�X�r���[�̐ݒ� ---
    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.Format = resourceDesc.Format;
    shaderResourceViewDesc.Texture2D.MipLevels = resourceDesc.MipLevels;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

    // --- �V�F�[�_�[���\�[�X�r���[�𐶐� ---
    graphics.GetDevice()->CreateShaderResourceView(
        texture_.Get(),
        &shaderResourceViewDesc,
        shaderResourceViewDescriptor_->GetCpuHandle()
    );

    // --- �e�N�X�`�����̎擾 ---
    transform_.SetSize(static_cast<float>(resourceDesc.Width), static_cast<float>(resourceDesc.Height));
    transform_.SetTexSize(static_cast<float>(resourceDesc.Width), static_cast<float>(resourceDesc.Height));
    transform_.SetOriginalSize({ static_cast<float>(resourceDesc.Width), static_cast<float>(resourceDesc.Height) });

#pragma endregion

    // --- Emissive ---
#pragma region Emissive
    if (emissiveFilename != nullptr)
    {
        // --- �e�N�X�`���̓ǂݍ��� ---
        result = Graphics::Instance().LoadTexture(emissiveFilename, emissiveTexture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }
    else
    {
        // --- �_�~�[�e�N�X�`���쐬 ---
        result = Graphics::Instance().CreateDummyTexture(emissiveTexture_.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));
    }

    // --- �f�B�X�N���v�^�̎擾 ---
    emissiceShaderResourceViewDescriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    // --- �V�F�[�_�[���\�[�X�r���[�̐��� ---
    resourceDesc = emissiveTexture_->GetDesc();

    // --- �V�F�[�_�[���\�[�X�r���[�̐ݒ� ---
    shaderResourceViewDesc = {};
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.Format = resourceDesc.Format;
    shaderResourceViewDesc.Texture2D.MipLevels = resourceDesc.MipLevels;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

    // --- �V�F�[�_�[���\�[�X�r���[�𐶐� ---
    graphics.GetDevice()->CreateShaderResourceView(
        emissiveTexture_.Get(),
        &shaderResourceViewDesc,
        emissiceShaderResourceViewDescriptor_->GetCpuHandle()
    );

#pragma endregion

    // ----- �萔�o�b�t�@�[ -----
#pragma region �萔�o�b�t�@�[

    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC constantResourceDesc = {};
    constantResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    constantResourceDesc.Alignment = 0;
    constantResourceDesc.Width = (sizeof(Dissolve) + 255) & ~255;
    constantResourceDesc.Height = 1;
    constantResourceDesc.DepthOrArraySize = 1;
    constantResourceDesc.MipLevels = 1;
    constantResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    constantResourceDesc.SampleDesc.Count = 1;
    constantResourceDesc.SampleDesc.Quality = 0;
    constantResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    constantResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    result = graphics.GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &constantResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(dissolveConstantBuffer_.GetAddressOf())
    );
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    dissolveConstantsDescriptor_ = Graphics::Instance().GetSrvCbvUavDescriptorHeap()->PopDescriptor();

    D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
    constantBufferViewDesc.BufferLocation = dissolveConstantBuffer_->GetGPUVirtualAddress();
    constantBufferViewDesc.SizeInBytes = static_cast<UINT>(dissolveConstantBuffer_->GetDesc().Width);

    graphics.GetDevice()->CreateConstantBufferView(
        &constantBufferViewDesc,
        dissolveConstantsDescriptor_->GetCpuHandle()
    );

#pragma endregion


#pragma endregion �e�N�X�`���̐���


    // --- �t���[�����\�[�X ---
#pragma region �t���[�����\�[�X
    frameResource_.resize(graphics.GetBufferCount());
    for (FrameResource& frameResource : frameResource_)
    {
        // --- ���_�o�b�t�@�[���� ---
#pragma region ���_�o�b�t�@�[����
        // --- �q�[�v�v���p�e�B�̐ݒ� ---
        D3D12_HEAP_PROPERTIES vertexBufferHeapProp = {};
        vertexBufferHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
        vertexBufferHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        vertexBufferHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        vertexBufferHeapProp.CreationNodeMask = 1;
        vertexBufferHeapProp.VisibleNodeMask = 1;

        // --- ���\�[�X�̐ݒ� ---
        D3D12_RESOURCE_DESC vertexBufferResourceDesc = {};
        vertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vertexBufferResourceDesc.Alignment = 0;
        vertexBufferResourceDesc.Width = sizeof(Vertex) * 4;
        vertexBufferResourceDesc.Height = 1;
        vertexBufferResourceDesc.DepthOrArraySize = 1;
        vertexBufferResourceDesc.MipLevels = 1;
        vertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        vertexBufferResourceDesc.SampleDesc.Count = 1;
        vertexBufferResourceDesc.SampleDesc.Quality = 0;
        vertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        vertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        // --- ���\�[�X���� ---

        result = graphics.GetDevice()->CreateCommittedResource(
            &vertexBufferHeapProp,
            D3D12_HEAP_FLAG_NONE,
            &vertexBufferResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(frameResource.vertexBuffer_.GetAddressOf())
        );
        result = graphics.GetDevice()->GetDeviceRemovedReason();
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.vertexBuffer_->SetName(L"SpriteVertexBuffer");

        // --- ���_�o�b�t�@�[�r���[ ---
        frameResource.vertexBufferView_.BufferLocation = frameResource.vertexBuffer_->GetGPUVirtualAddress();
        frameResource.vertexBufferView_.SizeInBytes = static_cast<UINT>(vertexBufferResourceDesc.Width);
        frameResource.vertexBufferView_.StrideInBytes = sizeof(Vertex);

        // --- �}�b�v ---
        frameResource.vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&frameResource.vertexData_));

#pragma endregion


        // --- �C���f�b�N�X�o�b�t�@�[�̐��� ---
#pragma region �C���f�b�N�X�o�b�t�@�[�̐���
        // --- �q�[�v�v���p�e�B�̐ݒ� ---
        vertexBufferHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        // --- ���\�[�X�̐ݒ� ---
        vertexBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT) * 6);

        // --- ���\�[�X���� ---
        result = graphics.GetDevice()->CreateCommittedResource(
            &vertexBufferHeapProp,
            D3D12_HEAP_FLAG_NONE,
            &vertexBufferResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(frameResource.indexBuffer_.GetAddressOf())
        );
        _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

        frameResource.indexBuffer_->SetName(L"SpriteIndexBuffer");

        // --- �C���f�b�N�X�o�b�t�@�[�r���[�ݒ� ---
        frameResource.indexBufferView_.BufferLocation = frameResource.indexBuffer_->GetGPUVirtualAddress();
        frameResource.indexBufferView_.SizeInBytes = static_cast<UINT>(vertexBufferResourceDesc.Width);
        frameResource.indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

        // --- �C���f�b�N�X�f�[�^�ݒ� ---
        UINT* index;
        frameResource.indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&index));

        // --- �l�p�`���O�p�`�Q�ɓW�J
        // 0---1      0---1  4
        // |   |  ��  |�^  �^|
        // 2---3      2  3---5
        index[0] =  0;
        index[1] =  1;
        index[2] =  2;
        index[3] =  2;
        index[4] =  1;
        index[5] =  3;

        frameResource.indexBuffer_->Unmap(0, nullptr);

#pragma endregion

    }

#pragma endregion �t���[�����\�[�X


    chargeConstants_ = std::make_unique<ConstantBuffer>(sizeof(ChargeConstant));

    //---ImGui���O���Ԃ�h�~�p---//
    std::string space = " ";
    std::string name = imguiName + space + std::to_string(nameNum++);
    SetName(name.c_str());
    //---ImGui���O���Ԃ�h�~�p---//
}

// ----- �f�X�g���N�^ -----
Sprite::~Sprite()
{
    for (FrameResource& frameResource : frameResource_)
    {
        if (frameResource.vertexData_ != nullptr)
        {
            frameResource.vertexBuffer_->Unmap(0, nullptr);
        }
    }

    --nameNum;
}

// ----- ������ -----
void Sprite::Initialize()
{
}

// ----- �X�V -----
void Sprite::Update(const float& elapsedTime)
{
    if (isVibration_) GetTransform()->Vibration(elapsedTime);

    if (isDissolve_)
    {
        
    }
}

// ----- �`�� -----
void Sprite::Render(ID3D12GraphicsCommandList* commandList, const int& noiseTextureType)
{
    Graphics& graphics = Graphics::Instance();

    // --- �X�N���[���T�C�Y���擾���� ---
    float screenWidth = graphics.GetScreenWidth();
    float screenHeight = graphics.GetScrennHeight();

    // --- �X�v���C�g���\������S���_�̃X�N���[�����W���v�Z���� ---
    DirectX::XMFLOAT2 positions[] =
    {
        // ����
        { transform_.GetPosX(), transform_.GetPosY() },
        // �E��
        { transform_.GetPosX() + transform_.GetSizeX(), transform_.GetPosY() },
        // ����
        { transform_.GetPosX(), transform_.GetPosY() + transform_.GetSizeY() },
        // �E��
        { transform_.GetPosX() + transform_.GetSizeX(), transform_.GetPosY() + transform_.GetSizeY() },
    };

    // --- �X�v���C�g���\������S���_�̃e�N�X�`�����W���v�Z���� ---
    DirectX::XMFLOAT2 texcoords[] =
    {
        // ����
        { transform_.GetTexPosX(), transform_.GetTexPosY() },
        // �E��
        { transform_.GetTexPosX() + transform_.GetTexSizeX(), transform_.GetTexPosY() },
        // ����
        { transform_.GetTexPosX(), transform_.GetTexPosY() + transform_.GetTexSizeY() },
        // �E��
        { transform_.GetTexPosX() + transform_.GetTexSizeX(), transform_.GetTexPosY() + transform_.GetTexSizeY() },
    };

    // --- �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu�����_�ɂȂ�悤�Ɉ�U���_���ړ������� ---
    float mx = transform_.GetPosX() + transform_.GetSizeX() * 0.5f;
    float my = transform_.GetPosY() + transform_.GetSizeY() * 0.5f;
    for (auto& p : positions)
    {
        p.x -= mx;
        p.y -= my;
    }

    // --- ���_����]������ ---
    float theta = DirectX::XMConvertToRadians(transform_.GetAngle()); // �p�x�����W�A��(��)�ɕϊ�
    float c = cosf(theta);
    float s = sinf(theta);
    for (auto& p : positions)
    {
        DirectX::XMFLOAT2 r = p;
        p.x = c * r.x + -s * r.y;
        p.y = s * r.x + c * r.y;
    }

    // --- ��]�̂��߂Ɉړ����������_�����̈ʒu�ɖ߂� ---
    for (auto& p : positions)
    {
        p.x += mx;
        p.y += my;
    }

    // --- �X�N���[�����W�n����NDC���W�n�֕ϊ����� ---
    for (auto& p : positions)
    {
        p.x = 2.0f * p.x / screenWidth - 1.0f;
        p.y = 1.0f - 2.0f * p.y / screenHeight;
    }

    // --- ���_�o�b�t�@�[�̓��e�̕ҏW���� ---
    FrameResource& frameResource = frameResource_.at(graphics.GetCurrentBufferIndex());
    Vertex* v = frameResource.vertexData_;

    for (int i = 0; i < 4; ++i)
    {
        v[i].position_.x = positions[i].x;
        v[i].position_.y = positions[i].y;
        v[i].position_.z = 0.0f;

        v[i].color_.x = transform_.GetColorR();
        v[i].color_.y = transform_.GetColorG();
        v[i].color_.z = transform_.GetColorB();
        v[i].color_.w = transform_.GetColorA();

        v[i].texcoord_.x = texcoords[i].x / transform_.GetOriginalSizeX();
        v[i].texcoord_.y = texcoords[i].y / transform_.GetOriginalSizeY();
    }



    // �p�C�v���C���ݒ�
    commandList->SetPipelineState(pipelineState_.Get());
    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    // �f�B�X�N���v�^�e�[�u��
    commandList->SetGraphicsRootDescriptorTable(0, shaderResourceViewDescriptor_->GetGpuHandle());
    commandList->SetGraphicsRootDescriptorTable(1, emissiceShaderResourceViewDescriptor_->GetGpuHandle());

    NoiseTexture::Instance().PSSetShaderResourceView(commandList, noiseTextureType);

    //DissolveConstants* dissolve;
    //dissolveConstantBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&dissolve));
    //dissolve->dissolveValue_ = dissolve_.GetDissolveValue();
    //dissolve->edgeThrehold_ = dissolve_.GetEdgeThrehold();
    //dissolve->edgeColor_ = dissolve_.GetEdgeColor();
    //dissolveConstantBuffer_->Unmap(0, nullptr);

    //commandList->SetGraphicsRootDescriptorTable(3, dissolveConstantsDescriptor_->GetGpuHandle());


    // ���_�o�b�t�@�ݒ�
    commandList->IASetVertexBuffers(0, 1, &frameResource.vertexBufferView_);
    commandList->IASetIndexBuffer(&frameResource.indexBufferView_);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    chargeConstants_->UpdateSubresource(&charge_);
    commandList->SetGraphicsRootDescriptorTable(4, chargeConstants_->GetDescriptor()->GetGpuHandle());

    // �`��
    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

// ----- ImGui�p -----
void Sprite::DrawDebug()
{
    if (ImGui::BeginMenu(GetName()))
    {
        // --- Transform ---
        GetTransform()->DrawDebug();

        ImGui::DragFloat("charge_", &charge_.value_);

        // --- �U�� ---
        ImGui::Checkbox(reinterpret_cast<const char*>(u8"�U��"), &isVibration_);
        if (isVibration_) GetVibration()->DrawDebug();

        // --- �f�B�]���u ---
        ImGui::Checkbox(reinterpret_cast<const char*>(u8"�f�B�]���u"), &isDissolve_);
        if (isDissolve_) dissolve_.DrawDebug();

        // --- PixelShader�؂�ւ� ---       
        if (ImGui::BeginCombo(reinterpret_cast<const char*>(u8"�s�N�Z���V�F�[�_�["), currentItem))
        {
            for (int index = 0; index < 3; ++index)
            {
                if (ImGui::Selectable(list[index].c_str()))
                {
                    currentItem = list[index].c_str();
                    (this->*usePSFunc[index])();
                }
            }

            ImGui::EndCombo();
        }

        ImGui::EndMenu();
    }
}

void Sprite::UseDefaultPixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ���[�g�V�O�l�`�� ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- �V�F�[�_�[ ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();
    graphicsPipeline_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength  = psBlob_->GetBufferSize();

    // --- ���̓��C�A�E�g ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

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
    graphicsPipeline_.DepthStencilState.DepthEnable = false;
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

void Sprite::UseDissolvePixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ���[�g�V�O�l�`�� ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- �V�F�[�_�[ ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength  = vsBlob_->GetBufferSize();

    graphicsPipeline_.PS.pShaderBytecode = psDissolveBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength  = psDissolveBlob_->GetBufferSize();

    // --- ���̓��C�A�E�g ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

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

void Sprite::UseEmissivePixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ���[�g�V�O�l�`�� ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- �V�F�[�_�[ ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength  = vsBlob_->GetBufferSize();

    graphicsPipeline_.PS.pShaderBytecode = psEmissiveBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength  = psEmissiveBlob_->GetBufferSize();

    // --- ���̓��C�A�E�g ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

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

void Sprite::UseChargePixelShader()
{
    HRESULT result = S_OK;

    Graphics::Instance().WaitIdle();

    // --- ���[�g�V�O�l�`�� ---
    graphicsPipeline_.pRootSignature = rootSignature_.Get();

    // --- �V�F�[�_�[ ---
    graphicsPipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
    graphicsPipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();

    graphicsPipeline_.PS.pShaderBytecode = psChargeBlob_->GetBufferPointer();
    graphicsPipeline_.PS.BytecodeLength = psChargeBlob_->GetBufferSize();

    // --- ���̓��C�A�E�g ---
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    graphicsPipeline_.InputLayout.pInputElementDescs = inputElementDesc;
    graphicsPipeline_.InputLayout.NumElements = _countof(inputElementDesc);

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

// ----- Sprite::Transform ImGui�p -----
void Sprite::Transform::DrawDebug()
{
    if (ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"position"), &position_.x);
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"size"), &size_.x);

        if (ImGui::Button(reinterpret_cast<const char*>(u8"sizeFactor�g�p����")))
        {
            isSizeFactor_ = !isSizeFactor_;
        }
        if (isSizeFactor_)
        {
            float sizeFactor = size_.x;
            ImGui::DragFloat("sizeFactor", &sizeFactor);
            SetSize(sizeFactor);
        }

        ImGui::ColorEdit4(reinterpret_cast<const char*>(u8"color"), &color_.x);
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"angle"), &angle_);
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"texPos"), &texPos_.x);
        ImGui::DragFloat2(reinterpret_cast<const char*>(u8"texSize"), &texSize_.x);

        ImGui::TreePop();
    }
}

// ----- �U�� -----
void Sprite::Transform::Vibration(const float& elapsedTime)
{
    if (GetVibration()->GetVolume() <= 0) return; // �U���l���Ȃ��ꍇ�͏������Ȃ�

    // �U���̊Ԃ̎��Ԃ��󂯂邽�߂̏���
    if (GetVibration()->GetVibrationBreakTimer() >= 0.0f)
    {   
        // breakTime���c���Ă���Ƃ��͏������Ȃ�
        GetVibration()->SubtractVibrationBreakTimer(elapsedTime);

        return;
    }

    // breakTime��ݒ�
    GetVibration()->SetVibrationBreakTimer(GetVibration()->GetBreakTime());

    // �O��̌��ʂ����Z�b�g����
    SubtractPos(GetVibration()->GetOldVibration());

    // �U���̕��A�������Z�o����
    DirectX::XMFLOAT2 vibration = { (rand() % 100 - 50.0f), (rand() % 100 - 50.0f) };
    DirectX::XMVECTOR Vibration = DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&vibration));

    float vibrationVolume = GetVibration()->GetVolume() * 1000 * elapsedTime;

    Vibration = DirectX::XMVectorScale(Vibration, vibrationVolume);
    DirectX::XMStoreFloat2(&vibration, Vibration);

    // �U���̒l������B
    AddPos(vibration);

    // ���݂̐U���l��ۑ����Ă���
    GetVibration()->SetOldVibration(vibration);
}

// ----- Sprite::VibrationTransfom ImGui�p -----
void Sprite::VibrationTransform::DrawDebug()
{
    if (ImGui::TreeNode(reinterpret_cast<const char*>(u8"�U��")))
    {
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"�U���̐U��"), &volume_);
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"���̐U���܂ł̎���"), &breakTime_);
        ImGui::InputFloat(reinterpret_cast<const char*>(u8"�^�C�}�["), &vibrationBreakTimer_);

        ImGui::TreePop();
    }
}

// ----- Sprite::Dissolve ImGui�p -----
void Sprite::Dissolve::DrawDebug()
{
    if (ImGui::TreeNode(reinterpret_cast<const char*>(u8"�f�B�]���u")))
    {
        ImGui::SliderFloat(reinterpret_cast<const char*>(u8"�K���l"), &dissolveValue_, 0.0f, 1.0f);
        ImGui::DragFloat(reinterpret_cast<const char*>(u8"����臒l"), &edgeThrehold_);
        ImGui::ColorEdit4(reinterpret_cast<const char*>(u8"���̐F"), &edgeColor_.x);

        ImGui::TreePop();
    }
}
