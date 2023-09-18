#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CObjectPool;
class CTransform;
class CShader;
class CTexture;
class CPhysics;
END

BEGIN(Client)

class CRailGunner_PistolBullet final : public CGameObject
{
private:
	enum class STATE	{ UNTARGET, TARGET, MAX };

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

	virtual HRESULT									Fetch(any aPoolPosition) override;

private:
	virtual HRESULT									Ready_Components() override;
	virtual HRESULT									Ready_Behaviors() override;

private:
	void											Set_Target(shared_ptr<class CMonster>);

private:
	void											Manage_State(_float fTimeDelta);
	void											Search_Target();

	void											Destroy();

private:
	STATE											m_eState	= STATE::MAX;

	_float											m_fTimeAcc	= 0.f;
	_float3											m_vDirection;

	shared_ptr<class CMonster>						m_pTarget;
	shared_ptr<CTransform>							m_pTargetTransform;

	shared_ptr<CTransform>							m_pTransform;
	shared_ptr<CShader>								m_pShader;
	shared_ptr<CTexture>							m_pTexMask;
	shared_ptr<CPhysics>							m_pPhysics;

	shared_ptr<CObjectPool>							m_pPool;

public:
	static shared_ptr<CRailGunner_PistolBullet>		Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END
