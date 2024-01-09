#pragma once
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Descriptor.h"
#include "ImGuiRenderer.h"

#include "Shader.h"

// 垂直同期間隔設定
static const int SyncInterval = 1;

class Graphics
{
public:
    Graphics(const HWND& hwnd, const UINT& bufferCount);
    ~Graphics();

    void WaitIdle();                    // 描画コマンド実行完了まで待つ    
    void Execute();                     // 描画実行
    ID3D12GraphicsCommandList* Begin(); // 描画開始
    void End();                         // 描画終了

    // シーン用定数バッファー更新
    Descriptor* UpdateSceneConstantBuffer(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT3& lightDirection);

    // テクスチャ読み込み
    HRESULT LoadTextureFromFile(const wchar_t* filename, ID3D12Resource** resource);
    HRESULT LoadTexture(const char* filename, ID3D12Resource** resource);

    // テクスチャ作成
    HRESULT CreateTexture(const BYTE* pixels, UINT width, UINT height, DXGI_FORMAT format, ID3D12Resource** resource);

    // ダミーテクスチャ生成
    HRESULT CreateDummyTexture(ID3D12Resource** resource);

    // バッファーコピー
    HRESULT CopyBuffer(ID3D12Resource* srcResource, ID3D12Resource* dstResource);

public:// とりあえずここにおいておく
    struct SceneConstants
    {
        DirectX::XMFLOAT4X4 viewProjection_;
        DirectX::XMFLOAT4   lightDirection_;
    };

public:// 取得・設定
#pragma region [Get, Set]Function
    // --- インスタンス取得 ---
    static Graphics& Instance() { return *instance_; }

    // --- DXGI関連取得 ---
    ID3D12Device* GetDevice() { return device_.Get(); }

    // --- バッファ取得 ---
    UINT GetBufferCount() const { return bufferCount_; }
    UINT GetCurrentBufferIndex() const { return swapChain_->GetCurrentBackBufferIndex(); }

    // --- ディスクリプタヒープの取得 ---
    DescriptorHeap* GetSrvCbvUavDescriptorHeap() const { return srvCbvUavDescriptorHeap_.get(); }
    DescriptorHeap* GetSamplerDescriptorHeap() const { return samplerDescriptorHeap_.get(); }
    
    DescriptorHeap* GetRtvDescriptorHeap() { return renderTargetViewDescriptorHeap_.get(); }
    DescriptorHeap* GetDsvDescriptorHeap() { return depthStencilViewDescriptorHeap_.get(); }

    // --- スクリーンサイズ取得 ---
    const float GetScreenWidth() const { return screenWidth_; }
    const float GetScrennHeight() const { return screenHeight_; }

    // --- ImGuiレンダラ取得 ---
    ImGuiRenderer* GetImGuiRenderer() const { return imguiRenderer_.get(); }

    // --- Shader ---
    Shader* GetShader() const { return shader_.get(); }

#pragma endregion

private:// 内部処理関数
#pragma region [private]Function
    void CreateDXGIDevice();                                            // DXGI まわり
    void CreateGraphicsCommandQueue();                                  // グラフィックスコマンドキューの生成    
    void CreateSwapChain(const HWND& hwnd, const UINT& bufferCount);    // スワップチェインの生成
    void CreateFrameResource(const UINT& bufferCount);                  // フレームリソース
    void CreateResourceCommand();                                       // リソース用コマンド関連

    // イメージコピー
    HRESULT CopyImage(const BYTE* pixels, UINT width, UINT height, DXGI_FORMAT format, ID3D12Resource* resource);

    // フォーマットに対応した１ピクセルあたりのビット数を返す
    static UINT BitsPerPixel(DXGI_FORMAT format);

    // コマンドキュー
    struct CommandQueue
    {
        Microsoft::WRL::ComPtr<ID3D12CommandQueue>  commandQueue_   = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Fence>         fence_          = nullptr;
        HANDLE                                      fenceEvent_     = NULL;
        UINT64                                      fenceValue_     = 0;
    };

    // コマンド終了を待つ
    void WaitIdle(CommandQueue& commandQueue);

#pragma endregion

private:
    // --- 疑似インスタンス ---
    static Graphics* instance_;

    // --- 画面表示に必要なもの ---
#pragma region 画面表示に必要なもの
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


    float fov_ = 45.0f; // 視野角



    // --- 画面サイズ ---
    float screenWidth_  = 0;
    float screenHeight_ = 0;
};