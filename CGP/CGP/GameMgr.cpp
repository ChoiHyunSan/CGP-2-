#include "GameMgr.h"
#include "pch.h"
#include "TimeMgr.h"

GameMgr::GameMgr():
	m_score(0),
	m_playTime(100),
	m_playerLife(2),
	m_playerPower(2),
	m_timer(0),
	m_CameraMode(true),
	m_eGameState(GAME_STATE::GAME_PLAY)
{

}

GameMgr::~GameMgr()
{

}

void GameMgr::update()
{
	// GameTimer ������
	if (m_timer >= 1.f)
	{
		m_playTime -= 1;
		m_timer = 0.f;
	}
	else
		m_timer += fDT;

	// �÷��̾��� ������ Ȯ�� (���� ���� ����)


	// �� ������Ʈ���� ���� Ȯ�� (�������� Ŭ���� ����)

}

void GameMgr::init()
{
	// ������ �ʱ�ȭ �Ѵ�.
	m_score = 0;
	m_playTime = 100;
	m_playerLife = 2;
	m_playerPower = 2;
	m_timer = 0;
	m_CameraMode = true;
	m_eGameState = GAME_STATE::GAME_PLAY;
}
