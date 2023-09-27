#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CNode abstract
{
protected:
	explicit CNode() DEFAULT;
	explicit CNode(const CNode&) DEFAULT;
	virtual ~CNode() DEFAULT;

public:
	virtual HRESULT				Initialize();
	virtual STATUS				Tick(_float fTimeDelta)		PURE;
	virtual HRESULT				Terminate();

	virtual STATUS				Update(_float fTimeDelta);

public:
	_bool						Is_Running() const		{ return m_eStatus == STATUS::RUNNING; }
	_bool						Is_Terminated() const	{ return m_eStatus != STATUS::RUNNING; }

protected:
	STATUS						m_eStatus	= STATUS::MAX;
};

END
