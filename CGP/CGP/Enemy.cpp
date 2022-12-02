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

// �̵��ϴ� ���⿡ ���� Ÿ�� ��ġ�� �����Ѵ�.
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
	// ��ǥ ��ġ�� ������� �ʴٸ�

	 //targetPos�� ���� ����ٸ� �̵����� �ʰ� �����Ѵ�.
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

	// ��ǥ ��ġ�� ���ؼ� �̵��Ѵ�.
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

	// ��ǥ ��ġ�� �����ϸ� �̵����¸� false�� ��ȯ
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
	// �̵� �������� �ʱ���ġ�� Enemy Class�� ���� ��ġ�� �����Ѵ�.
	targetPos = this->getPos();
	GetCollider()->SetScale(Pos(0.8, 0.8f, 0.8f));
}

Enemy::~Enemy()
{
}
