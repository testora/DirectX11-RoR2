#include "EnginePCH.h"
#include "Event_Handler.h"

void CEvent_Handler::Tick(_float _fTimeDelta)
{
	for (auto iter = m_mmapTickListener.begin(); iter != m_mmapTickListener.end();)
	{
		if (!iter->first.expired())
		{
			if (!iter->second(_fTimeDelta))
			{
				iter = m_mmapTickListener.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		else
		{
			iter = m_mmapTickListener.erase(iter);
		}
	}

	for (auto iter = m_vecTickListener.begin(); iter != m_vecTickListener.end();)
	{
		if (!(*iter)(_fTimeDelta))
		{
			iter = m_vecTickListener.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CEvent_Handler::Register_OnTickListener(shared_ptr<void> _pSubscriber, function<_bool(_float)> _func)
{
	m_mmapTickListener.emplace(_pSubscriber, _func);
}

void CEvent_Handler::Register_OnTickListener(function<_bool(_float)> _func)
{
	m_vecTickListener.emplace_back(_func);
}

void CEvent_Handler::Erase_OnTickListener(shared_ptr<void> _pSubscriber)
{
	m_mmapTickListener.erase(_pSubscriber);
}
