#pragma once
#include "Engine_Define.h"
#include "BlackBoard.h"

BEGIN(Engine)

class ENGINE_DLL CNode abstract
{
protected:
	explicit CNode() DEFAULT;
	explicit CNode(const CNode&) DEFAULT;
	virtual ~CNode() DEFAULT;

public:
	virtual HRESULT					Initialize(shared_ptr<class CBlackBoard>);

public:
	virtual void					Activate();
	virtual STATUS					Invoke(_float fTimeDelta)		PURE;
	virtual void					Terminate();

public:
	_bool							Is_Running() const		{ return m_eStatus == STATUS::RUNNING; }
	_bool							Is_Terminated() const	{ return m_eStatus == STATUS::MAX; }

protected:
	void							Begin_Invoke(_float fTimeDelta);
	STATUS							Return_Invoke();

protected:
	STATUS							m_eStatus	= STATUS::MAX;

	shared_ptr<class CBlackBoard>	m_pBlackBoard;

	_float							m_fTimeAcc	= 0.f;
};

END
