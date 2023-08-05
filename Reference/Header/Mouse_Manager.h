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
	void		Tick();

public:
	POINT		Get_CursorPos() const			{ return m_ptCurPos; }
	POINT		Get_CursorMove() const			{ return m_ptMovement; }
	_bool		Is_CursorOn() const				{ return m_bShow; }

	void		CheckFocus(_bool bCheck = true)	{ m_bCheckFocus = bCheck; }

	void		Fix_Cursor(_bool bFix = true)	{ m_bFix = bFix; }
	void		Show_Cursor(_bool bShow = true);
	void		Toggle_Cursor();

private:
	POINT		m_ptWinSize{};
	POINT		m_ptCurPos{};
	POINT		m_ptPrevPos{};
	POINT		m_ptMovement{};

	_bool		m_bCheckFocus	= true;
	_bool		m_bFix			= false;
	_bool		m_bShow			= true;

	HWND		m_hWnd			= 0;

	friend CSingleton<CMouse_Manager>;
};

END
