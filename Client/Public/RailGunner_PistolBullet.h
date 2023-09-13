#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
END

BEGIN(Client)

class CRailGunner_PistolBullet final : public CGameObject
{
private:
	explicit CRailGunner_PistolBullet(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRailGunner_PistolBullet(const CRailGunner_PistolBullet&);
	virtual ~CRailGunner_PistolBullet() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any = g_aNull) override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

	virtual HRESULT									Fetch(any aPosition) override;

private:
	virtual HRESULT									Ready_Components() override;

private:
	shared_ptr<CTransform>							m_pTransform;
	shared_ptr<CShader>								m_pShader;
	shared_ptr<CTexture>							m_pTexMask;

public:
	static shared_ptr<CRailGunner_PistolBullet>		Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END
