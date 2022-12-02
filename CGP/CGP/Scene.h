#pragma once
#include "pch.h"
#include "modelclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "textureclass.h"
#include "textclass.h"
#include "EffectClass.h"
#include "fireshaderclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"
#include "skydomeclass.h"
#include "skydomeshaderclass.h"

class ModelClass;
class CameraClass;
class LightClass;
class LightShaderClass;
class D3DClass;
class EffectClass;

class Scene
{
private:
	vector<ModelClass*> m_arrModel[(UINT)GROUP_TYPE::END];		// 오브젝트를 저장 및 관리할 벡터를 그룹만큼 선언
	vector<EffectClass*> m_arrEffect[(UINT)EFFECT_TYPE::END];	// 오브젝트를 저장 및 관리할 벡터를 그룹만큼 선언
	wstring				m_strName;								// Scene 이름
	bool				m_fixCamera;
protected:
	ModelClass* m_Model;
	EffectClass* m_Effect;
	ParticleSystemClass* m_ParticleSystem;

	CameraClass* m_Camera;
	LightClass* m_Light;
	LightShaderClass* m_LightShader;
	FireShaderClass* m_FireShader;

	TextureShaderClass* m_TextureShader;
	BitmapClass*	m_BackGround;
	TextClass*		m_UiText;

	ParticleShaderClass* m_ParticleShader;
	vector<ParticleSystemClass*> m_arrParticleSystem[(UINT)PARTICLE_TYPE::END];

	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;

	XMFLOAT3 cameraPosition; 
public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	ModelClass* GetModel() { return m_Model; }

	void render(D3DClass* D3D, float rotation);
	void update(D3DClass* D3D);
	virtual void finalUpdate(D3DClass* D3D);

	void AddObject(D3DClass* D3D, GROUP_TYPE _eType, Pos pos = Pos());
	void AddEffect(D3DClass* D3D, EFFECT_TYPE _eType, Pos pos);
	void AddParticle(D3DClass* D3D, PARTICLE_TYPE _eType, Pos pos);

	void ClearObjects(GROUP_TYPE _eType);
	void ClearEffects(EFFECT_TYPE _eType);

	virtual void init(D3DClass* D3D) = 0;	// 해당 Scene 에 진입 시 호출
	virtual void Exit() = 0;				// 해당 Scene 을 탈출 시 호출

	Pos getPlayerPos() { return m_arrModel[(UINT)GROUP_TYPE::PLAYER][0]->getPos(); }
	Pos getBombPos() { return m_arrModel[(UINT)GROUP_TYPE::BOMB][0]->getPos(); }

	bool isModelArrEmpty(GROUP_TYPE type) { return m_arrModel[(UINT)type].size() == 0; }
	bool isModelAllDead(GROUP_TYPE type);

	vector<ModelClass*> GetGroupObject(GROUP_TYPE eType) { return m_arrModel[(UINT)eType]; }
	vector<EffectClass*> GetGroupEffect(EFFECT_TYPE eType) { return m_arrEffect[(UINT)eType]; }

	bool isEmptyPlace(int x, int z) { return m_map[x][z] == 0; }

	void updateCamera();
	void FixCamera();

	void Reload();
	virtual void SetObjects(D3DClass* D3D) = 0;

private:


protected:
	XMMATRIX m_baseViewMatrix;
	char m_map[MAPSIZE][MAPSIZE];

	XMVECTOR   m_Eye;					// 카메라 위치   (위치)
	XMVECTOR   m_At;					// 카메라가 보는 곳 (위치)
	XMVECTOR   m_Up;                    // 카메라의 기준 축 ( 축이 위로 향하는 카메라 방향 )

public:
	Scene();
	virtual ~Scene();

};

