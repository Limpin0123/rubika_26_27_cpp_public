#pragma once

#include <unordered_map>

class DebugMgr;
class Logger;
class SceneMgr;
class TextureMgr;

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
};

extern Globals gData;