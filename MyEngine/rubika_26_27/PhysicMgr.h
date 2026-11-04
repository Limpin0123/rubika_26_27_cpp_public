#pragma once

#include "SceneMgr.h"

class PhysicMgr
{
public:
	PhysicMgr(SceneMgr* sceneMgr);

	void Init();
	void Update(float fDeltaTime);
	void Shut();

private:
	SceneMgr& SceneManager;
};

