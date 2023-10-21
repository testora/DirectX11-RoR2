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

class CBrother_LunarShard final : public CGameObject
{
private:
	explicit CBrother_LunarShard(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CBrother_LunarShard(const CBrother_LunarShard&);
	virtual ~CBrother_LunarShard() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

	virtual HRESULT							Fetch(any aPoolPosition) override;

private:
	virtual HRESULT							Ready_Components() override;
	virtual HRESULT							Ready_Behaviors() override;

private:
	_float									m_fTimeAcc			= 0.f;

	shared_ptr<CTransform>					m_pTargetTransform;

	shared_ptr<CTransform>					m_pTransform;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CPhysics>					m_pPhysics;

	shared_ptr<CObjectPool>					m_pPool;

public:
	static shared_ptr<CBrother_LunarShard>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END
