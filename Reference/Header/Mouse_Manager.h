#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CMouse_Manager final : public CSingleton<CMouse_Manager>
{
private:
	explicit CMouse_Manager() DEFAULT;
	virtual ~CMouse_Manager() DEFAULT;

public:
	HRESULT		Initialize(HWND, POINT ptWinSize);
	void		Tick(WNDPROCDESC);

public:
	_bool		Is_CursorOn() const					{ return m_bShow; }
	
	POINT		Get_CursorPos() const				{ return m_ptCurPos; }
	POINT		Get_CursorMove() const				{ return m_ptMovement; }
	POINTS		Get_CursorScroll() const			{ return m_ptScroll; }

	void		CheckFocus(_bool bCheck = true)		{ m_bCheckFocus = bCheck; }

	void		Fix_Cursor(_bool bFix = true)		{ m_bFix = bFix; }
	void		Show_Cursor(_bool bShow = true);
	void		Toggle_Cursor();

private:
	void		MessageProc(HWND, UINT, WPARAM, LPARAM);

private:
	POINT		m_ptWinSize{};
	POINT		m_ptCurPos{};
	POINT		m_ptPrevPos{};
	POINT		m_ptMovement{};
	POINTS		m_ptScroll{};

	_bool		m_bCheckFocus	= true;
	_bool		m_bFix			= false;
	_bool		m_bShow			= true;

	HWND		m_hWnd			= 0;

	friend CSingleton<CMouse_Manager>;
};

END
