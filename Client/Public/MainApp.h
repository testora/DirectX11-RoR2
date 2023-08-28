#pragma once
#include "Client_Define.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

BEGIN(Client)

class CMainApp final : public CSingleton<CMainApp>
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT								Initialize();
	void								Tick(_float fTimeDelta);
	HRESULT								Render();

#pragma region Windows Management

	LRESULT								WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#pragma endregion
#pragma region Timer Management

	void								Tick_Timer();
	_bool								Check_Timer(const _float fFPS);
	_float								Get_TimeDelta(const _float fFPS) const;

#pragma endregion

private:
	HRESULT								Default_Settings();
	HRESULT								Ready_Component_Prototype();
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	void								ImGui_GameStatus(_float fTimeDelta, _bool* pOpen);
#endif
#endif

private:
	shared_ptr<CRenderer>				m_pMainRenderer;
	shared_ptr<CGameInstance>			m_pGameInstance;
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	shared_ptr<class CImGui_Manager>	m_pImGui_Manager;
#endif
#endif

	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;

	friend CSingleton<CMainApp>;
};

END
