#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CObjectLayer final
{
private:
	explicit CObjectLayer();
	virtual ~CObjectLayer() DEFAULT;

public:
	void								Tick(_float fTimeDelta);
	void								Late_Tick(_float fTimeDelta);

public:
	HRESULT								Add(shared_ptr<class CGameObject>);
	HRESULT								Delete();

private:
	list<shared_ptr<class CGameObject>>	m_lstObjectList;

	friend class CFactory;
};

END
