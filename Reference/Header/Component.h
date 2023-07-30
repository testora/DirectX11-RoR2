#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract
{
protected:
	explicit CComponent(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const COMPONENT);
	virtual ~CComponent() DEFAULT;

public:
	const COMPONENT						Type() const	{ return m_eType; }

private:
	const COMPONENT						m_eType;

protected:
	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;
};

END
