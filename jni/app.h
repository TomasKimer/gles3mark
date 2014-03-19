/*
 * app.h
 */

#pragma once

#include "baseapp.h"

class App : public BaseApp {
public:
	App(android_app* state): BaseApp(state) {}
	virtual ~App() {}
};
