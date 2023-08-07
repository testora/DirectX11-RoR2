#pragma once
#include "Engine_Define.h"
#include <wrl/client.h>

BEGIN(Engine)

class ENGINE_DLL HandleWrapper final
{
public:
	HandleWrapper(HANDLE hHandle) : m_hHandle(hHandle)
	{
		if (INVALID_HANDLE_VALUE == hHandle)
			throw std::exception("INVALID_HANDLE_VALUE");
	}
	~HandleWrapper()
	{
		if (INVALID_HANDLE_VALUE != m_hHandle)
			CloseHandle(m_hHandle);
	}

	HandleWrapper()										= delete;
	HandleWrapper(const HandleWrapper&)					= delete;
	HandleWrapper(HandleWrapper&&) noexcept				= delete;
	HandleWrapper& operator=(const HandleWrapper&)		= delete;
	HandleWrapper& operator=(HandleWrapper&&) noexcept	= delete;

public:
	_bool	is_valid() const	{ return INVALID_HANDLE_VALUE != m_hHandle; }
	HANDLE	get() const			{ return m_hHandle; }

private:
	HANDLE	m_hHandle		= 0;
};

END
