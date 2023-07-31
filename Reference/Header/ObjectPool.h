#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CObjectPool final
{
private:
	explicit CObjectPool(const _uint iPoolSize);
	virtual ~CObjectPool() DEFAULT;

public:
	HRESULT								Initialize(shared_ptr<class CGameObject>);
	void								Tick(_float fTimeDelta);
	void								Late_Tick(_float fTimeDelta);

public:
	shared_ptr<class CGameObject>		Pop();
	void								Push(shared_ptr<class CGameObject>);

private:
	_uint								m_iPoolSize;

private:
	list<shared_ptr<class CGameObject>>	m_lstObjectPool;

	friend class CFactory;
};

END
