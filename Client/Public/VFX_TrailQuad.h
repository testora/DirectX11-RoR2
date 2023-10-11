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
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

	virtual HRESULT								Fetch(any pair_pTarget_pair_szBones) override;
	virtual _bool								Return() override;

protected:
	virtual	void								Fetch_Instance(void*, _uint iNumInstance, any = g_aNull) override;
	virtual	void								Update_Instance(void*, _uint iNumInstance, _float fTimeDelta) override;

public:
	HRESULT										Set_Texture(shared_ptr<CTexture> pTexture);
	HRESULT										Set_Texture(const wstring& wstrTexture);
	void										Set_MaxInstance(_uint iMaxInstance)				{ m_iMaxInstance = iMaxInstance; }
	void										Set_Interval(_float fInterval)					{ m_fInterval = fInterval; }
	void										Set_Color(_color vColor)						{ m_vColor = vColor; }
	void										Set_RelativeLength(_float fStart, _float fEnd)	{ m_pairRelativeLength = make_pair(fStart, fEnd); }

private:
	shared_ptr<CTransform>						m_pTargetTransform;

	pair<const _float4x4*, const _float4x4*>	m_pairTargetPoint		= make_pair(nullptr, nullptr);
	_float4x4									m_mTargetPivot;

	_float										m_fTimeAcc				= 0.f;
	_uint										m_iIndex				= 0;

	deque<pair<_float4, _float4>>				m_deqQuad;

	_uint										m_iMaxInstance			= 100;
	_float										m_fInterval				= 0.01f;
	_color										m_vColor				= _color(1.f, 1.f, 1.f, 1.f);
	pair<_float, _float>						m_pairRelativeLength	= make_pair(0.f, 0.f);

public:
	static shared_ptr<CVFX_TrailQuad>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
