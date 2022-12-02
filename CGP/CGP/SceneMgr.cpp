#include "SceneMgr.h"
#include "pch.h"
#include "Scene.h"
#include "Title_Scene.h"
#include "graphicsclass.h"
#include "Stage01_Scene.h"
#include "GameClear_Scene.h"
#include "GameOver_Scene.h"

void SceneMgr::init()
{
	// TODO : 만들고 싶은 씬 추가하기
	m_arrScene[(UINT)SCENE_TYPE::START] = new Title_Scene;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Title Scene");

	m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new Stage01_Scene;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01]->SetName(L"Stage01_Scene");

	m_arrScene[(UINT)SCENE_TYPE::GAME_CLEAR] = new GameClear_Scene;
	m_arrScene[(UINT)SCENE_TYPE::GAME_CLEAR]->SetName(L"GameClear_Scene");

	m_arrScene[(UINT)SCENE_TYPE::GAME_OVER] = new GameOver_Scene;
	m_arrScene[(UINT)SCENE_TYPE::GAME_OVER]->SetName(L"GameOver_Scene");

	// 만들어진 모든 씬에 대해서 초기화를 진행한다.
	for (const auto& Scene : m_arrScene)
	{
		if (Scene != nullptr)
		{
			Scene->init(GraphicsClass::GetInst()->GetD3D());
		}
	}

	// 초기 씬 지정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];
}

void SceneMgr::update(D3DClass* D3D)
{
	m_pCurScene->update(D3D);
}

void SceneMgr::finalUpdate(D3DClass* D3D)
{
	m_pCurScene->finalUpdate(D3D);
}

void SceneMgr::render(D3DClass* D3D, float rotation)
{
	m_pCurScene->render(D3D , rotation);
}

void SceneMgr::setCurScene(SCENE_TYPE _eType)
{
	m_pCurScene = m_arrScene[(UINT)_eType];
}

void SceneMgr::Reload()
{
	for (const auto& Scene : m_arrScene)
	{
		if (Scene != nullptr)
		{
			Scene->Reload();
		}
	}
}

SceneMgr::SceneMgr()
	: m_arrScene{}, m_pCurScene(nullptr)
{

}

SceneMgr::~SceneMgr()
{
	for (UINT i = 0; i < (UINT)SCENE_TYPE::START; ++i)
	{
		if (m_arrScene[i] != nullptr)
		{
			delete m_arrScene[i];
		}
	}
}