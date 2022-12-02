#pragma once
#include "Scene.h"

class Stage01_Scene
	:public Scene
{
public:
	Stage01_Scene();
	~Stage01_Scene();

public:

	virtual void init(D3DClass* D3D) override;
	virtual void Exit() override;
	virtual void finalUpdate(D3DClass* D3D) override;
	virtual void SetObjects(D3DClass* D3D) override;
};

