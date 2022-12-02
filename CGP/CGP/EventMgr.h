#pragma once
#include "pch.h"
#include "ModelClass.h"

// DWORD_PTR : �÷����� ���� ����Ʈ ���� ��ȯ���ش�.
struct tEvent
{
	EVENT_TYPE  eEven;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};

class EventMgr
{
	SINGLE(EventMgr)
private:
	vector<tEvent> m_vecEvent;

	vector<ModelClass*> m_vecDead;

public:
	void update();

private:
	void Excute(const tEvent& _eve);

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }

};

