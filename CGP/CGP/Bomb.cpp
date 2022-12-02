#include "Bomb.h"
#include "inputclass.h"
#include "pch.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "GameMgr.h"
#include "graphicsclass.h"

Bomb::Bomb(ID3D11Device* device, GROUP_TYPE type, Pos pos)
	: ModelClass(device, GROUP_TYPE::BOMB),
	timer(0.0f)
{

}

Bomb::~Bomb()
{

}

void Bomb::Update()
{
	if (timer < 3.0f)
	{
		timer += fDT;
	}
	else
	{
		// 사라짐과 동시에 본인 위치 포함 가로세로로 각각 PlayerPower만큼 Fire Effect생성
		int power = GameMgr::GetInst()->getPlayerPower();

		// 폭탄 위치 생성
		SceneMgr::GetInst()->GetCurScene()->AddEffect(GraphicsClass::GetInst()->GetD3D(), EFFECT_TYPE::FIRE, getPos());

		for (int i = 0; i < power; ++i)
		{
			if (!SceneMgr::GetInst()->GetCurScene()->isEmptyPlace(MAPSIZE - getPos().z - 1, (getPos().x + (i + 1)) + MAPSIZE / 2))
			{
				break;
			}
			if (getPos().x + (i + 1) > MAPSIZE / 2)
			{
				break;
			}
			// 폭탄 좌우 생성
			SceneMgr::GetInst()->GetCurScene()->AddEffect(GraphicsClass::GetInst()->GetD3D(), EFFECT_TYPE::FIRE, getPos() + Pos((i + 1), 0, 0));
		}
		for (int i = 0; i < power; ++i)
		{
			if (!SceneMgr::GetInst()->GetCurScene()->isEmptyPlace(MAPSIZE - getPos().z - 1, (getPos().x - (i + 1)) + MAPSIZE / 2))
			{
				break;
			}
			if (getPos().x - (i + 1) < -MAPSIZE / 2)
			{
				break;
			}
			SceneMgr::GetInst()->GetCurScene()->AddEffect(GraphicsClass::GetInst()->GetD3D(), EFFECT_TYPE::FIRE, getPos() + Pos(-(i + 1), 0, 0));
		}		
		for (int i = 0; i < power; ++i)
		{
			if (!SceneMgr::GetInst()->GetCurScene()->isEmptyPlace(MAPSIZE - getPos().z - (i + 1) - 1, getPos().x + MAPSIZE / 2))
			{
				break;
			}
			if (MAPSIZE - getPos().z - (i + 1) - 1 < 0)
			{
				break;
			}
			SceneMgr::GetInst()->GetCurScene()->AddEffect(GraphicsClass::GetInst()->GetD3D(), EFFECT_TYPE::FIRE, getPos() + Pos(0, 0, (i + 1)));
		}		
		for (int i = 0; i < power; ++i)
		{
			if (!SceneMgr::GetInst()->GetCurScene()->isEmptyPlace(MAPSIZE - getPos().z + (i + 1) - 1, getPos().x + MAPSIZE / 2))
			{
				break;
			}
			if (MAPSIZE - getPos().z + (i + 1) > MAPSIZE)
			{
				break;
			}
			SceneMgr::GetInst()->GetCurScene()->AddEffect(GraphicsClass::GetInst()->GetD3D(), EFFECT_TYPE::FIRE, getPos() + Pos(0, 0, -(i + 1)));
		}

		SceneMgr::GetInst()->GetCurScene()->ClearObjects(GROUP_TYPE::BOMB);
	}
}
