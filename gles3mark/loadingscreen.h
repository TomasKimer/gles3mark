
#pragma once

#include <memory>

#include "assetmanager.h"

class LoadingScreen {  // public Screen
public:
    LoadingScreen(const AssetManager& assetManager, int width, int height);
    ~LoadingScreen();
};