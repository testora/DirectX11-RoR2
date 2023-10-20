#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CLunarExploderNodeLeaf_Sprint final : public CNode
{
private:
	explicit CLunarExploderNodeLeaf_Sprint(_float fForward, _float fLeft, _float fRight);
	virtual ~CLunarExploderNodeLeaf_Sprint() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

private:
	const ENTITYDESC*									m_pEntityDesc	= nullptr;

	shared_ptr<CTransform>								m_pTransform;
	shared_ptr<CPhysics>								m_pPhysics;
	shared_ptr<CAnimator>								m_pAnimator;

	shared_ptr<CTransform>								m_pTargetTransform;

	DIRECTION											m_eDirection	= DIRECTION::MAX;

	_float												m_fForward		= 0.f;
	_float												m_fLeft			= 0.f;
	_float												m_fRight		= 0.f;

public:
	static shared_ptr<CLunarExploderNodeLeaf_Sprint>	Create(shared_ptr<class CBlackBoard>, _float fForward, _float fLeft, _float fRight);
};

END
