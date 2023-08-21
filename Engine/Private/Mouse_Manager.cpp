#include "EnginePCH.h"
#include "Mouse_Manager.h"

HRESULT CMouse_Manager::Initialize(HWND _hWnd, POINT _ptWinSize)
{
	m_hWnd      = _hWnd;
	m_ptWinSize = _ptWinSize;

	return S_OK;
}

void CMouse_Manager::Tick(WNDPROCDESC _tWndProcDesc)
{
	if (m_bCheckFocus ? m_hWnd == GetFocus() : true)
	{
		MessageProc(m_hWnd, _tWndProcDesc.message, _tWndProcDesc.wParam, _tWndProcDesc.lParam);

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);

		m_ptPrevPos = m_ptCurPos;
		m_ptCurPos = pt;

		m_ptMovement.x = m_ptCurPos.x - m_ptPrevPos.x;
		m_ptMovement.y = m_ptCurPos.y - m_ptPrevPos.y;

		if (m_bFix && m_hWnd == GetFocus())
		{
			POINT ptCenter	= { static_cast<_long>(m_ptWinSize.x * .5f), static_cast<_long>(m_ptWinSize.y * .5f) };
			m_ptCurPos		= ptCenter;

			ClientToScreen(m_hWnd, &ptCenter);
			SetCursorPos(ptCenter.x, ptCenter.y);
		}
	}
}

void CMouse_Manager::Show_Cursor(_bool _bShow)
{
	if (true == _bShow
	&&	false == m_bShow)
	{
		ShowCursor(true);
		m_bShow	= true;
		m_bFix	= false;
	}
	if (true != _bShow
	&&	false != m_bShow)
	{
		ShowCursor(false);
		m_bShow	= false;
		m_bFix	= true;
	}
}

void CMouse_Manager::Toggle_Cursor()
{
	m_bShow ? Show_Cursor(false) : Show_Cursor(true);
}

void CMouse_Manager::MessageProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_MOUSEWHEEL:
		m_ptScroll.y = GET_WHEEL_DELTA_WPARAM(_wParam);
		break;
	case WM_MOUSEHWHEEL:
		m_ptScroll.x = GET_WHEEL_DELTA_WPARAM(_wParam);
		break;
	}
}
