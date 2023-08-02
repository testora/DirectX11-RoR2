#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CObjectPool final
{
private:
	explicit CObjectPool(const _uint iPoolSize);
	virtual ~CObjectPool() DEFAULT;

public:
	HRESULT											Initialize(const wstring& strPrototype, std::any = nullptr);
	void											Tick(_float fTimeDelta);
	void											Late_Tick(_float fTimeDelta);

public:
	shared_ptr<class CGameObject>					Pop();
	void											Push(shared_ptr<class CGameObject> = nullptr);

private:
	deque<shared_ptr<class CGameObject>>			m_deqPool;
 	unordered_set<shared_ptr<class CGameObject>>	m_usetPop;

	_uint											m_iPoolSize;

	shared_ptr<class CGameObject>					m_pPrototype;

	std::function<shared_ptr<class CGameObject>()>	m_fnPush;

public:
	static shared_ptr<CObjectPool>					Create(const _uint iPoolSize, const wstring& strPrototypeTag, std::any = nullptr);
};

END
