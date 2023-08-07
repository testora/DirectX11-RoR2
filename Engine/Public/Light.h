#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight final
{
private:
	explicit CLight(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CLight() DEFAULT;

public:
	HRESULT						Initialize(const LIGHTDESC&);

private:
	LIGHTDESC					m_tLightDesc;

	ComPtr<ID3D11Device>		m_pDevice;
	ComPtr<ID3D11DeviceContext>	m_pContext;

public:
	static shared_ptr<CLight>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const LIGHTDESC&);
};

END
