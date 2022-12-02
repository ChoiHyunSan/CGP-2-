#pragma once
#include "modelclass.h"
class Player :
    public ModelClass
{


public:
    Player(ID3D11Device* device, GROUP_TYPE type, Pos pos);
    ~Player();

public:
    virtual void Update();

    void OnCollisionEnter(Collider* _pOther);
    void OnCollisionExit(Collider* _pOther);

private:
    
    // 4방향으로의 이동이 가능한지 확인
    // 1 : UP, 2 : DONW, 3: RIGHT, 4 : LEFT
    int dir = 0;

};

