#pragma once
#include "Engine_Define.h"
#include "Singleton.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CSingleton<CGameInstance>
{
	friend class CSingleton<CGameInstance>;
private:
	explicit CGameInstance();
	virtual ~CGameInstance() DEFAULT;

public:
#pragma region Engine

	HRESULT			Initialize_Engine(_In_ const GRAPHICDESC, _Out_ ComPtr<ID3D11Device>&, _Out_ ComPtr<ID3D11DeviceContext>&);
	void			Tick_Engine(_float fTimeDelta);

#pragma endregion
#pragma region Graphic Device

	HRESULT			Clear_BackBuffer_View(_float4 vColor);
	HRESULT			Clear_DepthStencil_View();
	HRESULT			Present();

#pragma endregion

private:
	shared_ptr<class CGraphicDevice>	m_pGraphicDevice;

public:
	static void		Release_Engine();
};

END
