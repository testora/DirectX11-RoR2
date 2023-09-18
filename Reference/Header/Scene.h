#pragma once
#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public ISystem
{
protected:
	explicit CScene(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const SCENE);
	virtual ~CScene() DEFAULT;

public:
	virtual HRESULT					Initialize()					PURE;
	virtual void					Tick(_float fTimeDelta)			PURE;
	virtual void					Late_Tick(_float fTimeDelta)	PURE;
	virtual HRESULT					Render()						PURE;

public:
	virtual const SCENE				Get_Scene(_bool _bSkipLoad = true) const	{ return m_eScene; }

protected:
	const SCENE						m_eScene;

protected:
	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pContext;
};

END
