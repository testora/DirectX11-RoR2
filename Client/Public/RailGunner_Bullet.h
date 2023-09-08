#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CRailGunner_Bullet abstract : public CGameObject
{
protected:
	explicit CRailGunner_Bullet(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRailGunner_Bullet(const CRailGunner_Bullet&);
	virtual ~CRailGunner_Bullet() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

	virtual HRESULT							Fetch(any = g_aNull) override;

public:
	void									Shoot();

protected:
	shared_ptr<CTexture>					m_pTexBullet;
};

END
