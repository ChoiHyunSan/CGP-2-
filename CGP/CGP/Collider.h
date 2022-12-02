#pragma once
#include "pch.h"

class ModelClass;
class EffectClass;

class Collider
{
private:
	static UINT g_iNextID;		//

	ModelClass* m_pOwnerModel;		// collider를 소유하고 있는 오브젝트
	EffectClass* m_pOwnerEffect;	// collider를 소유하고 있는 이펙트

	Pos			m_vOffsetPos;	// 오브젝트로부터 상대적인 위치 
	Pos			m_vFinalPos;	// finalupdate에서 매 프레임마다 계산
	Pos			m_vScale;		// 충돌체의 크기

	UINT		m_iID;			// 충돌체 고유한 ID값
	UINT		m_iCol;
public:
	void SetOffsetPos(Pos _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Pos _vScale) { m_vScale = _vScale; }

	Pos GetOffsetPos() { return m_vOffsetPos; }
	Pos GetScale() { return m_vScale; }

	Pos GetFinalPos() { return m_vFinalPos; }

	UINT GetID() { return m_iID; }

	ModelClass* GetModel() { return m_pOwnerModel; }
	EffectClass* GetEffect() { return m_pOwnerEffect; }

public:
	void finalUpdate();
	void render();

public:
	// 충돌 시점 함수
	void OnCollision(Collider* _pOther); // 충돌 중인 경우 호출되는 함수
	void OnCollisionEnter(Collider* _pOther);
	void OnCollisionExit(Collider* _pOther);

	// 콜라이더 간의 대입을 방지한다.
	Collider& operator= (Collider& _origin) = delete;

public:

	Collider();
	Collider(const Collider& _origin);

	~Collider();

	friend class ModelClass;
	friend class EffectClass;
};

