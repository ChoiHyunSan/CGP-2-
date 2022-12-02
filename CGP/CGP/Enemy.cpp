#include "Enemy.h"
#include "Inputclass.h"
#include "TimeMgr.h"
#include "pch.h"
#include "Scene.h"
#include "SceneMgr.h"
void Enemy::Update()
{	
	Move();
}

// 이동하는 방향에 따라서 타겟 위치를 변경한다.
void Enemy::setTargetPos(DIR dir)
{
	isMove = true;
	switch (dir)
	{
	case DIR::UP:
		targetPos.z += 1;
		setRotate('y', 3.141592f / 1);
		break;
	case DIR::DOWN:
		targetPos.z -= 1;
		setRotate('y', 0);

		break;
	case DIR::LEFT:
		targetPos.x -= 1;
		setRotate('y', 3.141592f / 2);
		break;
	case DIR::RIGHT:
		targetPos.x += 1;
		setRotate('y', 3.141592f / 2 * 3);
		break;
	}
	OutputDebugStringA((to_string(targetPos.x) + " ").c_str());
	OutputDebugStringA((to_string(targetPos.z)+ "\n").c_str());
	// 목표 위치가 비어있지 않다면

	 //targetPos가 맵을 벗어난다면 이동하지 않게 조정한다.
	if (targetPos.x > MAPSIZE / 2)
	{
		targetPos.x -= 1;
		isMove = false;
		Move();
		return;
	}
	if (targetPos.z > MAPSIZE) 
	{ 
		targetPos.z -= 1;
		isMove = false;
		Move();
		return;
	}
	if (targetPos.x < -MAPSIZE / 2) 
	{ 
		targetPos.x += 1;
		isMove = false;
		Move();
		return;
	}
	if (targetPos.z < 0) 
	{ 
		targetPos.z += 1; 
		isMove = false;
		Move();
		return;
	}

	if (!SceneMgr::GetInst()->GetCurScene()->isEmptyPlace(MAPSIZE - targetPos.z - 1, targetPos.x + MAPSIZE / 2))
	{
		switch (dir)
		{
		case DIR::UP:
			targetPos.z -= 1;
			break;
		case DIR::DOWN:
			targetPos.z += 1;
			break;
		case DIR::LEFT:
			targetPos.x -= 1;
			break;
		case DIR::RIGHT:
			targetPos.x += 1;
			break;
		}

		isMove = false;
		Move();
	}
}

void Enemy::Move()
{
	int dir = rand() % 4;

	if (!isMove) setTargetPos((DIR)dir);

	// 목표 위치를 향해서 이동한다.
	if (getPos().x >= targetPos.x)
	{
		addPos(-fDT * speed, 0, 0);
	}
	if (getPos().x <= targetPos.x)
	{
		addPos(fDT * speed, 0, 0);
	}
	if (getPos().z >= targetPos.z)
	{
		addPos(0, 0, -fDT * speed);
	}
	if (getPos().z <= targetPos.z)
	{
		addPos(0, 0, fDT * speed);
	}

	// 목표 위치에 도달하면 이동상태를 false로 변환
	if (abs(targetPos.x - getPos().x) <= 0.05f && abs(targetPos.z - getPos().z) <= 0.05f)
	{
		isMove = false;
	}
}


Enemy::Enemy(ID3D11Device* device , GROUP_TYPE type, Pos pos)
	: ModelClass(device, GROUP_TYPE::ENEMY),
	targetPos(0, 0, 0), isMove(false), speed(1.3f)
{
	setPos(pos.x, pos.y +1, pos.z);
	SetName(L"Enemy");
	// 이동 목적지의 초기위치는 Enemy Class의 스폰 위치로 지정한다.
	targetPos = this->getPos();
	GetCollider()->SetScale(Pos(0.8, 0.8f, 0.8f));
}

Enemy::~Enemy()
{
}
