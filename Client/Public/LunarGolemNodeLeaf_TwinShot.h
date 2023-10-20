#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLunarGolemNodeLeaf_TwinShot final : public CNode
{
private:
	explicit CLunarGolemNodeLeaf_TwinShot() DEFAULT;
	virtual ~CLunarGolemNodeLeaf_TwinShot() DEFAULT;

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

	ANIMATION::LUNAR_GOLEM								m_eAnimation	= ANIMATION::LUNAR_GOLEM::MAX;

	_uint												m_iMagazine		= 0;			

public:
	static shared_ptr<CLunarGolemNodeLeaf_TwinShot>		Create(shared_ptr<class CBlackBoard>);
};

END
