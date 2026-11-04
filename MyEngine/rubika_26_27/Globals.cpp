#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>
#include "SceneMgr.h"
#include "PhysicMgr.h"
#include "TextureMgr.h"
#include "InputMgr.h"

Globals gData;

Globals::Globals() : FrameCount(0)
{
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	SceneManager = new SceneMgr();
	PhysicManager = new PhysicMgr(SceneManager);
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
	PhysicManager->Init();
	TextureManager->Init();
	InputManager->Init();
}

void Globals::Shut()
{
	InputManager->Shut();
	SceneManager->Shut();
	PhysicManager->Shut();
	TextureManager->Shut();
	Logger->Shut();
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
