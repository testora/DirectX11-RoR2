#pragma once
#include "Engine_Define.h"
#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CComposite abstract : public CNode
{
protected:
	explicit CComposite() DEFAULT;
	virtual ~CComposite() DEFAULT;

public:
	virtual HRESULT					Initialize() override;
	virtual STATUS					Tick(_float fTimeDelta) override	PURE;
	virtual HRESULT					Terminate() override;

public:
	void							Add_Child(shared_ptr<CNode> pNode);

protected:
	list<shared_ptr<CNode>>			m_lstChildren;
};

END
