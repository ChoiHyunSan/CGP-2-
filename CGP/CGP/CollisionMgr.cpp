#include "CollisionMgr.h"
#include "pch.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ModelClass.h"
#include "Collider.h"

CollisionMgr::CollisionMgr()
	: m_arrCheck{}
{

}

CollisionMgr::~CollisionMgr()
{

}

void CollisionMgr::update()
{
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		m_arrCheck[iRow];
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				if(iCol == (UINT)GROUP_TYPE::FIRE)
					CollisionGroupUpdate((GROUP_TYPE)iRow, EFFECT_TYPE::FIRE);
				else
					CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}


}

void CollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로,
	// 큰 값을 열(비트)로 사용

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		// 비트 연산
		m_arrCheck[iRow] |= (1 << iCol);
	}

}

void CollisionMgr::Reset()
{
	memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);
}

void CollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	Scene* pCurScene = SceneMgr::GetInst()->GetCurScene();

	// Effect가 들어온 경우 바꿔준다.

	const vector<ModelClass*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<ModelClass*>& vecRight = pCurScene->GetGroupObject(_eRight);

	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 죽은 상태인지 체크
		if (vecLeft[i]->IsDead()) continue;
		
		// Collider가 있는지 체크
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 죽은 상태인지 체크
			if (vecRight[j]->IsDead()) continue;

			// Collider가 없거나 자기 자신과 충돌하는지 체크
			if (nullptr == vecRight[j]->GetCollider()
				|| vecLeft[i] == vecRight[j])
			{
				continue;
			}

			Collider* pLeftCol = vecLeft[i]->GetCollider();
			Collider* pRightCol = vecRight[j]->GetCollider();

			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.Left_id = vecLeft[i]->GetCollider()->GetID();
			ID.Right_id = vecRight[j]->GetCollider()->GetID();

			iter = m_mapColInfo.find(ID.ID);

			// 충돌 정보가 미 등록 상태인 경우 등록(충돌하지 않았다로)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			// 충돌 처리
			if (isCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌중이다.

				if (iter->second)
				{
					// 이전에도 충돌 하고 있었다.

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 둘중 하나가 삭제 예정이라면, 충돌 해제시켜준다.
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}
				else
				{
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						// 이전에는 충돌하지 않았다.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}

				}
			}
			else
			{
				// 현재 충돌하고 있지 않다.

				if (iter->second)
				{
					// 이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

void CollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, EFFECT_TYPE _eRight)
{
	Scene* pCurScene = SceneMgr::GetInst()->GetCurScene();

	// Effect가 들어온 경우 바꿔준다.

	const vector<ModelClass*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<EffectClass*>& vecRight = pCurScene->GetGroupEffect(_eRight);

	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// Collider가 있는지 체크
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// Collider가 없거나 자기 자신과 충돌하는지 체크
			if (nullptr == vecRight[j]->GetCollider())
			{
				continue;
			}

			Collider* pLeftCol = vecLeft[i]->GetCollider();
			Collider* pRightCol = vecRight[j]->GetCollider();

			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.Left_id = vecLeft[i]->GetCollider()->GetID();
			ID.Right_id = vecRight[j]->GetCollider()->GetID();

			iter = m_mapColInfo.find(ID.ID);

			// 충돌 정보가 미 등록 상태인 경우 등록(충돌하지 않았다로)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			// 충돌 처리
			if (isCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌중이다.

				if (iter->second)
				{
					// 이전에도 충돌 하고 있었다.

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 둘중 하나가 삭제 예정이라면, 충돌 해제시켜준다.
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}
				else
				{

					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						// 이전에는 충돌하지 않았다.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}

				}
			}
			else
			{
				// 현재 충돌하고 있지 않다.
				if (iter->second)
				{
					// 이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

bool CollisionMgr::isCollision(Collider* _pLeftCol, Collider* _pRightCol)
{
	Pos vLeftPos = _pLeftCol->GetFinalPos();
	Pos vLeftScale = _pLeftCol->GetScale();

	Pos vRightPos = _pRightCol->GetFinalPos();
	Pos vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) < (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.z - vLeftPos.z) < (vLeftScale.z + vRightScale.z) / 2.f)
	{
		return true;
	}

	return false;
}

void CollisionMgr::init()
{

}