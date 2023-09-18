#pragma once
#include "Client_Define.h"
#include "Behavior.h"

BEGIN(Engine)
class CTransform;
class CPhysics;
END

BEGIN(Client)

class CHostile abstract : public CBehavior
{
protected:
	explicit CHostile();
	explicit CHostile(const CHostile&);
	virtual ~CHostile() DEFAULT;

public:
	virtual HRESULT					Initialize(shared_ptr<class CGameObject> pOwner, shared_ptr<class CGameObject> pTarget, const ENTITYDESC*);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

protected:
	_bool							Is_Target_InRange(_float fDistance) const;

protected:
	void							Chase_Target(_float fTimeDelta);

protected:
	const ENTITYDESC*				m_pEntityDesc	= nullptr;
	
	shared_ptr<CTransform>			m_pTransform;
	shared_ptr<CPhysics>			m_pPhysics;

	shared_ptr<class CGameObject>	m_pTarget;
	shared_ptr<CTransform>			m_pTargetTransform;
};

END
