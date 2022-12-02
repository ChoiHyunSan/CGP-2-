#include "Fire.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "EventMgr.h"
#include "GameMgr.h"
void Fire::Update()
{
	if (timer < 0.5f)
	{
		timer += fDT;
	}
	else
	{
		SceneMgr::GetInst()->GetCurScene()->ClearEffects(EFFECT_TYPE::FIRE);
		//OutputDebugStringA("FireOut");
	}
}

Fire::Fire(ID3D11Device* device, Pos pos)
	: EffectClass(device, EFFECT_TYPE::FIRE, pos),
	timer(0.f)
{

}

Fire::~Fire()
{

}

void Fire::OnCollisionEnter(Collider* _pOther)
{
	if (_pOther->GetModel()->GetName() == L"Player")
	{
		int playerLife = GameMgr::GetInst()->getPlayerLife();
		if (playerLife > 1)
		{
			// �÷��̾��� ������ ������ ��� �̺�Ʈ�� ���� �ش� �κ��� ó���ϵ��� �����ϱ�
			GameMgr::GetInst()->AddLife(-1);
			_pOther->GetModel()->setPos(0.f, 0.f, 0.f);
		}
		else
		{
			GameMgr::GetInst()->AddLife(-1);
			_pOther->GetModel()->SetDead();

			GameMgr::GetInst()->SetGameState(GAME_STATE::GAME_OVER);
		}
	}

	if (_pOther->GetModel()->GetName() == L"Enemy")
	{
		// ���� �߰��� �̺�Ʈ���� ó���ϰԲ� �����ϱ�
		GameMgr::GetInst()->AddScore();
		_pOther->GetModel()->SetDead();

		if (SceneMgr::GetInst()->GetCurScene()->isModelAllDead(GROUP_TYPE::ENEMY))
		{
			GameMgr::GetInst()->SetGameState(GAME_STATE::GAME_CLEAR);
		}

		OutputDebugStringA("Bomb Collision \n");
	}
}
