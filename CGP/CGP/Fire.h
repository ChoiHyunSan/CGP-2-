#pragma once
#include "EffectClass.h"
#include "pch.h"

class Fire :
    public EffectClass
{
    float timer;

public:
    virtual void Update();

public:
    Fire(ID3D11Device* device, Pos pos);
    ~Fire();

    void OnCollisionEnter(Collider* _pOther);
};

