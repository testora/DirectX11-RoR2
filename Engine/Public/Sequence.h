#pragma once
#include "Engine_Define.h"
#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CSequence : public CComposite
{
protected:
	explicit CSequence() DEFAULT;
	virtual ~CSequence() DEFAULT;

public:
	virtual HRESULT						Initialize() override;
	virtual STATUS						Tick(_float fTimeDelta) override;
	virtual HRESULT						Terminate() override;

protected:
	list<shared_ptr<CNode>>::iterator	m_itCurrentChild;

public:
	static shared_ptr<CSequence>		Create();
};

END
