#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBehavior abstract
{
protected:
	explicit CBehavior(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const BEHAVIOR);
	virtual ~CBehavior() DEFAULT;

public:
	const BEHAVIOR				Type() const	{ return m_eType; }

private:
	const BEHAVIOR				m_eType;

protected:
	ComPtr<ID3D11Device>        m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pContext;
};

END
