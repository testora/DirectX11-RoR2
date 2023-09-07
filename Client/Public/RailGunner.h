#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CRailGunner final : public CGameObject
{
private:
	enum class STATE	{ ATTACK_AVAILABE, MAX };

private:
	explicit CRailGunner(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRailGunner(const CRailGunner&);
	virtual ~CRailGunner() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	virtual HRESULT								Ready_Components() override;

private:
	bitset<IDX(STATE::MAX)>						m_bitPlayerState;

	shared_ptr<class CRailGunner_Crosshair>		m_pCrosshair;

	shared_ptr<CRenderer>						m_pRenderer;
	shared_ptr<CTransform>						m_pTransform;

public:
	static shared_ptr<CRailGunner>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
