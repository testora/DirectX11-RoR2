#pragma once
#include "Client_Define.h"
#include "RailGunner_Bullet.h"

BEGIN(Client)

class CRailGunner_PistolBullet final : public CRailGunner_Bullet
{
private:
	explicit CRailGunner_PistolBullet(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRailGunner_PistolBullet(const CRailGunner_PistolBullet&);
	virtual ~CRailGunner_PistolBullet() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

	virtual HRESULT								Fetch(any = g_aNull) override;

public:
	static shared_ptr<CRailGunner_PistolBullet>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
