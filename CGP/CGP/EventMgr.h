#pragma once
#include "pch.h"
#include "ModelClass.h"

// DWORD_PTR : 플랫폼에 따라서 바이트 수를 변환해준다.
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

