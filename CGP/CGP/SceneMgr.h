#pragma once
#include "pch.h"

class Scene;
class D3DClass;

class SceneMgr
{
	SINGLE(SceneMgr)

private:
	Scene* m_arrScene[(UINT)SCENE_TYPE::END];
	Scene* m_pCurScene;

public:
	void init();

	void update(D3DClass* D3D);
	void finalUpdate(D3DClass* D3D);

	void render(D3DClass* D3D, float rotation);

	void setCurScene(SCENE_TYPE _eType);
	Scene* GetCurScene() const {return m_pCurScene;}

	void Reload();
public:

};

