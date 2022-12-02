#pragma once
#include "modelclass.h"
class Bomb :
    public ModelClass
{
    float timer;

public:
    void Update();

public:
    Bomb(ID3D11Device* device, GROUP_TYPE type, Pos pos);
    ~Bomb();
};

