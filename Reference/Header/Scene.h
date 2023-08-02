#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract
{
protected:
	explicit CScene(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const SCENE);
	virtual ~CScene() DEFAULT;

public:
	virtual HRESULT												Initialize();
	virtual void												Tick(_float fTimeDelta);
	virtual void												Late_Tick(_float fTimeDelta);
	virtual HRESULT												Render();

protected:
	const SCENE													m_eScene;

private:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;
};

END
