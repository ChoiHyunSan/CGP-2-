#pragma once
#include "pch.h"

class Collider;

union COLLIDER_ID
{
	struct {
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CollisionMgr
{
	SINGLE(CollisionMgr)

private:
	map<ULONGLONG, bool> m_mapColInfo;						// �浹ü ���� ���� ������ �浹 ����
	UINT				m_arrCheck[(UINT)GROUP_TYPE::END];	// �׷찣�� �浹 üũ ��Ʈ����


public:
	void init();
	void update();

	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset();

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, EFFECT_TYPE _eRight);
	bool isCollision(Collider* _pLeftCol, Collider* _pRightCol);
};

