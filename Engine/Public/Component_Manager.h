#pragma once
#include "Engine_Define.h"

#include "Renderer.h"
#include "Transform.h"

BEGIN(Engine)

class CComponent_Manager final : public CSingleton<CComponent_Manager>
{
private:
	explicit CComponent_Manager() DEFAULT;
	virtual ~CComponent_Manager() DEFAULT;

public:
	HRESULT												Add_Prototype(const wstring& strPrototypeTag, shared_ptr<CComponent> pPrototype);

	shared_ptr<CComponent>								Clone_Component(const wstring& strPrototypeTag, std::any = nullptr);

private:
	shared_ptr<CComponent>								Find_Prototype(const wstring& strPrototypeTag);

private:
	unordered_map<wstring, shared_ptr<CComponent>>		m_umapPrototype;

	friend CSingleton<CComponent_Manager>;
};

END
