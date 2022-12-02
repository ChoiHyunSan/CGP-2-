#pragma once
#include "Scene.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "lightshaderclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "d3dclass.h"
#include "systemclass.h"
#include  <assert.h>
#include "inputclass.h"
#include "SceneMgr.h"
#include "Player.h"
#include "Bomb.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "textureclass.h"
#include "GameMgr.h"
#include "Enemy.h"
#include "EffectClass.h"
#include "fireshaderclass.h"
#include "Fire.h"


void Scene::render(D3DClass* D3D, float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// effet를 위한 변수 선언
	XMFLOAT3 scrollSpeeds, scales;
	XMFLOAT2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	static float frameTime = 0.0f;

	bool result;
	// Clear the buffers to begin the scene.
	D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// 스카이 박스를 화면에 띄운다.
	m_Camera->GetViewMatrix(viewMatrix);
	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix *= XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	D3D->TurnOffCulling();

	// Turn off the Z buffer.
	D3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	if (m_SkyDome != nullptr && m_SkyDomeShader != nullptr)
	{
		m_SkyDome->Render(D3D->GetDeviceContext());
		m_SkyDomeShader->Render(D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	}
	// Turn back face culling back on.
	D3D->TurnOnCulling();

	// Turn the Z buffer back on.
	D3D->TurnZBufferOn();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);

	if (m_fixCamera)
		viewMatrix *= XMMatrixRotationX(-0.3f) * XMMatrixTranslation(0, -7, -1.5) * XMMatrixLookAtLH(m_Eye, m_At, m_Up);
	else
		viewMatrix *= XMMatrixTranslation(0, -7, -1.5) * XMMatrixLookAtLH(m_Eye, m_At, m_Up);

	for (int i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrModel[i].size(); j++)
		{
			if (m_arrModel[i][j]->IsDead()) continue;

			// Generate the view matrix based on the camera's position.
			m_Camera->Render();

			// Get the world, view, and projection matrices from the camera and d3d objects.

			D3D->GetWorldMatrix(worldMatrix);
			D3D->GetProjectionMatrix(projectionMatrix);

			// 월드 메트릭스 조절
			worldMatrix =
				m_arrModel[i][j]->getRotate() * m_arrModel[i][j]->getScale() *
				XMMatrixTranslation(m_arrModel[i][j]->getPos().x, m_arrModel[i][j]->getPos().y - 2, m_arrModel[i][j]->getPos().z);

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			m_arrModel[i][j]->Render(D3D->GetDeviceContext());

			// Render the model using the light shader.
			result = m_LightShader->Render(D3D->GetDeviceContext(), m_arrModel[i][j]->GetVertexCount(), m_arrModel[i][j]->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_arrModel[i][j]->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());


			m_arrModel[i][j]->finalUpdate();

			assert(result);
		}
	}

	// frameTime 카운터를 증가시킵니다.
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}
	// 세 노이즈 텍스쳐의 스크롤 속도를 설정합니다.
	scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// 세 크기값을 사용하여 세 가지의 다른 노이즈 옥타브 텍스쳐를 만듭니다.
	scales = XMFLOAT3(1.0f, 2.0f, 3.0f);

	// 세 노이즈 텍스쳐의 서로 다른 세 개의 x, y 왜곡 인자를 설정합니다.
	distortion1 = XMFLOAT2(0.1f, 0.2f);
	distortion2 = XMFLOAT2(0.1f, 0.3f);
	distortion3 = XMFLOAT2(0.1f, 0.1f);

	// 텍스쳐 샘플링 좌표의 교란을 위한 크기 및 바이어스 값입니다.
	distortionScale = 0.8f;
	distortionBias = 0.5f;

	D3D->TurnOnAlphaBlending();

	// 이펙트를 화면에 띄운다.
	for (int i = 0; i < (UINT)EFFECT_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrEffect[i].size(); ++j)
		{
			// 카메라의 위치에 기반하여 뷰 행렬을 생성합니다.
			m_Camera->Render();

			// 카메라와 d3d객체에서 월드, 뷰, 투영 행렬을 가져옵니다.
			D3D->GetWorldMatrix(worldMatrix);
			//m_Camera->GetViewMatrix(viewMatrix);
			D3D->GetProjectionMatrix(projectionMatrix);

			m_Effect->Render(D3D->GetDeviceContext());
			worldMatrix *= XMMatrixRotationX(3.141592 / 2) * XMMatrixTranslation(0.0f, -4.55f, 0.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(m_arrEffect[i][j]->getPos().x, m_arrEffect[i][j]->getPos().y - 2, m_arrEffect[i][j]->getPos().z);

			result = m_FireShader->Render(D3D->GetDeviceContext(), m_arrEffect[i][j]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_arrEffect[i][j]->GetTexture1(), m_arrEffect[i][j]->GetTexture2(), m_arrEffect[i][j]->GetTexture3(), frameTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);

			m_arrEffect[i][j]->finalUpdate();
		}
	}

	// 파티클을 화면에 띄운다.


	for (int i = 0; i < (UINT)PARTICLE_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrParticleSystem[i].size(); ++j)
		{
			// 카메라의 위치에 기반하여 뷰 행렬을 생성합니다.
			m_Camera->Render();

			// 카메라와 d3d객체에서 월드, 뷰, 투영 행렬을 가져옵니다.
			D3D->GetWorldMatrix(worldMatrix);
			D3D->GetProjectionMatrix(projectionMatrix);

			m_ParticleSystem->Render(D3D->GetDeviceContext());

			worldMatrix *= XMMatrixTranslation(m_arrParticleSystem[i][j]->getPos().x, m_arrParticleSystem[i][j]->getPos().y - 2, m_arrParticleSystem[i][j]->getPos().z);
			
			result = m_ParticleShader->Render(D3D->GetDeviceContext(), m_arrParticleSystem[i][j]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_arrParticleSystem[i][j]->GetTexture());
		}
	}

	// 알파 블렌딩을 끕니다.       
	D3D->TurnOffAlphaBlending();

	m_Camera->GetViewMatrix(viewMatrix);

	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);
	D3D->GetOrthoMatrix(orthoMatrix);
	D3D->TurnZBufferOff();

	if (SceneMgr::GetInst()->GetCurScene()->GetName() == L"Title Scene" 
		|| SceneMgr::GetInst()->GetCurScene()->GetName() == L"GameClear_Scene"
		|| SceneMgr::GetInst()->GetCurScene()->GetName() == L"GameOver_Scene")
	{
		result = m_BackGround->Render(D3D->GetDeviceContext(), 0, 0);
	}
	else
	{
		result = m_BackGround->Render(D3D->GetDeviceContext(), 0, 500);
	}

	m_TextureShader->Render(D3D->GetDeviceContext(), m_BackGround->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_BackGround->GetTexture());

	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);
	D3D->GetOrthoMatrix(orthoMatrix);

	D3D->TurnOnAlphaBlending();

	result = m_UiText->Render(D3D->GetDeviceContext(), worldMatrix, orthoMatrix);

	D3D->TurnOffAlphaBlending();

	D3D->TurnZBufferOn();

}

