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
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, const CHARACTERDESC*);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	void							Force(_vectorf vDirection, _float fMagnitude, _float fTimeDelta = 1.f);
	void							Force(TRANSFORM eDirection, _float fMagnitude, _float fTimeDelta = 1.f);
	void							Flattern(_bool bX, _bool bY, _bool bZ);

private:
	void							Execute_Gravity(_float fTimeDelta);
	void							Resist(_float fTimeDelta);
	void							Terminate();

private:
	_float3							m_vVelocity;

	const CHARACTERDESC*			m_pCharacterDesc{};

	shared_ptr<class CTransform>	m_pTargetTransform;

#ifdef _DEBUG
#if ACTIVATE_IMGUI
public:
	const _float3					Get_Velocity() const { return m_vVelocity; }
#endif
#endif

public:
	static shared_ptr<CPhysics>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, shared_ptr<class CGameObject> pOwner, const CHARACTERDESC*);
	virtual shared_ptr<CBehavior>	Clone(any = any()) override;
};

END
