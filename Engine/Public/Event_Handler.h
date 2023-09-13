#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CEvent_Handler final : public CSingleton<CEvent_Handler>
{
private:
	explicit CEvent_Handler() DEFAULT;
	virtual ~CEvent_Handler() DEFAULT;

public:
	void																	Tick(_float fTimeDelta);

public:
	void																	Register_OnTickListener(shared_ptr<void>, function<_bool(_float fTimeDelta)> funcListener);
	void																	Erase_OnTickListener(shared_ptr<void>);

private:
	multimap<weak_ptr<void>, function<_bool(_float)>, std::owner_less<>>	m_mmapTickListener;

	friend CSingleton<CEvent_Handler>;
};

END
