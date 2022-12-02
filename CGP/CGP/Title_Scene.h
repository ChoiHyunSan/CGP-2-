#pragma once
#include "Scene.h"

class D3DClass;

class Title_Scene :
    public Scene
{

public:
    Title_Scene();
    ~Title_Scene();

public:
    virtual void init(D3DClass* D3D) override;
    virtual void Exit() override;
    virtual void finalUpdate(D3DClass* D3D) override;
    virtual void SetObjects(D3DClass* D3D) override;
};

