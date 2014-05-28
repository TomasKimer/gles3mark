/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <memory>

#include "assetmanager.h"

/**
 * \brief Basic loading screen.
 */
class LoadingScreen {  // public Screen
public:
    LoadingScreen(const AssetManager& assetManager, int width, int height);
    ~LoadingScreen();
};