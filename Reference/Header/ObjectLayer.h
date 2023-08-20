#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CObjectLayer final
{
private:
	explicit CObjectLayer(const SCENE);
	virtual ~CObjectLayer() DEFAULT;

public:
	void											Tick(_float fTimeDelta);
	void											Late_Tick(_float fTimeDelta);

public:
	HRESULT											Add(shared_ptr<class CGameObject>);
	HRESULT											Delete(shared_ptr<class CGameObject>);

	void											Iterate_Objects(function<_bool(shared_ptr<class CGameObject>)>);

private:
	unordered_set<shared_ptr<class CGameObject>>	m_usetObject;

	const SCENE										m_eScene;

public:
	static shared_ptr<CObjectLayer>					Create(const SCENE);
};

END
