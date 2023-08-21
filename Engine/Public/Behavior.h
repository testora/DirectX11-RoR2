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
	HRESULT							Initialize(any pOwner);
	virtual void					Tick(_float fTimeDelta);
	virtual void					Late_Tick(_float fTimeDelta);

public:
	const BEHAVIOR					Type() const	{ return m_eType; }

protected:
	weak_ptr<class CGameObject>		m_pOwner;

	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pContext;

private:
	const BEHAVIOR					m_eType;

public:
	virtual shared_ptr<CBehavior>	Clone(any = any())	PURE;
};

END
