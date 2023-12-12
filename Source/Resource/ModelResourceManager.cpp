#include "ModelResourceManager.h"


const std::shared_ptr<Model> ModelResourceManager::LoadModelResource(const char* const fbxFilename, const bool triangulate, const float samplingRate)
{
    // --- モデルを検索 ---
    const ModelMap::iterator it = models.find(fbxFilename);

    if (it != models.end())
    {
        // --- リンク（寿命）が切れてないか確認 ---
        if (it->second.expired() == false)
        {
            // --- 検索したモデルと同じモデルが見つかったらそれを返す ---
            return it->second.lock();
        }
    }

    // --- 見つからなかったら新規モデルリソースを作成・読み込み ---
    const std::shared_ptr<Model> model = std::make_shared<Model>(
        fbxFilename,
        triangulate,
        samplingRate
    );

    // --- マップに登録 ---
    models[fbxFilename] = model;

    return model;
}
