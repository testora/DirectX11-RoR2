#pragma once
#include "Client_Define.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CSingleton<CMainApp>
{
	friend CSingleton<CMainApp>;
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Initialize();
	void		Tick(_float fTimeDelta);
	HRESULT		Render();

#pragma region Timer Management

	void		Tick_Timer();
	_bool		Check_Timer(const _float fFPS);
	_float		Get_TimeDelta(const _float fFPS) const;

#pragma endregion

private:
	HRESULT		Default_Settings();

private:
	ComPtr<ID3D11Device>		m_pDevice;
	ComPtr<ID3D11DeviceContext>	m_pContext;
	shared_ptr<CGameInstance>	m_pGameInstance;
};

END
