#pragma once
#include "Engine_Define.h"
#include <wrl/client.h>

BEGIN(Engine)

class ENGINE_DLL HandleRAII final
{
public:
	HandleRAII(HANDLE hHandle) : m_hHandle(hHandle)
	{
		if (INVALID_HANDLE_VALUE == hHandle)
			throw std::exception("INVALID_HANDLE_VALUE");
	}
	~HandleRAII()
	{
		if (INVALID_HANDLE_VALUE != m_hHandle)
			CloseHandle(m_hHandle);
	}

	HandleRAII()									= delete;
	HandleRAII(const HandleRAII&)					= delete;
	HandleRAII(HandleRAII&&) noexcept				= delete;
	HandleRAII& operator=(const HandleRAII&)		= delete;
	HandleRAII& operator=(HandleRAII&&) noexcept	= delete;

public:
	HANDLE	get() const		{ return m_hHandle; }
	_bool	isValid() const	{ return INVALID_HANDLE_VALUE != m_hHandle; }

private:
	HANDLE	m_hHandle		= 0;
};

END
