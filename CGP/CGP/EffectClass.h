////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _EFFECTCLASS_H_
#define _EFFECTCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include "pch.h"

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include <fstream>
using namespace std;

#include "CollisionMgr.h"
#include "Collider.h"

class EffectClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	virtual void Update();

public:
	EffectClass(ID3D11Device* device, EFFECT_TYPE _eType, Pos pos);
	EffectClass(const EffectClass&);
	~EffectClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	Pos getPos()const { return m_pos; }
	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();

	// 콜라이더로부터 호출받은 함수를 다형성으로 처리한다.
	virtual void OnCollision(Collider* _pOther);
	virtual void OnCollisionEnter(Collider* _pOther);
	virtual void OnCollisionExit(Collider* _pOther);

	bool IsDead(){ return !m_bAlive; }

	Collider* GetCollider() { return m_pCollider; }

	Collider* GetCollider() const { return m_pCollider; }
	virtual void finalUpdate() final;
	void CreateCollider();

	const wstring& GetName() { return m_strName; }
	void SetName(const wstring& str) { m_strName = str; }
	void SetDead() { m_bAlive = false; }
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*);
	void ReleaseTextures();

	bool LoadModel(const WCHAR*);
	void ReleaseModel();

	void setFileInfo();
private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture1, * m_Texture2, * m_Texture3;
	ModelType* m_model;

	EFFECT_TYPE m_type;
	Pos		m_pos;

	WCHAR* m_modelFilename;
	WCHAR* m_textureFilename1;
	WCHAR* m_textureFilename2;
	WCHAR* m_textureFilename3;


	bool m_bAlive = true;

	Collider* m_pCollider;

	wstring m_strName;

	friend class EventMgr;

};
#endif