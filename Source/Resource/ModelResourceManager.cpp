#include "ModelResourceManager.h"


const std::shared_ptr<Model> ModelResourceManager::LoadModelResource(const char* const fbxFilename, const bool triangulate, const float samplingRate)
{
    // --- ���f�������� ---
    const ModelMap::iterator it = models.find(fbxFilename);

    if (it != models.end())
    {
        // --- �����N�i�����j���؂�ĂȂ����m�F ---
        if (it->second.expired() == false)
        {
            // --- �����������f���Ɠ������f�������������炻���Ԃ� ---
            return it->second.lock();
        }
    }

    // --- ������Ȃ�������V�K���f�����\�[�X���쐬�E�ǂݍ��� ---
    const std::shared_ptr<Model> model = std::make_shared<Model>(
        fbxFilename,
        triangulate,
        samplingRate
    );

    // --- �}�b�v�ɓo�^ ---
    models[fbxFilename] = model;

    return model;
}
