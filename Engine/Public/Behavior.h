#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBehavior abstract
{
protected:
	explicit CBehavior(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const BEHAVIOR);
	explicit CBehavior(const CBehavior&);
	virtual ~CBehavior() DEFAULT;

public:
	virtual HRESULT					Initialize(any = any());
	virtual void					Tick(_float fTimeDelta);
	virtual void					Late_Tick(_float fTimeDelta);

public:
	const BEHAVIOR					Type() const	{ return m_eType; }

private:
	const BEHAVIOR					m_eType;

protected:
	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pContext;

public:
	virtual shared_ptr<CBehavior>	Clone(any = any())	PURE;
};

END
