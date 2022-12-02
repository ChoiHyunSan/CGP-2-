#include "Stage01_Scene.h"
#include "modelclass.h"
#include "d3dclass.h"
#include "systemclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textureclass.h"
#include "CollisionMgr.h"
#include "GameMgr.h"
#include "SceneMgr.h"

Stage01_Scene::Stage01_Scene()
{

}

Stage01_Scene::~Stage01_Scene()
{

}

void Stage01_Scene::init(D3DClass* D3D)
{
	bool result;
	
	SetObjects(D3D);

	// ����Ʈ ȿ�� �߰�
	if (m_Light == nullptr)
	{
		// Create the light object.
		m_Light = new LightClass;

		assert(m_Light);

		// Initialize the light object.
		m_Light->SetAmbientColor(0.8f, 0.8f, 0.8f, 1.0f);
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetDirection(0.0f, 0.5f, 0.5f);
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(32.0f);
	}

	if (m_LightShader == nullptr)
	{
		// Create the light shader object.
		m_LightShader = new LightShaderClass;

		// Initialize the light shader object.
		result = m_LightShader->Initialize(D3D->GetDevice(), SystemClass::GetInst()->GetHwnd());

		assert(result);
	}

	// Create the texture shader object.
	if (m_TextureShader == nullptr)
	{
		m_TextureShader = new TextureShaderClass;

		// Initialize the texture shader object.
		result = m_TextureShader->Initialize(D3D->GetDevice(), SystemClass::GetInst()->GetHwnd());
		if (!result)
		{
			MessageBox(SystemClass::GetInst()->GetHwnd(), L"Could not initialize the texture shader object.", L"Error", MB_OK);
		}
	}

	// Create the bitmap object.
	if (m_BackGround == nullptr)
	{
		m_BackGround = new BitmapClass;

		// Initialize the bitmap object.
		result = m_BackGround->Initialize(D3D->GetDevice(), 800, 600, L"./data/UI.dds", 800, 100);
		if (!result)
		{
			MessageBox(SystemClass::GetInst()->GetHwnd(), L"Could not initialize the bitmap object.", L"Error", MB_OK);
		}
	}

	if (m_Camera == nullptr)
	{
		m_Camera = new CameraClass;

		// Set the initial position of the camera.
		m_Camera->SetPosition(0.0f, 0.0f, -5.0f);	// for cube
	}

	// Create the text object.
	if (m_UiText == nullptr)
	{
		m_Camera->Render();
		m_Camera->GetViewMatrix(m_baseViewMatrix);

		m_UiText = new TextClass;

		// Initialize the text object.
		result = m_UiText->Initialize(D3D->GetDevice(), D3D->GetDeviceContext(), SystemClass::GetInst()->GetHwnd(), 800, 600, m_baseViewMatrix);
		if (!result)
		{
			MessageBox(SystemClass::GetInst()->GetHwnd(), L"Could not initialize the text object.", L"Error", MB_OK);
		}
	}

	if (m_FireShader == nullptr)
	{
		m_FireShader = new FireShaderClass;

		// �Ҳ� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
		result = m_FireShader->Initialize(D3D->GetDevice(), SystemClass::GetInst()->GetHwnd());
		if (!result)
		{
			MessageBox(SystemClass::GetInst()->GetHwnd(), L"Could not initialize the fire shader object.", L"Error", MB_OK);
		}
	}


	if (m_ParticleShader == nullptr)
	{
		m_ParticleShader = new ParticleShaderClass;

		// ��ƼŬ ���̴� ��ü�� �ʱ�ȭ�մϴ�.
		result = m_ParticleShader->Initialize(D3D->GetDevice(), SystemClass::GetInst()->GetHwnd());
		if (!result)
		{
			MessageBox(SystemClass::GetInst()->GetHwnd(), L"Could not initialize the particle shader object.", L"Error", MB_OK);
		}

	}

	if (m_SkyDome == nullptr)
	{
		m_SkyDome = new SkyDomeClass;

		result = m_SkyDome->Initialize(D3D->GetDevice());
		if (!result)
		{
			MessageBox(SystemClass::GetInst()->GetHwnd(), L"Could not initialize the sky dome object.", L"Error", MB_OK);
		}
	}


	if (m_SkyDomeShader == nullptr)
	{
		m_SkyDomeShader = new SkyDomeShaderClass;

		result = m_SkyDomeShader->Initialize(D3D->GetDevice(), SystemClass::GetInst()->GetHwnd());
		if (!result)
		{
			MessageBox(SystemClass::GetInst()->GetHwnd(), L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		}
	}
	cameraPosition = m_Camera->GetPosition();

	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::ENEMY);
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::FIRE);
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::ENEMY, GROUP_TYPE::FIRE);

	// ������ �浹ó��
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DEFAULT);
}

void Stage01_Scene::Exit()
{

}

void Stage01_Scene::finalUpdate(D3DClass* D3D)
{
	if (GameMgr::GetInst()->GetGameState() == GAME_STATE::GAME_CLEAR)
	{
		SceneMgr::GetInst()->setCurScene(SCENE_TYPE::GAME_CLEAR);
	}
	else if (GameMgr::GetInst()->GetGameState() == GAME_STATE::GAME_OVER)
	{
		SceneMgr::GetInst()->setCurScene(SCENE_TYPE::GAME_OVER);
	}

}

void Stage01_Scene::SetObjects(D3DClass* D3D)
{
	char map[MAPSIZE][MAPSIZE] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0,0,0,0,0,1,0,0},
	{0,0,1,0,1,0,0,0,0,1,0,0,1,0,0},
	{0,0,0,0,1,0,0,0,0,1,1,1,1,0,0},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,1,1,0,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			m_map[i][j] = map[i][j];
		}
	}

	// �𵨵����� ���� ��, ���Ϳ� �߰�

	// TODO : Ÿ��Ʋ ������ ������ �������� ���Ϳ� �߰�
	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			// Map[i][j]�� ���� 0�� ��쿡�� ��ġ
			AddObject(D3D, GROUP_TYPE::DEFAULT, Pos(i - (MAPSIZE / 2), 0, MAPSIZE - j - 1));
			if (map[j][i] == 1)
				AddObject(D3D, GROUP_TYPE::DEFAULT, Pos(i - (MAPSIZE / 2), 1, MAPSIZE - j - 1));
		}
	}

	AddObject(D3D, GROUP_TYPE::PLAYER, Pos(0, 0, 0));
	AddObject(D3D, GROUP_TYPE::ENEMY, Pos(5, 0, 0));
	AddObject(D3D, GROUP_TYPE::ENEMY, Pos(-5, 0, 0));
	AddObject(D3D, GROUP_TYPE::ENEMY, Pos(8, 0, 0));
	AddParticle(D3D, PARTICLE_TYPE::DEFAULT, Pos(10, 1, 0));
}
