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
}

void CEvent_Handler::Register_TickListener(shared_ptr<void> _pSubscriber, function<_bool(_float)> _func)
{
	m_mmapTickListener.emplace(_pSubscriber, _func);
}

void CEvent_Handler::Erase_TickListener(shared_ptr<void> _pSubscriber)
{
	m_mmapTickListener.erase(_pSubscriber);
}