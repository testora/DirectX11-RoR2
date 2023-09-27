#pragma once
#include "Engine_Define.h"
#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CDecorator abstract : public CNode
{
protected:
	explicit CDecorator(shared_ptr<CNode>);
	virtual ~CDecorator() DEFAULT;

public:
	virtual HRESULT					Initialize() override;
	virtual STATUS					Tick(_float fTimeDelta) override	PURE;
	virtual HRESULT					Terminate() override;

protected:
	shared_ptr<CNode>				m_pNode;
};

END
