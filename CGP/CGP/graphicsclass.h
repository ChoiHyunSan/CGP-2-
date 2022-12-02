////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "pch.h"
#include "Scene.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;  // 수직 동기화
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
	SINGLE(GraphicsClass)
public:
	GraphicsClass(const GraphicsClass&);

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int);

	D3DClass* GetD3D() const { return m_D3D; } 
private:
	bool Render(float);

private:
	D3DClass* m_D3D;

};

#endif