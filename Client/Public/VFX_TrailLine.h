#pragma once
#include "Client_Define.h"
#include "Effect.h"

BEGIN(Client)

class CVFX_TrailLine final : public CEffect
{
private:
	explicit CVFX_TrailLine(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVFX_TrailLine(const CVFX_TrailLine&);
	virtual ~CVFX_TrailLine() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Fetch(any pair_pTarget_szBone) override;
	virtual _bool						Return() override;

protected:
	virtual	void						Fetch_Instance(void*, _uint iNumInstance, any = g_aNull) override;
	virtual	void						Update_Instance(void*, _uint iNumInstance, _float fTimeDelta) override;

public:
	HRESULT								Set_Texture(shared_ptr<CTexture> pTexture);
	HRESULT								Set_Texture(const wstring& wstrTexture);
	void								Set_MaxInstance(_uint iMaxInstance)			{ m_iMaxInstance = iMaxInstance; }
	void								Set_Interval(_float fInterval)				{ m_fInterval = fInterval; }
	void								Set_Color(_color vColor)					{ m_vColor = vColor; }
	void								Set_Thickness(_float fThickness)			{ m_fThickness = fThickness; }

private:
	shared_ptr<CTransform>				m_pTargetTransform;

	const _float4x4*					m_pTargetPoint	= nullptr;
	_float4x4							m_mTargetPivot;

	_float								m_fTimeAcc		= 0.f;
	_uint								m_iIndex		= 0;

	deque<_float4>						m_deqLine;

	_uint								m_iMaxInstance	= 100;
	_float								m_fInterval		= 0.01f;
	_color								m_vColor		= _color(1.f, 1.f, 1.f, 1.f);
	_float								m_fThickness	= 0.05f;

public:
	static shared_ptr<CVFX_TrailLine>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
