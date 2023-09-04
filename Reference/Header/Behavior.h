#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBehavior abstract
{
protected:
	explicit CBehavior(const BEHAVIOR);
	explicit CBehavior(const CBehavior&);
	virtual ~CBehavior() DEFAULT;

public:
	HRESULT							Initialize(any pOwner);
	virtual void					Tick(_float fTimeDelta);
	virtual void					Late_Tick(_float fTimeDelta);

public:
	const BEHAVIOR					Type() const	{ return m_eType; }

protected:
	weak_ptr<class CGameObject>		m_pOwner;

private:
	const BEHAVIOR					m_eType;

public:
	virtual shared_ptr<CBehavior>	Clone(any = g_aNull)	PURE;
};

END
