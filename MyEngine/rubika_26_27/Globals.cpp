#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "InputMgr.h"

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	SceneManager = new SceneMgr();
	TextureManager = new TextureMgr();
	InputManager = new InputMgr();
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	Logger->Init();
	SceneManager->Init();
	TextureManager->Init();
	InputManager->Init();
}

void Globals::Shut()
{
	Logger->Shut();
	SceneManager->Shut();
	TextureManager->Shut();
	InputManager->Shut();
}

void Globals::Destroy()
{
	delete SceneManager;
	SceneManager = nullptr;

	delete TextureManager;
	TextureManager = nullptr;

	delete DebugMgr;
	DebugMgr = nullptr;

	delete Logger;
	Logger = nullptr;
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
