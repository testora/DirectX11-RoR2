#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
private:
	explicit CCollider(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCollider(const CCollider&);
	virtual ~CCollider() DEFAULT;

public:
	HRESULT												Initialize_Prototype();
	HRESULT												Initialize(shared_ptr<class CGameObject>, COLLIDERDESC);
	void												Tick(_float fTimeDelta);
#ifdef _DEBUG
	virtual HRESULT										Render() override;
#endif

public:
	const _uint											Get_ID() const							{ return m_iColID; }
	_bool												Intersect(shared_ptr<CCollider>);

public:

	void												OnCollisionEnter(shared_ptr<class CGameObject>, _float fTimeDelta);
	void												OnCollision(shared_ptr<class CGameObject>, _float fTimeDelta);
	void												OnCollisionExit(shared_ptr<class CGameObject>, _float fTimeDelta);

private:
	weak_ptr<class CGameObject>							m_pOwner;
	weak_ptr<class CTransform>							m_pTransform;

	COLLIDERDESC										m_tColliderDesc{};

	shared_ptr<class IBounding>							m_pBounding;

	_bool												m_bCollision	= false;
	_uint												m_iColCnt		= 0;

	static _uint										s_iColID;
	const _uint											m_iColID;

#ifdef _DEBUG
	ComPtr<ID3D11InputLayout>							m_pInputLayout;

	shared_ptr<PrimitiveBatch<VertexPositionColor>>		m_pBatch;
	shared_ptr<BasicEffect>								m_pEffect;
#endif

public:
	static shared_ptr<CCollider>						Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>						Clone(any _pairObject_Desc) override;
};

END
