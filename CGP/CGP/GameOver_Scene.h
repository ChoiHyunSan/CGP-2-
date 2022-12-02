#pragma once
#include "Scene.h"
class GameOver_Scene :
    public Scene
{
public:
	GameOver_Scene();
	~GameOver_Scene();

public:
	virtual void init(D3DClass* D3D) override;
	virtual void Exit() override;
	virtual void finalUpdate(D3DClass* D3D) override;
	virtual void SetObjects(D3DClass* D3D) override;
};