void Scene::update(D3DClass* D3D)
{
	bool result;

	// TODO : 오브젝트들을 업데이트 시킨다.
	if (InputClass::GetInst()->GetKeyState(DIK_NUMPAD0) & 0x80)
	{
		SceneMgr::GetInst()->setCurScene(SCENE_TYPE::START);
	}
	if (InputClass::GetInst()->GetKeyState(DIK_F1) & 0x80)
	{
		FixCamera();
	}

	GameMgr::GetInst()->update();
	m_UiText->update();

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for(int j = 0; j< m_arrModel[i].size(); j++)
		{
			if(!m_arrModel[i][j]->IsDead())
				m_arrModel[i][j]->Update();
		}

	}
	for (int i = 0; i < (UINT)EFFECT_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrEffect[i].size(); i++)
		{
			if (!m_arrEffect[i][j]->IsDead())
				m_arrEffect[i][j]->Update();
		}
	}
	for (int i = 0; i < (UINT)PARTICLE_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrParticleSystem[i].size(); i++)
		{
			m_arrParticleSystem[i][j]->Frame(SystemClass::GetInst()->GetTime(),D3D->GetDeviceContext());
			
		}
	}

	if(!m_fixCamera)
		updateCamera();
}

void Scene::finalUpdate(D3DClass* D3D)
{
}

