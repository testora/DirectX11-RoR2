#include "EnginePCH.h"
#include "BlackBoard.h"
#include "System.h"

HRESULT CBlackBoard::Add_System(const wstring& _wstrKey, shared_ptr<ISystem> _pSystem)
{
	if (m_umapSystem.end() != m_umapSystem.find(_wstrKey))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTree::Add_System", "Invalid Key");
	}

	m_umapSystem.emplace(_wstrKey, _pSystem);

	return S_OK;
}

HRESULT CBlackBoard::Add_EntityDesc(const wstring& _wstrKey, const ENTITYDESC* _pEntityDesc)
{
	if (m_umapEntityDesc.end() != m_umapEntityDesc.find(_wstrKey))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTree::Add_EntityDesc", "Invalid Key");
	}

	m_umapEntityDesc.emplace(_wstrKey, _pEntityDesc);

	return S_OK;
}

HRESULT CBlackBoard::Add_Anything(const wstring& _wstrKey, any _aAny)
{
	if (m_umapAnything.end() != m_umapAnything.find(_wstrKey))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTree::Add_Anything", "Invalid Key");
	}

	m_umapAnything.emplace(_wstrKey, _aAny);

	return S_OK;
}

shared_ptr<ISystem> CBlackBoard::Get_System(const wstring& _wstrKey)
{
	if (m_umapSystem.end() != m_umapSystem.find(_wstrKey))
	{
		return m_umapSystem[_wstrKey];
	}

	return nullptr;
}

const ENTITYDESC* CBlackBoard::Get_EntityDesc(const wstring& _wstrKey)
{
	if (m_umapEntityDesc.end() != m_umapEntityDesc.find(_wstrKey))
	{
		return m_umapEntityDesc[_wstrKey];
	}

	return nullptr;
}

any CBlackBoard::Get_Anything(const wstring& _wstrKey)
{
	if (m_umapAnything.end() != m_umapAnything.find(_wstrKey))
	{
		return m_umapAnything[_wstrKey];
	}

	return g_aNull;
}

shared_ptr<CBlackBoard> CBlackBoard::Create()
{
	return make_private_shared(CBlackBoard);
}
