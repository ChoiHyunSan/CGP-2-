////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "CollisionMgr.h"
#include "EventMgr.h"
GraphicsClass::GraphicsClass(): 
	m_D3D(0)
{

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{

}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// 씬 매니저 초기화
	SceneMgr::GetInst()->init();

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame(int mouseX, int mouseY)
{
	bool result;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	// 새로운 씬을 생성

	SceneMgr::GetInst()->update(m_D3D);

	SceneMgr::GetInst()->render(m_D3D, rotation);

	CollisionMgr::GetInst()->update();
	
	EventMgr::GetInst()->update();

	SceneMgr::GetInst()->finalUpdate(m_D3D);

	m_D3D->EndScene();

	return true;
}