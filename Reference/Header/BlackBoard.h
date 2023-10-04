#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBlackBoard final
{
private:
	explicit CBlackBoard() DEFAULT;
	virtual ~CBlackBoard() DEFAULT;

public:
	HRESULT												Add_System(const wstring& wstrKey, shared_ptr<class ISystem> pSystem);
	HRESULT												Add_EntityDesc(const wstring& wstrKey, const ENTITYDESC*);
	HRESULT												Add_Anything(const wstring& wstrKey, any);

	template<typename T>
	shared_ptr<T>										Get_System(const wstring& wstrKey);
	shared_ptr<class ISystem>							Get_System(const wstring& wstrKey);
	const ENTITYDESC*									Get_EntityDesc(const wstring& wstrKey);
	template<typename T>
	optional<T>											Get_Anything(const wstring& wstrKey);
	any													Get_Anything(const wstring& wstrKey);

private:
	unordered_map<wstring, shared_ptr<class ISystem>>	m_umapSystem;
	unordered_map<wstring, const ENTITYDESC*>			m_umapEntityDesc;
	unordered_map<wstring, any>							m_umapAnything;

public:
	static shared_ptr<CBlackBoard>						Create();
};

#include "BlackBoard.inl"

END