void Scene::AddObject(D3DClass* D3D, GROUP_TYPE _eType, Pos pos)
{
	if(_eType == GROUP_TYPE::DEFAULT)
		m_Model = new ModelClass(D3D->GetDevice(), GROUP_TYPE::DEFAULT, pos);

	if (_eType == GROUP_TYPE::PLAYER)
		m_Model = new Player(D3D->GetDevice(), GROUP_TYPE::PLAYER, pos);

	if (_eType == GROUP_TYPE::BOMB)
		m_Model = new Bomb(D3D->GetDevice(), GROUP_TYPE::BOMB, pos);

	if (_eType == GROUP_TYPE::ENEMY)
		m_Model = new Enemy(D3D->GetDevice(), GROUP_TYPE::ENEMY, pos);

	m_arrModel[(UINT)_eType].push_back(m_Model);
}

void Scene::AddEffect(D3DClass* D3D, EFFECT_TYPE _eType, Pos pos)
{
	if (_eType == EFFECT_TYPE::FIRE)
	{
		m_Effect = new Fire(D3D->GetDevice(), pos);
		
	}
	m_arrEffect[(UINT)_eType].push_back(m_Effect);
}

void Scene::AddParticle(D3DClass* D3D, PARTICLE_TYPE _eType, Pos pos)
{
	if (_eType == PARTICLE_TYPE::DEFAULT)
	{
		m_ParticleSystem = new ParticleSystemClass(D3D->GetDevice(), L"./data/star.dds" ,pos);
	}
	m_arrParticleSystem[(UINT)_eType].push_back(m_ParticleSystem);
}

void Scene::ClearObjects(GROUP_TYPE _eType)
{
	m_arrModel[(UINT)_eType].clear();
}

void Scene::ClearEffects(EFFECT_TYPE _eType)
{
	m_arrEffect[(UINT)_eType].clear();
}

bool Scene::isModelAllDead(GROUP_TYPE type)
{
	for (const auto& index : m_arrModel[(UINT)type])
	{
		if (!index->IsDead())
			return false;
	}
	return true;
}

void Scene::updateCamera()
{bool result;
	bool isClip = true;
	static float rotation = 0.1f;

	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	DWORD dwCurTime = GetTickCount();
	static DWORD dwOldTime = GetTickCount();
	DWORD m_dwElapsedTime = dwCurTime - dwOldTime;
	dwOldTime = dwCurTime;

	float rotSpeed = 0.08f;
	float moveSpeed = 0.02f;

	if (GetAsyncKeyState(0x57))
	{
		XMVECTOR Direction;
		Direction = XMVector3Normalize(m_At - m_Eye);
		m_Eye += m_dwElapsedTime * Direction * moveSpeed;
		m_At += m_dwElapsedTime * Direction * moveSpeed;
	}

	if (GetAsyncKeyState(0x53)) 
	{
		XMVECTOR Direction;
		Direction = XMVector3Normalize(m_At - m_Eye);
		m_Eye -= m_dwElapsedTime * Direction * moveSpeed;
		m_At -= m_dwElapsedTime * Direction * moveSpeed;
	}

	if (GetAsyncKeyState(0x41))
	{
		XMVECTOR UpNormal, ForwardNormal, Direction;
		UpNormal = XMVector3Normalize(m_Up);
		ForwardNormal = XMVector3Normalize(m_At - m_Eye);
		Direction = XMVector3Cross(ForwardNormal, UpNormal);
		Direction = XMVector3Normalize(Direction);
		m_Eye += m_dwElapsedTime * Direction * moveSpeed;
		m_At += m_dwElapsedTime * Direction * moveSpeed;
	}

	if (GetAsyncKeyState(0x44)) 
	{
		XMVECTOR UpNormal, ForwardNormal, Direction;
		UpNormal = XMVector3Normalize(m_Up);
		ForwardNormal = XMVector3Normalize(m_At - m_Eye);
		Direction = XMVector3Cross(ForwardNormal, UpNormal);
		Direction = XMVector3Normalize(Direction);
		m_Eye -= m_dwElapsedTime * Direction * moveSpeed;
		m_At -= m_dwElapsedTime * Direction * moveSpeed;
	}

	POINT curPt;
	GetCursorPos(&curPt);
	DWORD _CurTime = GetTickCount();
	static DWORD _OldTime = GetTickCount();
	if (_CurTime - _OldTime > 500.f)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			if (isClip)
				isClip = false;
			else
				isClip = true;
			_OldTime = _CurTime;
		}
	}
	int screenWidth = 800;
	int screenHeight = 600;
	if (isClip) {
		SetCursorPos(screenWidth / 2, screenHeight / 2);
		if (curPt.y < screenHeight / 2)
		{
			XMVECTOR UpNormal;
			UpNormal = XMVector3Normalize(m_Up);
			m_At += m_dwElapsedTime * UpNormal * rotSpeed;
		}

		if (curPt.y > screenHeight / 2)
		{
			XMVECTOR UpNormal;
			UpNormal = XMVector3Normalize(m_Up);
			m_At -= m_dwElapsedTime * UpNormal * rotSpeed;
		}

		if (curPt.x < screenWidth / 2)
		{
			XMVECTOR UpNormal, ForwardNormal, Left;
			UpNormal = XMVector3Normalize(m_Up);
			ForwardNormal = XMVector3Normalize(m_At - m_Eye);
			Left = XMVector3Cross(ForwardNormal, UpNormal);
			Left = XMVector3Normalize(Left);
			m_At += m_dwElapsedTime * Left * rotSpeed;
		}

		if (curPt.x > screenWidth / 2)
		{
			XMVECTOR UpNormal, ForwardNormal, Right;
			UpNormal = XMVector3Normalize(m_Up);
			ForwardNormal = XMVector3Normalize(m_At - m_Eye);
			Right = XMVector3Cross(ForwardNormal, UpNormal);
			Right = XMVector3Normalize(Right);
			m_At -= m_dwElapsedTime * Right * rotSpeed;
		}
	}
}

