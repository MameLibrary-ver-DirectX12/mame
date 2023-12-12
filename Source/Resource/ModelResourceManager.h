#pragma once

#include <map>
#include <string>
#include <memory>

#include "Model.h"

class ModelResourceManager
{
private:
    ModelResourceManager() {}
    ~ModelResourceManager() {}

public:
    static ModelResourceManager& Instance()
    {
        static ModelResourceManager instance;
        return instance;
    }

    const std::shared_ptr<Model> LoadModelResource(
        const char* const   fbxFilename,
        const bool          triangulate = false,
        const float         samplingRate = 0
    );

private:
    using ModelMap = std::map<const char*, std::weak_ptr<Model>>;

    ModelMap models = {};
};

