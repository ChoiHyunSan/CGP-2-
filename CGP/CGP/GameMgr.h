#pragma once
#include "pch.h"



class GameMgr
{
	SINGLE(GameMgr)

public:
	void	update();

public:
	int		getPlayerLife() const { return m_playerLife; }
	int		getPlayerPower() const { return m_playerPower; }
	void	setPlayerLife(int playerLife) { m_playerLife = playerLife; };

	int		getPlayTime() const { return m_playTime; }
	int		getPlayScore() const { return m_score; }
	void	AddScore() { m_score += 100; }

	void	AddLife(const int value) { m_playerLife += value; }
	void    SetCameraMode(const bool state) { m_CameraMode = state; }
	bool	GetCameraMode() const { return m_CameraMode; }

	GAME_STATE GetGameState() { return m_eGameState; }
	void	SetGameState(GAME_STATE eState) { m_eGameState = eState; }

	void	init();

private:
	int		m_score;
	int		m_playTime;
	int		m_playerLife;
	int		m_playerPower;
	float	m_timer;
	
	bool    m_CameraMode;

	GAME_STATE m_eGameState;

};

