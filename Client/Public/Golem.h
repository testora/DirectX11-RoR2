#pragma once
#include "Client_Define.h"
#include "Monster.h"

BEGIN(Client)

class CGolem final : public CMonster
{
private:
	explicit CGolem(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGolem(const CGolem&);
	virtual ~CGolem() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_ShadowDepth() override;

private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

public:
//	virtual void						Wander() override;
	
	virtual void						Hit() override;
	virtual void						Hit_WeakPoint() override;

public:
	static shared_ptr<CGolem>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
