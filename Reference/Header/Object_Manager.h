#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CObject_Manager final : public CSingleton<CObject_Manager>
{
private:
	explicit CObject_Manager() DEFAULT;
	virtual ~CObject_Manager() DEFAULT;

public:
	void														Tick(_float fTimeDelta);
	void														Late_Tick(_float fTimeDelta);

public:
	shared_ptr<class CObjectLayer>								Find_Layer(const wstring& strLayerTag);
	shared_ptr<class CObjectPool>								Find_Pool(const wstring& strPoolTag);

public:
	HRESULT														Add_Prototype(const wstring& strPrototypeTag, shared_ptr<class CGameObject> pPrototype);

	HRESULT														Add_Layer(const wstring& strLayerTag);
	HRESULT														Add_Pool(const wstring& strPoolTag, const wstring& strPrototypeTag, _uint iPoolSize, any = any());

	shared_ptr<class CGameObject>								Clone_GameObject(const wstring& strPrototypeTag, any = any());

private:
	shared_ptr<class CGameObject>								Find_Prototype(const wstring& strPrototypeTag);

protected:
	unordered_map<wstring, shared_ptr<class CObjectLayer>>		m_umapLayer;
	unordered_map<wstring, shared_ptr<class CObjectPool>>		m_umapPool;

private:
	unordered_map<wstring, shared_ptr<class CGameObject>>		m_umapPrototype;

	friend CSingleton<CObject_Manager>;
};

END
