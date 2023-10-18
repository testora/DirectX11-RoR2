#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CPhysics final : public CBehavior
{
private:
	explicit CPhysics();
	explicit CPhysics(const CPhysics&);
	virtual ~CPhysics() DEFAULT;

public:
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	inline const _bool				Is_Moving(_bool bX = true, _bool bY = true, _bool bZ = true) const;

	const _float3					Get_Velocity() const				{ return m_vVelocity; }
	void							Set_Gravity(_bool bEnable = true)	{ m_bEnableGravity = bEnable; }

public:
	void							Force(_vectorf);
	void							Force(_vectorf vDirection, _float fMagnitude, _float fTimeDelta = 1.f);
	void							Force(TRANSFORM eDirection, _float fMagnitude, _float fTimeDelta = 1.f);
	void							Flattern(_bool bX, _bool bY, _bool bZ);

private:
	void							Execute_Gravity(_float fTimeDelta);
	void							Resist(_float fTimeDelta);
	void							Terminate();

private:
	_float3							m_vVelocity;

	_bool							m_bEnableGravity	= true;

	const ENTITYDESC*				m_pEntityDesc{};
	shared_ptr<class CTransform>	m_pTargetTransform;

public:
	static shared_ptr<CPhysics>		Create(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>	Clone(any = g_aNull) override;
};

END
