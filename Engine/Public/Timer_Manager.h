#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CTimer_Manager final : public CSingleton<CTimer_Manager>
{
	friend class CSingleton<CTimer_Manager>;
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() DEFAULT;

public:
	void									Tick();

public:
	shared_ptr<class CTimer>				Find_Timer(const _float fFPS) const;
	_float									Get_TimeDelta(const _float fFPS) const;

	HRESULT									Add_Timer(const _float fFPS);
	HRESULT									Delete_Timer(const _float fFPS);

	_bool									Check_Timer(const _float fFPS);

private:
	map<_float, shared_ptr<class CTimer>>	m_mapTimer;
};

END
