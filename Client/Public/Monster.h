#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CMonster abstract : public CGameObject
{
protected:
	explicit CMonster(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster(const CMonster&);
	virtual ~CMonster() DEFAULT;

protected:
	virtual HRESULT				Initialize(any = g_aNull) override;
	virtual HRESULT				Ready_Components() override;
	virtual HRESULT				Ready_Behaviors() override;

public:
	_float4x4					Get_WeakPoint() const;

public:
	_float						Distance(shared_ptr<CGameObject>);

	virtual void				Hit()			PURE;
	virtual void				Hit_WeakPoint()	PURE;

	virtual void				Wander()		PURE;

protected:
	shared_ptr<CTransform>		m_pTransform;
	shared_ptr<CPhysics>		m_pPhysics;
	shared_ptr<CAnimator>		m_pAnimator;

	const _float4x4*			m_pWeakPointWorld	= nullptr;
	_float4x4					m_mPivot			= g_mUnit;
};

END
