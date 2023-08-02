#pragma once
#include "Engine_Define.h"

#include "Physics.h"

BEGIN(Engine)

class CBehavior_Manager final : public CSingleton<CBehavior_Manager>
{
private:
	explicit CBehavior_Manager() DEFAULT;
	virtual ~CBehavior_Manager() DEFAULT;

public:
	HRESULT												Add_Prototype(const wstring& strPrototypeTag, shared_ptr<CBehavior> pPrototype);

	shared_ptr<CBehavior>								Clone_Behavior(const wstring& strPrototypeTag, std::any = nullptr);

private:
	shared_ptr<CBehavior>								Find_Prototype(const wstring& strPrototypeTag);

private:
	unordered_map<wstring, shared_ptr<CBehavior>>		m_umapPrototype;

	friend CSingleton<CBehavior_Manager>;
};

END
