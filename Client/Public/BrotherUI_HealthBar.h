#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Client)

class CBrotherUI_HealthBar final : public CGameObject
{
private:
	explicit CBrotherUI_HealthBar(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CBrotherUI_HealthBar() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	virtual HRESULT								Ready_Components() override;
	virtual HRESULT								Ready_Behaviors() override;

public:
	static shared_ptr<CBrotherUI_HealthBar>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
