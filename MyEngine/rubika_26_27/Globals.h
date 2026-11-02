#pragma once

//#define TEXTURE_MANAGER_DEEP_DEBUG

#include <unordered_map>

class DebugMgr;
class Logger;
class SceneMgr;
class TextureMgr;
class InputMgr;

enum AnimationRevertType
{
	RevertAnimation,
	RevertX,
	RevertY,
	RevertXY
};

class Globals
{
public:
	Globals();
	~Globals();

	void Init();
	void Shut();
	void Destroy();

	void IncrementFrameCount();

	unsigned long long FrameCount;

	DebugMgr* DebugMgr;
	Logger* Logger;
	SceneMgr* SceneManager;
	TextureMgr* TextureManager;
	InputMgr* InputManager;
};

extern Globals gData;