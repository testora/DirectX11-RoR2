#pragma once
#include "Engine_Define.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"

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

#pragma endregion
#pragma region Timer Manager

	void										Tick_Timer();

	HRESULT										Add_Timer(const _float fFPS);
	HRESULT										Delete_Timer(const _float fFPS);
	_bool										Check_Timer(const _float fFPS);
	shared_ptr<class CTimer>					Find_Timer(const _float fFPS);
	_float										Get_TimeDelta(const _float fFPS);

#pragma endregion
#pragma region Graphic Device

	HRESULT										Clear_BackBuffer_View(_float4 vColor);
	HRESULT										Clear_DepthStencil_View();
	HRESULT										Present();

#pragma endregion
#pragma region Scene Manager

	HRESULT										Open_Scene(SCENE, shared_ptr<class CScene>);
	const SCENE									Current_Scene(_bool bSkipLoad = true);

#pragma endregion
#pragma region Object Manager

	HRESULT										Add_Object_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<class CGameObject> pPrototype);
	shared_ptr<class CGameObject>				Clone_GameObject(const SCENE, const wstring& strPrototypeTag, any = any());

	HRESULT										Add_Layer(const SCENE, const wstring& strLayerTag);
	HRESULT										Add_Pool(const SCENE, const wstring& strPoolTag, const wstring& strPrototypeTag, _uint iPoolSize, any = any());

	shared_ptr<class CObjectLayer>				Find_Layer(const SCENE, const wstring& strLayerTag);
	shared_ptr<class CObjectPool>				Find_Pool(const SCENE, const wstring& strPoolTag);

#pragma endregion
#pragma region Component Manager

	HRESULT										Add_Component_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<class CComponent> pPrototype);
	shared_ptr<class CComponent>				Clone_Component(const SCENE, const wstring& strPrototypeTag, any = any());

#pragma endregion
#pragma region Behavior Manager

	HRESULT										Add_Behavior_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<class CBehavior> pPrototype);
	shared_ptr<class CBehavior>					Clone_Behavior(const SCENE, const wstring& strPrototypeTag, any = any());

#pragma endregion

private:
	shared_ptr<class CTimer_Manager>			m_pTimer_Manager;
	shared_ptr<class CGraphicDevice>			m_pGraphic_Device;
	shared_ptr<class CScene_Manager>			m_pScene_Manager;
	shared_ptr<class CObject_Manager>			m_pObject_Manager;
	shared_ptr<class CComponent_Manager>		m_pComponent_Manager;
	shared_ptr<class CBehavior_Manager>			m_pBehavior_Manager;

public:
	static void									Release_Engine();

	friend CSingleton<CGameInstance>;
};

END
