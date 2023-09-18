#pragma once
#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)

class ENGINE_DLL CBehavior abstract : public ISystem
{
protected:
	explicit CBehavior(const BEHAVIOR);
	explicit CBehavior(const CBehavior&);
	virtual ~CBehavior() DEFAULT;

public:
	HRESULT							Initialize(any pOwner);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	const BEHAVIOR					Type() const	{ return m_eType; }

protected:
	inline shared_ptr<CBehavior>	shared_from_behavior();

protected:
	weak_ptr<class CGameObject>		m_pOwner;

private:
	const BEHAVIOR					m_eType;

public:
	virtual shared_ptr<CBehavior>	Clone(any = g_aNull)	PURE;
};

END
