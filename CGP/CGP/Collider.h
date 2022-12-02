#pragma once
#include "pch.h"

class ModelClass;
class EffectClass;

class Collider
{
private:
	static UINT g_iNextID;		//

	ModelClass* m_pOwnerModel;		// collider�� �����ϰ� �ִ� ������Ʈ
	EffectClass* m_pOwnerEffect;	// collider�� �����ϰ� �ִ� ����Ʈ

	Pos			m_vOffsetPos;	// ������Ʈ�κ��� ������� ��ġ 
	Pos			m_vFinalPos;	// finalupdate���� �� �����Ӹ��� ���
	Pos			m_vScale;		// �浹ü�� ũ��

	UINT		m_iID;			// �浹ü ������ ID��
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
	// �浹 ���� �Լ�
	void OnCollision(Collider* _pOther); // �浹 ���� ��� ȣ��Ǵ� �Լ�
	void OnCollisionEnter(Collider* _pOther);
	void OnCollisionExit(Collider* _pOther);

	// �ݶ��̴� ���� ������ �����Ѵ�.
	Collider& operator= (Collider& _origin) = delete;

public:

	Collider();
	Collider(const Collider& _origin);

	~Collider();

	friend class ModelClass;
	friend class EffectClass;
};

