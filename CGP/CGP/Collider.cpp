#include "Collider.h"
#include "pch.h"
#include "ModelClass.h"
#include "EffectClass.h"

UINT Collider::g_iNextID = 0;

Collider::Collider()
	:m_pOwnerModel(nullptr),
	m_pOwnerEffect(nullptr)
	, m_iID(g_iNextID++)
	, m_iCol(0)
{
}

Collider::Collider(const Collider& _origin)
	:m_pOwnerModel(nullptr), m_pOwnerEffect(nullptr),
	m_vOffsetPos(_origin.m_vOffsetPos),
	m_vScale(_origin.m_vScale),
	m_iID(g_iNextID++),
	m_iCol(0)
{

}

Collider::~Collider()
{
	//if (m_pOwnerModel != nullptr)
	//	delete m_pOwnerModel;

	//if (m_pOwnerEffect != nullptr)
	//	delete m_pOwnerEffect;
}

void Collider::finalUpdate()
{
	// Object의 위치를 따라간다.
	if (m_pOwnerModel != nullptr)
	{
		Pos vObjectPos = m_pOwnerModel->getPos();
		m_vFinalPos = vObjectPos + m_vOffsetPos;
	}
	else
	{
		Pos vEffectPos = m_pOwnerEffect->getPos();
		m_vFinalPos = vEffectPos + m_vOffsetPos;
	}

}

void Collider::render()
{

}

void Collider::OnCollision(Collider* _pOther)
{
	// 충돌한 콜라이더를 소유한 오브젝트로부터 OnCollision함수 호출
	if (m_pOwnerModel != nullptr)
		m_pOwnerModel->OnCollision(_pOther);
	else
		m_pOwnerEffect->OnCollision(_pOther);
}

void Collider::OnCollisionEnter(Collider* _pOther)
{
	++m_iCol;
	if (m_pOwnerModel != nullptr)
		m_pOwnerModel->OnCollisionEnter(_pOther);
	else
		m_pOwnerEffect->OnCollisionEnter(_pOther);
}

void Collider::OnCollisionExit(Collider* _pOther)
{
	--m_iCol;
	if (m_pOwnerModel != nullptr)
		m_pOwnerModel->OnCollisionExit(_pOther);
	else
		m_pOwnerEffect->OnCollisionExit(_pOther);
}
