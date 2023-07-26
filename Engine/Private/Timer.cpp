#include "EnginePCH.h"
#include "Timer.h"

CTimer::CTimer()
{
}

HRESULT CTimer::Initialize()
{
	QueryPerformanceFrequency(&m_cpuTick);
	QueryPerformanceCounter(&m_orgTime);
	QueryPerformanceCounter(&m_curTime);
	QueryPerformanceCounter(&m_oldTime);

	return S_OK;
}

void CTimer::Tick()
{
	QueryPerformanceCounter(&m_curTime);

	if (m_curTime.QuadPart - m_orgTime.QuadPart > m_cpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_cpuTick);
		m_orgTime = m_curTime;
	}

	m_fTimeDelta = _float(m_curTime.QuadPart - m_oldTime.QuadPart) / m_cpuTick.QuadPart;

	m_oldTime = m_curTime;

	m_fTimeAcc += m_fTimeDelta;
}

_bool CTimer::Check(_float _fFPS)
{
	if (m_fTimeAcc >= _fFPS)
	{
		m_fTimeAcc = fmod(m_fTimeAcc, _fFPS);
		return true;
	}

	return false;
}
