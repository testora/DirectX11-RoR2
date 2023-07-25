#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CTimer final
{
public:
	explicit CTimer();
	virtual ~CTimer() DEFAULT;


public:
	HRESULT			Initialize();
	void			Tick();

public:
	_float			Get_TimeDelta() const	{ return m_fTimeDelta; }

	_bool			Check(_float fFPS);

private:
	LARGE_INTEGER	m_cpuTick{};
	LARGE_INTEGER	m_orgTime{};
	LARGE_INTEGER	m_curTime{};
	LARGE_INTEGER	m_oldTime{};

	_float			m_fTimeDelta	= 0.f;
	_float			m_fTimeAcc		= 0.f;
};

END
