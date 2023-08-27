#pragma once
#include "Engine_Define.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"
#include "PipeLine.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CSingleton<CGameInstance>
{
private:
	explicit CGameInstance();
	virtual ~CGameInstance() DEFAULT;

public:
#pragma region Engine

	HRESULT										Initialize_Engine(_In_ const SCENE eStatic, _In_ const SCENE eMax, _In_ const GRAPHICDESC, _Out_ ComPtr<ID3D11Device>&, _Out_ ComPtr<ID3D11DeviceContext>&);
	void										Tick_Engine(_float fTimeDelta);

	LRESULT										WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	WNDPROCDESC									Get_WndProcDesc() const;

#pragma endregion
#pragma region Graphic Device

	HRESULT										Clear_BackBuffer_View(_color cBackBuffer);
	HRESULT										Clear_DepthStencil_View();
	HRESULT										Present();

#pragma endregion
#pragma region Timer Manager

	void										Tick_Timer();

	HRESULT										Add_Timer(const _float fFPS);
	HRESULT										Delete_Timer(const _float fFPS);
	_bool										Check_Timer(const _float fFPS);
	shared_ptr<class CTimer>					Find_Timer(const _float fFPS);
	_float										Get_TimeDelta(const _float fFPS);

#pragma endregion
#pragma region Mouse Manager

	POINT										Get_CursorPos();
	POINT										Get_CursorMove();
	_bool										Is_CursorOn();

	void										CheckFocus_OnMouse(_bool bCheck = true);

	void										Fix_Cursor(_bool bFix = true);
	void										Show_Cursor(_bool bShow = true);
	void										Toggle_Cursor();

#pragma endregion
#pragma region Key Manager

	_bool										Key_Down(_uint iKey = VK_MAX);
	_bool										Key_Hold(_uint iKey = VK_MAX);
	_bool										Key_Up(_uint iKey = VK_MAX);

	void										CheckFocus_OnKeyboard(_bool bCheck = true);

#pragma endregion
#pragma region Event Handler

	void										Register_TickListener(shared_ptr<class CGameObject>, function<_bool(_float fTimeDelta)>);

#pragma endregion
#pragma region Scene Manager

	HRESULT										Open_Scene(SCENE, shared_ptr<class CScene>);
	const SCENE									Current_Scene(_bool bSkipLoad = true);

#pragma endregion
#pragma region Object Manager

	HRESULT										Add_Object_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<class CGameObject> pPrototype);
	shared_ptr<class CGameObject>				Clone_GameObject(const SCENE, const wstring& strPrototypeTag, any = any());

	shared_ptr<class CObjectLayer>				Add_Layer(const SCENE, const wstring& strLayerTag);
	shared_ptr<class CObjectPool>				Add_Pool(const SCENE, const wstring& strPoolTag, const wstring& strPrototypeTag, _uint iPoolSize, any = any());

	shared_ptr<class CObjectLayer>				Find_Layer(const SCENE, const wstring& strLayerTag);
	shared_ptr<class CObjectPool>				Find_Pool(const SCENE, const wstring& strPoolTag);

	void										Iterate_Layers(const SCENE, function<_bool(pair<wstring, shared_ptr<class CObjectLayer>>)>);
	void										Iterate_Pools(const SCENE, function<_bool(pair<wstring, shared_ptr<class CObjectPool>>)>);

#pragma endregion
#pragma region Component Manager

	HRESULT										Add_Component_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<class CComponent> pPrototype);
	shared_ptr<class CComponent>				Clone_Component(const SCENE, const wstring& strPrototypeTag, any = any());

#pragma endregion
#pragma region Behavior Manager

	HRESULT										Add_Behavior_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<class CBehavior> pPrototype);
	shared_ptr<class CBehavior>					Clone_Behavior(const SCENE, const wstring& strPrototypeTag, any = any());

#pragma endregion
#pragma region Grid Manager

	_float3										Get_GridSize();

	HRESULT										Register_VIBuffer(const SCENE, const wstring& strGridLayerTag, shared_ptr<class CGameObject>);
	HRESULT										Reset_Grids(const SCENE, const wstring& strGridLayerTag = wstring());

	_float3										Raycast(_vectorf vRayOrigin, _vectorf vRayDirection, _float fRange = FLT_MAX);
	_float3										Raycast(const wstring& strGridLayerTag, _vectorf vRayOrigin, _vectorf vRayDirection, _float fRange = FLT_MAX);

#pragma endregion
#pragma region Grid Manager

	HRESULT										Add_BindShaders(shared_ptr<class CShader>);
	HRESULT										Add_Lights(const SCENE, LIGHTDESC, shared_ptr<class CTransform>);
	HRESULT										Clear_Lights(const SCENE);


#pragma endregion

private:
	shared_ptr<class CGraphicDevice>			m_pGraphic_Device;
	shared_ptr<class CTimer_Manager>			m_pTimer_Manager;
	shared_ptr<class CMouse_Manager>			m_pMouse_Manager;
	shared_ptr<class CKey_Manager>				m_pKey_Manager;
	shared_ptr<class CEvent_Handler>			m_pEvent_Handler;
	shared_ptr<class CScene_Manager>			m_pScene_Manager;
	shared_ptr<class CObject_Manager>			m_pObject_Manager;
	shared_ptr<class CComponent_Manager>		m_pComponent_Manager;
	shared_ptr<class CBehavior_Manager>			m_pBehavior_Manager;
	shared_ptr<class CGrid_Manager>				m_pGrid_Manager;
	shared_ptr<class CLight_Manager>			m_pLight_Manager;
	shared_ptr<class CPicker>					m_pPicker;

private:
	WNDPROCDESC									m_tWndProcDesc{};

public:
	static void									Release_Engine();

	friend CSingleton<CGameInstance>;
};

END
