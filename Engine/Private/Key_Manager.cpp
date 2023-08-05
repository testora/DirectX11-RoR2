#include "EnginePCH.h"
#include "Key_Manager.h"

HRESULT CKey_Manager::Initialize(HWND _hWnd)
{
	m_hWnd = _hWnd;

	m_vecKey.reserve(VK_MAX);
	for (_uint i = 0; i < VK_MAX; ++i)
	{
		m_vecKey.push_back(KEYINFO{ KEY_STATE::NONE, false });
	}

    return S_OK;
}

void CKey_Manager::Tick()
{
	if (m_bCheckFocus ? m_hWnd == GetFocus() : true)
	{
		for (_uint i = 0; i < VK_MAX; ++i)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eKeyState = KEY_STATE::HOLD;
				}
				else
				{
					m_vecKey[i].eKeyState = KEY_STATE::DOWN;
				}

				m_vecKey[i].bPrevPress = true;
			}
			else
			{
				if (m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eKeyState = KEY_STATE::UP;
				}
				else
				{
					m_vecKey[i].eKeyState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPress = false;
			}
		}
	}
}

_bool CKey_Manager::Key_Down()
{
	for (_uint i = 0; i < VK_MAX; ++i)
	{
		if (i == VK_LBUTTON
		||	i == VK_RBUTTON
		||	i == VK_MBUTTON
		||	i == VK_XBUTTON1
		||	i == VK_XBUTTON2)
		{
			continue;
		}

		if (m_vecKey[i].eKeyState == KEY_STATE::DOWN)
		{
			return true;
		}
	}

	return false;
}

_bool CKey_Manager::Key_Hold()
{
	for (_uint i = 0; i < VK_MAX; ++i)
	{
		if (i == VK_LBUTTON
		||	i == VK_RBUTTON
		||	i == VK_MBUTTON
		||	i == VK_XBUTTON1
		||	i == VK_XBUTTON2)
		{
			continue;
		}

		if (m_vecKey[i].eKeyState == KEY_STATE::HOLD)
		{
			return true;
		}
	}

	return false;
}

_bool CKey_Manager::Key_Up()
{
	for (_uint i = 0; i < VK_MAX; ++i)
	{
		if (i == VK_LBUTTON
		||	i == VK_RBUTTON
		||	i == VK_MBUTTON
		||	i == VK_XBUTTON1
		||	i == VK_XBUTTON2)
		{
			continue;
		}

		if (m_vecKey[i].eKeyState == KEY_STATE::UP)
		{
			return true;
		}
	}

	return false;
}
