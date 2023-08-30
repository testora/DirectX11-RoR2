#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CObjectPool final
{
private:
	explicit CObjectPool(const SCENE, const _uint iPoolSize);
	virtual ~CObjectPool() DEFAULT;

public:
	HRESULT											Initialize(const wstring& wstrPrototype, any = any());
	void											Tick(_float fTimeDelta);
	void											Late_Tick(_float fTimeDelta);

public:
	shared_ptr<class CGameObject>					Pop();
	HRESULT											Push(shared_ptr<class CGameObject>);

private:
	void											Add();

private:
	deque<shared_ptr<class CGameObject>>			m_deqPool;
 	unordered_set<shared_ptr<class CGameObject>>	m_usetPop;

	_uint											m_iPoolSize;

	shared_ptr<class CGameObject>					m_pPrototype;

	std::function<shared_ptr<class CGameObject>()>	m_funcPush;
	any												m_fetchArg;

	const SCENE										m_eScene;

public:
	static shared_ptr<CObjectPool>					Create(const SCENE, const _uint iPoolSize, const wstring& wstrPrototypeTag, any = any());
};

END
