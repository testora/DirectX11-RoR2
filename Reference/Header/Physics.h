#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CPhysics final : public CBehavior
{
private:
	explicit CPhysics(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPhysics(const CPhysics&);
	virtual ~CPhysics() DEFAULT;

public:
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	
public:
	void							Set_MaxSpeed(_float3 vMaxSpeed)	{ m_vMaxSpeed = vMaxSpeed; }
	void							Set_Resist(_float3 vResist)		{ m_vResist = vResist; }

public:
	void							Force(_vectorf vDirection, _float fMagnitude, _float fTimeDelta);
	void							Force(TRANSFORM eDirection, _float fMagnitude, _float fTimeDelta);

private:
	void							Terminate();
	void							Resist(_float fTimeDelta);

private:
	_float3							m_vDirection	= { 0.f, 0.f, 0.f };
	_float3							m_vMaxSpeed		= { 1.f, 1.f, 1.f };
	_float3							m_vResist		= { 1.f, 1.f, 1.f };

	shared_ptr<class CTransform>	m_pTargetTransform;

public:
	static shared_ptr<CPhysics>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, shared_ptr<class CGameObject> pOwner);
	virtual shared_ptr<CBehavior>	Clone(any = any()) override;
};

END
