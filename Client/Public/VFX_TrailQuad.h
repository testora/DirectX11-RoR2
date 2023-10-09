#pragma once
#include "Client_Define.h"
#include "Effect.h"

BEGIN(Client)

class CVFX_TrailQuad final : public CEffect
{
private:
	explicit CVFX_TrailQuad(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVFX_TrailQuad(const CVFX_TrailQuad&);
	virtual ~CVFX_TrailQuad() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Fetch(any = g_aNull) override;
	virtual _bool						Return() override;

protected:
	virtual	void						Fetch_Instance(void*, _uint iNumInstance, any = g_aNull) override;
	virtual	void						Update_Instance(void*, _uint iNumInstance, _float fTimeDelta) override;

private:
	_float								m_fTimeAcc	= 0.f;

	_uint								m_iIndex	= 0;

	deque<_float3>						m_deqDown;
	deque<_float3>						m_deqUp;

public:
	static shared_ptr<CVFX_TrailQuad>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
