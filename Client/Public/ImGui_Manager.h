#pragma once
#include "Client_Define.h"

BEGIN(Client)

class CImGui_Manager final : public CSingleton<CImGui_Manager>
{
private:
	explicit CImGui_Manager() DEFAULT;
	virtual ~CImGui_Manager();

public:
	HRESULT	Initialize(_In_ HWND hWnd, ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	void	Tick();
	void	Render();

public:
	_bool	Is_Enable() const	{ return m_bEnable; }

	void	Enable()			{ m_bReserveEnable = true; }
	void	Disable()			{ m_bReserveEnable = false; }

private:
	_bool	m_bEnable			= false;
	_bool	m_bReserveEnable	= false;

	friend CSingleton<CImGui_Manager>;
};

END
