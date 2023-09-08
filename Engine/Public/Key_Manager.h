#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CKey_Manager final : public CSingleton<CKey_Manager>
{
public:
	typedef struct tagKeyInfo
	{
		KEY_STATE	eKeyState;
		_bool		bPrevPress;
	} KEYINFO;

private:
	explicit CKey_Manager() DEFAULT;
	virtual ~CKey_Manager() DEFAULT;

public:
	HRESULT				Initialize(HWND);
	void				Tick();

public:
	_bool				Key_Down(_uint _iKey)				{ return KEY_STATE::DOWN	== m_vecKey[_iKey].eKeyState; }
	_bool				Key_Hold(_uint _iKey)				{ return KEY_STATE::HOLD	== m_vecKey[_iKey].eKeyState; }
	_bool				Key_Up(_uint _iKey)					{ return KEY_STATE::UP		== m_vecKey[_iKey].eKeyState; }

	_bool				Key_Down();
	_bool				Key_Hold();
	_bool				Key_Up();

	void				CheckFocus(_bool _bCheck = true)	{ m_bCheckFocus = _bCheck; }

private:
	KEY_STATE			Get_KeyState(_uint _iKey) const		{ return m_vecKey[_iKey].eKeyState; }

private:
	vector<KEYINFO>		m_vecKey;

	_bool				m_bCheckFocus	= true;

	HWND				m_hWnd			= 0;

	friend CSingleton<CKey_Manager>;
};

END
