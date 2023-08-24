#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CControl final : public CBehavior
{
private:
	explicit CControl();
	explicit CControl(const CControl&);
	virtual ~CControl() DEFAULT;

public:
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, const CHARACTERDESC*);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	void							Handle_MouseInput(_float fTimeDelta);
	void							Handle_KeyInput(_float fTimeDelta);

private:
	const CHARACTERDESC*			m_pCharacterDesc	= nullptr;
	
	shared_ptr<class CTransform>	m_pTargetTransform;
	shared_ptr<class CPhysics>		m_pTargetPhysics;
	shared_ptr<class CAnimator>		m_pTargetAnimator;

public:
	static shared_ptr<CControl>		Create(shared_ptr<class CGameObject> pOwner, const CHARACTERDESC*);
	virtual shared_ptr<CBehavior>	Clone(any = any()) override;
};

END
