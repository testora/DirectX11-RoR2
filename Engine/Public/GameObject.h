#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract
{
protected:
	explicit CGameObject(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CGameObject() DEFAULT;

public:
	virtual HRESULT												Initialize_Prototype()			PURE;
	virtual HRESULT												Initialize(void* pArg)			PURE;
	virtual void												Tick(_float fTimeDelta)			PURE;
	virtual void												Late_Tick(_float fTimeDelta)	PURE;
	virtual HRESULT												Render()						PURE;

	virtual HRESULT												Fetch_Pool()					PURE;

private:
	unordered_map<COMPONENT, shared_ptr<class CComponent>>		m_mapComponent;
	bitset<IDX(COMPONENT::MAX)>									m_bitComponent;

	unordered_map<BEHAVIOR, shared_ptr<class CBehavior>>		m_mapBehavior;
	bitset<IDX(BEHAVIOR::MAX)>									m_bitBehavior;

protected:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;
};

END
