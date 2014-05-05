
#pragma once

#include <memory>

#include "assetmanager.h"

class LoadingScreen {  // public Screen
public:
    LoadingScreen(std::unique_ptr<AssetManager>& assetManager, int width, int height);
    ~LoadingScreen();
};