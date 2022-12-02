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
	map<ULONGLONG, bool> m_mapColInfo;						// 충돌체 간의 이전 프레임 충돌 정보
	UINT				m_arrCheck[(UINT)GROUP_TYPE::END];	// 그룹간의 충돌 체크 매트릭스


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

