#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CObjectLayer final
{
private:
	explicit CObjectLayer() DEFAULT;
	virtual ~CObjectLayer() DEFAULT;

public:
	void											Tick(_float fTimeDelta);
	void											Late_Tick(_float fTimeDelta);

public:
	HRESULT											Add(shared_ptr<class CGameObject>);
	HRESULT											Delete(shared_ptr<class CGameObject>);

private:
	unordered_set<shared_ptr<class CGameObject>>	m_usetObject;

public:
	static shared_ptr<CObjectLayer>					Create();
};

END
