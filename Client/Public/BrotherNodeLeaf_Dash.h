#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_Dash final : public CNode
{
private:
	explicit CBrotherNodeLeaf_Dash(_float fForward, _float fBackward, _float fLeft, _float fRight);
	virtual ~CBrotherNodeLeaf_Dash() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void								Activate() override;
	virtual STATUS								Invoke(_float fTimeDelta) override;
	virtual void								Terminate() override;

private:
	shared_ptr<CTransform>						m_pTransform;
	shared_ptr<CPhysics>						m_pPhysics;
	shared_ptr<CAnimator>						m_pAnimator;

	shared_ptr<CTransform>						m_pTargetTransform;

	DIRECTION									m_eDirection	= DIRECTION::MAX;
	ANIMATION::BROTHER							m_eAnimation	= ANIMATION::BROTHER::MAX;

	const _float								m_fForward		= 0.f;
	const _float								m_fBackward		= 0.f;
	const _float								m_fLeft			= 0.f;
	const _float								m_fRight		= 0.f;

public:
	static shared_ptr<CBrotherNodeLeaf_Dash>	Create(shared_ptr<class CBlackBoard>, _float fForward, _float fBackward, _float fLeft, _float fRight);
};

END
