#include "EventMgr.h"
#include "pch.h"
#include "ModelClass.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "graphicsClass.h"
EventMgr::EventMgr()
{

}

EventMgr::~EventMgr()
{

}

void EventMgr::update()
{
	// 이전 프레임에서 등록해둔 Dead Object들을 삭제한다.
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();

	// ===========
	// Event 처리
	// ===========
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Excute(m_vecEvent[i]);
	}

	m_vecEvent.clear();
}

void EventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEven)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : Object Adress
		// wParam : Group Type
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;

		SceneMgr::GetInst()->GetCurScene()->AddObject(GraphicsClass::GetInst()->GetD3D(), eType, Pos(0,0,0));

	}
	break;

	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : Object Adress
		// Object를 Dead상태로 변경
		// 삭제예정 오브젝트들을 모아둔다.

		ModelClass* pDeadObj = (ModelClass*)_eve.lParam;

		if (!pDeadObj->IsDead())
		{
			pDeadObj->SetDead();
			m_vecDead.push_back(pDeadObj);
		}
	}


	break;

	case EVENT_TYPE::SCENE_CHANGE:



		break;

	}
}
