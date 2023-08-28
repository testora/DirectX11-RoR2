#pragma once
#include "Engine_Define.h"
#include <windows.h>

BEGIN(Engine)

class ENGINE_DLL HandleRAII final
{
public:
	HandleRAII(HANDLE hHandle) : m_hHandle(hHandle)
	{
		if (INVALID_HANDLE_VALUE == hHandle)
			m_hHandle = nullptr;
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
	_bool	is_valid() const	{ return INVALID_HANDLE_VALUE != m_hHandle; }
	HANDLE	get() const			{ return m_hHandle; }

private:
	HANDLE	m_hHandle		= nullptr;
};

END
