#pragma once
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Descriptor.h"
#include "ImGuiRenderer.h"

#include "Shader.h"

// ���������Ԋu�ݒ�
static const int SyncInterval = 1;

class Graphics
{
public:
    Graphics(const HWND& hwnd, const UINT& bufferCount);
    ~Graphics();

    void WaitIdle();                    // �`��R�}���h���s�����܂ő҂�    
    void Execute();                     // �`����s
    ID3D12GraphicsCommandList* Begin(); // �`��J�n
    void End();                         // �`��I��

    // �V�[���p�萔�o�b�t�@�[�X�V
    Descriptor* UpdateSceneConstantBuffer(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT3& lightDirection);

    // �e�N�X�`���ǂݍ���
    HRESULT LoadTextureFromFile(const wchar_t* filename, ID3D12Resource** resource);
    HRESULT LoadTexture(const char* filename, ID3D12Resource** resource);

    // �e�N�X�`���쐬
    HRESULT CreateTexture(const BYTE* pixels, UINT width, UINT height, DXGI_FORMAT format, ID3D12Resource** resource);

    // �_�~�[�e�N�X�`������
    HRESULT CreateDummyTexture(ID3D12Resource** resource);

    // �o�b�t�@�[�R�s�[
    HRESULT CopyBuffer(ID3D12Resource* srcResource, ID3D12Resource* dstResource);

public:// �Ƃ肠���������ɂ����Ă���
    struct SceneConstants
    {
        DirectX::XMFLOAT4X4 viewProjection_;
        DirectX::XMFLOAT4   lightDirection_;
    };

public:// �擾�E�ݒ�
#pragma region [Get, Set]Function
    // --- �C���X�^���X�擾 ---
    static Graphics& Instance() { return *instance_; }

    // --- DXGI�֘A�擾 ---
    ID3D12Device* GetDevice() { return device_.Get(); }

    // --- �o�b�t�@�擾 ---
    UINT GetBufferCount() const { return bufferCount_; }
    UINT GetCurrentBufferIndex() const { return swapChain_->GetCurrentBackBufferIndex(); }

    // --- �f�B�X�N���v�^�q�[�v�̎擾 ---
    DescriptorHeap* GetSrvCbvUavDescriptorHeap() const { return srvCbvUavDescriptorHeap_.get(); }
    DescriptorHeap* GetSamplerDescriptorHeap() const { return samplerDescriptorHeap_.get(); }
    
    DescriptorHeap* GetRtvDescriptorHeap() { return renderTargetViewDescriptorHeap_.get(); }
    DescriptorHeap* GetDsvDescriptorHeap() { return depthStencilViewDescriptorHeap_.get(); }

    // --- �X�N���[���T�C�Y�擾 ---
    const float GetScreenWidth() const { return screenWidth_; }
    const float GetScrennHeight() const { return screenHeight_; }

    // --- ImGui�����_���擾 ---
    ImGuiRenderer* GetImGuiRenderer() const { return imguiRenderer_.get(); }

    // --- Shader ---
    Shader* GetShader() const { return shader_.get(); }

#pragma endregion

private:// ���������֐�
#pragma region [private]Function
    void CreateDXGIDevice();                                            // DXGI �܂��
    void CreateGraphicsCommandQueue();                                  // �O���t�B�b�N�X�R�}���h�L���[�̐���    
    void CreateSwapChain(const HWND& hwnd, const UINT& bufferCount);    // �X���b�v�`�F�C���̐���
    void CreateFrameResource(const UINT& bufferCount);                  // �t���[�����\�[�X
    void CreateResourceCommand();                                       // ���\�[�X�p�R�}���h�֘A

    // �C���[�W�R�s�[
    HRESULT CopyImage(const BYTE* pixels, UINT width, UINT height, DXGI_FORMAT format, ID3D12Resource* resource);

    // �t�H�[�}�b�g�ɑΉ������P�s�N�Z��������̃r�b�g����Ԃ�
    static UINT BitsPerPixel(DXGI_FORMAT format);

    // �R�}���h�L���[
    struct CommandQueue
    {
        Microsoft::WRL::ComPtr<ID3D12CommandQueue>  commandQueue_   = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Fence>         fence_          = nullptr;
        HANDLE                                      fenceEvent_     = NULL;
        UINT64                                      fenceValue_     = 0;
    };

    // �R�}���h�I����҂�
    void WaitIdle(CommandQueue& commandQueue);

#pragma endregion

private:
    // --- �^���C���X�^���X ---
    static Graphics* instance_;

    // --- ��ʕ\���ɕK�v�Ȃ��� ---
#pragma region ��ʕ\���ɕK�v�Ȃ���
    struct FrameResource
    {
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      commandAllocator_   = nullptr;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   commandList_        = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource>              renderTargetView_   = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource>              depthStencilView_   = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource>              constantBufferView_ = nullptr;
        Descriptor*                                         renderTargetViewDescriptor_     = nullptr;
        Descriptor*                                         depthStencilViewDescriptor_     = nullptr;
        Descriptor*                                         constantBufferViewDescriptor_   = nullptr;
        SceneConstants*                                     sceneConstants_                 = nullptr;
    };
    
    Microsoft::WRL::ComPtr<ID3D12Device>                device_                     = nullptr; 
    Microsoft::WRL::ComPtr<IDXGIFactory4>               dxgiFactory_                = nullptr; 
    Microsoft::WRL::ComPtr<IDXGISwapChain4>             swapChain_                  = nullptr; 
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      resourceCommandAllocator_   = nullptr; 
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   resourceCommandList_        = nullptr; 
    
    CommandQueue graphicsQueue_;
    CommandQueue resourceQueue_;
    
    std::vector<FrameResource> frameResource_;
    
    std::unique_ptr<DescriptorHeap> renderTargetViewDescriptorHeap_     = nullptr;
    std::unique_ptr<DescriptorHeap> depthStencilViewDescriptorHeap_     = nullptr;
    std::unique_ptr<DescriptorHeap> srvCbvUavDescriptorHeap_            = nullptr;
    std::unique_ptr<DescriptorHeap> samplerDescriptorHeap_              = nullptr;

    std::unique_ptr<ImGuiRenderer> imguiRenderer_;

    UINT bufferCount_ = 0;
    //std::vector<ID3D12Resource*> backBuffers_;

#pragma endregion

    // --- Shader ---
    std::unique_ptr<Shader> shader_;


    float fov_ = 45.0f; // ����p



    // --- ��ʃT�C�Y ---
    float screenWidth_  = 0;
    float screenHeight_ = 0;
};