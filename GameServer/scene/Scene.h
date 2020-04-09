#pragma once
#include "map/ZoneMgr.h"
#include "map/SceneMap.h"

class SceneMap;
enum ESceneType
{
	E_SCENE_NORM = 1,		// 普通场景
	E_SCENE_MIRROR = 2,		// 游戏镜像
	E_SCENE_DUNGEON = 3,	// 游戏副本
};

// 场景处理
class Scene
{
public:
	bool Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize);

	ESceneType GetSceneType() { return m_scene_type; }
	void SetSceneMap(SceneMap* scenemap) { m_scene_map = scenemap; }
	SceneMap* GetSceneMap() { return m_scene_map; }
private:
	ESceneType m_scene_type;
	SceneMap* m_scene_map;
	ZoneMgr m_zone_mgr;
};

