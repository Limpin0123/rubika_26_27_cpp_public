#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>
#include "SceneMgr.h"
#include "TextureMgr.h"

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	SceneManager = new SceneMgr();
	TextureManager = new TextureMgr();
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	Logger->Init();
	TextureManager->Init();
}

void Globals::Shut()
{
	Logger->Shut();
	TextureManager->Shut();
}

void Globals::Destroy()
{
	delete DebugMgr;
	DebugMgr = nullptr;

	delete Logger;
	Logger = nullptr;

	delete SceneManager;
	SceneManager = nullptr;

	delete TextureManager;
	TextureManager = nullptr;
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
