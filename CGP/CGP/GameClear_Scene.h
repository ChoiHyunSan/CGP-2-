#pragma once
#include "Scene.h"
class GameClear_Scene :
    public Scene
{
public:
	GameClear_Scene();
	~GameClear_Scene();

public:
	virtual void init(D3DClass* D3D) override;
	virtual void Exit() override;
	virtual void finalUpdate(D3DClass* D3D) override;
	virtual void SetObjects(D3DClass* D3D) override;
};

