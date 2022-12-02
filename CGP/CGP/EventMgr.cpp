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
	// ���� �����ӿ��� ����ص� Dead Object���� �����Ѵ�.
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();

	// ===========
	// Event ó��
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
		// Object�� Dead���·� ����
		// �������� ������Ʈ���� ��Ƶд�.

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
