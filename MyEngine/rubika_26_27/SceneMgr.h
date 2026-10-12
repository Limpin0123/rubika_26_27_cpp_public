#pragma once

#include "Scene.h"
#include "DebugMgr.h"

class SceneMgr : public IDebugable
{
public: 
	SceneMgr();
	~SceneMgr();

	Scene* activeScene;

	Scene* AddAndLoadScene(std::string sceneName);
	Scene* AddScene(std::string sceneName);
	void RemoveScene(std::string sceneName);

	void LoadScene(std::string sceneName);

	void UpdateActiveScene(float fDeltaTime);

	virtual void DrawDebug() override;

private:
	std::unordered_map<std::string, Scene> scenes;

#ifdef _USE_IMGUI
	Scene* inspectedScene;
#endif
};

extern SceneMgr SceneManager;