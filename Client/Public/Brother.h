#pragma once
#include "Client_Define.h"
#include "Monster.h"

BEGIN(Client)

class CBrother final : public CMonster
{
private:
	explicit CBrother(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CBrother(const CBrother&);
	virtual ~CBrother() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

	virtual HRESULT							Render_ShadowDepth() override;

private:
	virtual HRESULT							Fetch(any vPosition3 = g_aNull) override;
	virtual _bool							Return() override;

	virtual void							OnCollisionEnter(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta) override;
	virtual void							OnCollision(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta) override;
	virtual void							OnCollisionExit(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta) override;

	virtual HRESULT							Ready_Components() override;
	virtual HRESULT							Ready_Behaviors() override;

public:
	void									Set_Render(_bool bEnable = true)	{ m_bRender = bEnable; }

public:
	virtual void							Hit() override;
	virtual void							Hit_WeakPoint() override;

private:
	shared_ptr<class CBrotherUI_HealthBar>	m_pHealthBar;

	shared_ptr<class CVFX_TrailLine>		m_pVFX_EyeTrail;
	shared_ptr<class CVFX_TrailQuad>		m_pVFX_HammerTrail;
	
	_bool									m_bRender	= true;

public:
	static shared_ptr<CBrother>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END
