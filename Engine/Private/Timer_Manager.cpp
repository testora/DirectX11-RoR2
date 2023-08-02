#include "EnginePCH.h"
#include "Timer_Manager.h"
#include "Timer.h"

void CTimer_Manager::Tick()
{
	for (auto& iter : m_mapTimer)
	{
		iter.second->Tick();
	}
}

shared_ptr<CTimer> CTimer_Manager::Find_Timer(const _float _fFPS) const
{
	auto iter = m_mapTimer.find(_fFPS);
	if (iter == m_mapTimer.end())
	{
		return nullptr;
	}

	return iter->second;
}

_float CTimer_Manager::Get_TimeDelta(const _float _fFPS) const
{
	auto pTimer = Find_Timer(_fFPS);
	if (nullptr == pTimer)
	{
		MSG_RETURN(0.f, "CTimer_Manager::Get_TimeDelta", "Failed: Find_Timer");
	}

	return pTimer->Get_TimeDelta();
}

HRESULT CTimer_Manager::Add_Timer(const _float _fFPS)
{
	if (nullptr != Find_Timer(_fFPS))
	{
		return E_FAIL;
	}

	m_mapTimer.emplace(_fFPS, CFactory<CTimer>::Create());
	if (FAILED(m_mapTimer[_fFPS]->Initialize()))
	{
		m_mapTimer.erase(_fFPS);
		MSG_RETURN(E_FAIL, "CTimer_Manager::Add_Timer", "Failed: m_mapTimer[_fFPS]->Initialize");
	}

	return S_OK;
}

HRESULT CTimer_Manager::Delete_Timer(const _float _fFPS)
{
	if (nullptr == Find_Timer(_fFPS))
	{
		MSG_RETURN(E_FAIL, "CTimer_Manager::Delete_Timer", "Failed: Find_Timer");
	}

	m_mapTimer.erase(_fFPS);

	return S_OK;
}

_bool CTimer_Manager::Check_Timer(const _float _fFPS)
{
	auto pTimer = Find_Timer(_fFPS);
	if (nullptr == pTimer)
	{
		MSG_RETURN(false, "CTimer_Manager::Check_Timer", "Failed: Find_Timer");
	}

	return pTimer->Check(_fFPS);
}