void Scene::FixCamera()
{
	if (m_fixCamera)
	{
		m_fixCamera = false;
	}
	else
	{
		m_fixCamera = true;
		m_Eye = XMVectorSet(0.0f, 8.0f, -10.0f, 1.0f);
		m_At = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		m_Up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	}
	GameMgr::GetInst()->SetCameraMode(m_fixCamera);
}

void Scene::Reload()
{
	// 게임 정보값 초기화
	GameMgr::GetInst()->init();

	// 모든 오브젝트들을 삭제
	for (int i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrModel[i].size(); j++)
		{
			delete m_arrModel[i][j];
		}
		m_arrModel[i].clear();
	}
	

	for (int i = 0; i < (UINT)EFFECT_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrEffect[i].size(); j++)
		{
			delete m_arrEffect[i][j];
		}
		m_arrEffect[i].clear();
	}

	for (int i = 0; i < (UINT)PARTICLE_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrParticleSystem[i].size(); j++)
		{
			delete m_arrParticleSystem[i][j];
		}
		m_arrParticleSystem[i].clear();
	}

	// 오브젝트들을 다시 생성
	SetObjects(GraphicsClass::GetInst()->GetD3D());

}

Scene::Scene():
	m_Model(0),
	m_Camera(0),
	m_Light(0),
	m_LightShader(0),
	m_BackGround(0),
	m_TextureShader(0),
	m_UiText(0),
	m_Effect(0),
	m_FireShader(0),
	m_ParticleShader(0),
	m_fixCamera(true)
{
	m_Eye = XMVectorSet(0.0f, 8.0f, -10.0f, 1.0f);
	m_At = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	m_Up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
}

Scene::~Scene()
{
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the bitmap object.
	if (m_BackGround)
	{
		m_BackGround->Shutdown();
		delete m_BackGround;
		m_BackGround = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the Effect object.
	if (m_Effect)
	{
		m_Effect->Shutdown();
		delete m_Effect;
		m_Effect = 0;
	}
	// Release the fire shader object.
	if (m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	// Release the text object.
	if (m_UiText)
	{
		m_UiText->Shutdown();
		delete m_UiText;
		m_UiText = 0;
	}

	// 파티클 시스템 객체를 해제합니다.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// 파티클 셰이더 객체를 해제합니다.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	for (int i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrModel[i].size(); j++)
		{
			delete m_arrModel[i][j];
		}
	}

	for (int i = 0; i < (UINT)EFFECT_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrEffect[i].size(); j++)
		{
			delete m_arrEffect[i][j];
		}
	}

	for (int i = 0; i < (UINT)PARTICLE_TYPE::END; i++)
	{
		for (int j = 0; j < m_arrParticleSystem[i].size(); j++)
		{
			delete m_arrParticleSystem[i][j];
		}
	}
}
