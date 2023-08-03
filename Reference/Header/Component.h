#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract
{
protected:
	explicit CComponent(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const COMPONENT);
	explicit CComponent(const CComponent&);
	virtual ~CComponent() DEFAULT;

public:
	const COMPONENT						Type() const	{ return m_eType; }

private:
	const COMPONENT						m_eType;

protected:
	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;

public:
	virtual shared_ptr<CComponent>		Clone(any = any())	PURE;
};

END
