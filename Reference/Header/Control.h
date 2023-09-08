#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CControl abstract : public CBehavior
{
protected:
	explicit CControl();
	virtual ~CControl() DEFAULT;

public:
	virtual HRESULT					Initialize(shared_ptr<class CGameObject> pOwner, const CHARACTERDESC*);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

protected:
	virtual void					Handle_MouseInput(_float fTimeDelta)	PURE;
	virtual void					Handle_KeyInput(_float fTimeDelta)		PURE;

protected:
	const CHARACTERDESC*			m_pCharacterDesc	= nullptr;
	
	shared_ptr<class CTransform>	m_pTargetTransform;
	shared_ptr<class CPhysics>		m_pTargetPhysics;
	shared_ptr<class CAnimator>		m_pTargetAnimator;
};

END
