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
	// �� ���� ���� �׷� Ÿ���� ������,
	// ū ���� ��(��Ʈ)�� ���

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
		// ��Ʈ ����
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

	// Effect�� ���� ��� �ٲ��ش�.

	const vector<ModelClass*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<ModelClass*>& vecRight = pCurScene->GetGroupObject(_eRight);

	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// ���� �������� üũ
		if (vecLeft[i]->IsDead()) continue;
		
		// Collider�� �ִ��� üũ
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// ���� �������� üũ
			if (vecRight[j]->IsDead()) continue;

			// Collider�� ���ų� �ڱ� �ڽŰ� �浹�ϴ��� üũ
			if (nullptr == vecRight[j]->GetCollider()
				|| vecLeft[i] == vecRight[j])
			{
				continue;
			}

			Collider* pLeftCol = vecLeft[i]->GetCollider();
			Collider* pRightCol = vecRight[j]->GetCollider();

			// �� �浹ü ���� ���̵� ����
			COLLIDER_ID ID;
			ID.Left_id = vecLeft[i]->GetCollider()->GetID();
			ID.Right_id = vecRight[j]->GetCollider()->GetID();

			iter = m_mapColInfo.find(ID.ID);

			// �浹 ������ �� ��� ������ ��� ���(�浹���� �ʾҴٷ�)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			// �浹 ó��
			if (isCollision(pLeftCol, pRightCol))
			{
				// ���� �浹���̴�.

				if (iter->second)
				{
					// �������� �浹 �ϰ� �־���.

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// ���� �ϳ��� ���� �����̶��, �浹 ���������ش�.
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
						// �������� �浹���� �ʾҴ�.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}

				}
			}
			else
			{
				// ���� �浹�ϰ� ���� �ʴ�.

				if (iter->second)
				{
					// �������� �浹�ϰ� �־���.
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

	// Effect�� ���� ��� �ٲ��ش�.

	const vector<ModelClass*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<EffectClass*>& vecRight = pCurScene->GetGroupEffect(_eRight);

	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// Collider�� �ִ��� üũ
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// Collider�� ���ų� �ڱ� �ڽŰ� �浹�ϴ��� üũ
			if (nullptr == vecRight[j]->GetCollider())
			{
				continue;
			}

			Collider* pLeftCol = vecLeft[i]->GetCollider();
			Collider* pRightCol = vecRight[j]->GetCollider();

			// �� �浹ü ���� ���̵� ����
			COLLIDER_ID ID;
			ID.Left_id = vecLeft[i]->GetCollider()->GetID();
			ID.Right_id = vecRight[j]->GetCollider()->GetID();

			iter = m_mapColInfo.find(ID.ID);

			// �浹 ������ �� ��� ������ ��� ���(�浹���� �ʾҴٷ�)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			// �浹 ó��
			if (isCollision(pLeftCol, pRightCol))
			{
				// ���� �浹���̴�.

				if (iter->second)
				{
					// �������� �浹 �ϰ� �־���.

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// ���� �ϳ��� ���� �����̶��, �浹 ���������ش�.
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
						// �������� �浹���� �ʾҴ�.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}

				}
			}
			else
			{
				// ���� �浹�ϰ� ���� �ʴ�.
				if (iter->second)
				{
					// �������� �浹�ϰ� �־���.
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