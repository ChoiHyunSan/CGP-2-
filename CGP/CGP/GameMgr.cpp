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
	// GameTimer 돌리기
	if (m_timer >= 1.f)
	{
		m_playTime -= 1;
		m_timer = 0.f;
	}
	else
		m_timer += fDT;

	// 플레이어의 라이프 확인 (게임 오버 조건)


	// 적 오브젝트들의 개수 확인 (스테이지 클리어 조건)

}

void GameMgr::init()
{
	// 값들을 초기화 한다.
	m_score = 0;
	m_playTime = 100;
	m_playerLife = 2;
	m_playerPower = 2;
	m_timer = 0;
	m_CameraMode = true;
	m_eGameState = GAME_STATE::GAME_PLAY;
}
