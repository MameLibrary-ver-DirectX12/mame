#pragma once
#include "BaseScene.h"

class SceneManager
{
private:
    SceneManager() {}
    ~SceneManager() {}

public:
    // --- インスタンス取得 ---
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    void Update(const float& elapsedTime);                      // 更新
    void Render(ID3D12GraphicsCommandList* commandList);        // 描画
    void UIRender(ID3D12GraphicsCommandList* commnadList);      // UI描画
    void ShadowRender(ID3D12GraphicsCommandList* commandList);  // 影抽出

    
    void Clear();                                           // シーンクリア

    void ChangeScene(BaseScene* scene);                         // シーン切り替え

    BaseScene* GetCurrentScene() const { return currentScene_; }

    void ResetCountFrame() { countFrame_ = 0; }

private:
    BaseScene* currentScene_    = nullptr;
    BaseScene* nextScene_       = nullptr;

    int countFrame_ = 0; // 処理落ち対策用変数
};

