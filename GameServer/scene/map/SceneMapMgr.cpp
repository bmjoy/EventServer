#include "SceneMapMgr.h"
#include "SceneMap.h"
#include "scene/Scene.h"

std::unique_ptr<SceneMapMgr> g_pSceneMapMgr = nullptr;

bool SceneMapMgr::LoadAllMap()
{
	// 读取配置文件<sceneids、lineid>

	// 加载地图

	return true;
}

Scene* SceneMapMgr::GetScene(int32_t mapid, int32_t lineid)
{
	return nullptr;
}