#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract
{
private:
	explicit CGameObject(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CGameObject() DEFAULT;

public:
	virtual HRESULT					Initialize_Prototype()			PURE;
	virtual HRESULT					Initialize()					PURE;
	virtual void					Tick(_float fTimeDelta)			PURE;
	virtual void					Late_Tick(_float fTimeDelta)	PURE;
	virtual HRESULT					Render()						PURE;

	virtual HRESULT					Fetch_Pool()					PURE;

protected:


protected:
	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pContext;
};

END
