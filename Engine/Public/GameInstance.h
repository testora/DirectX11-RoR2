#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CSingleton<CGameInstance>
{
private:
	explicit CGameInstance();
	virtual ~CGameInstance() DEFAULT;

public:
#pragma region Engine

	HRESULT						Initialize_Engine(_In_ const GRAPHICDESC, _Out_ ComPtr<ID3D11Device>&, _Out_ ComPtr<ID3D11DeviceContext>&);
	void						Tick_Engine(_float fTimeDelta);

#pragma endregion
#pragma region Timer Manager

	void						Tick_Timer();

	HRESULT						Add_Timer(const _float fFPS);
	HRESULT						Delete_Timer(const _float fFPS);
	_bool						Check_Timer(const _float fFPS);
	shared_ptr<class CTimer>	Find_Timer(const _float fFPS);
	_float						Get_TimeDelta(const _float fFPS);

#pragma endregion
#pragma region Graphic Device

	HRESULT						Clear_BackBuffer_View(_float4 vColor);
	HRESULT						Clear_DepthStencil_View();
	HRESULT						Present();

#pragma endregion

private:
	shared_ptr<class CTimer_Manager>	m_pTimer_Manager;
	shared_ptr<class CGraphicDevice>	m_pGraphic_Device;

public:
	static void					Release_Engine();

	friend CSingleton<CGameInstance>;
};

END
