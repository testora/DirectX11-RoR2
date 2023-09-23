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
	HRESULT												Initialize(COLLIDERDESC);
#ifdef _DEBUG
	virtual HRESULT										Render() override;
#endif

public:
	void												Tick_Transformation(_matrixf mWorld);

private:
	COLLIDERDESC										m_tColliderDesc{};

	shared_ptr<class CBounding>							m_pBounding;

#ifdef _DEBUG
	ComPtr<ID3D11InputLayout>							m_pInputLayout;

	shared_ptr<PrimitiveBatch<VertexPositionColor>>		m_pBatch;
	shared_ptr<BasicEffect>								m_pEffect;
#endif

public:
	static shared_ptr<CCollider>						Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>						Clone(any _tColliderDesc) override;
};

END
