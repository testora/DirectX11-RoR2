#pragma once
#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public ISystem
{
protected:
	explicit CComponent(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const COMPONENT);
	explicit CComponent(const CComponent&);
	virtual ~CComponent() DEFAULT;

public:
	const COMPONENT						Type() const	{ return m_eType; }

protected:
	inline shared_ptr<CComponent>		shared_from_componenet();

private:
	const COMPONENT						m_eType;

protected:
	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;

public:
	virtual shared_ptr<CComponent>		Clone(any = g_aNull)	PURE;
};

END
