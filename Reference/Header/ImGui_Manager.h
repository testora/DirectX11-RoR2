#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CImGui_Manager final : public CSingleton<CImGui_Manager>
{
private:
	explicit CImGui_Manager() DEFAULT;
	virtual ~CImGui_Manager();

public:
	HRESULT	Initialize(_In_ HWND hWnd, ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	void	Tick();
	void	Render();

public:
	void	Enable();
	void	Disable();

private:
	_bool	m_bEnable	= false;

public:
	IMGUI_IMPL_API LRESULT WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	friend CSingleton<CImGui_Manager>;
};

END
