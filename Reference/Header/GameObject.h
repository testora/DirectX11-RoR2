#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract
{
protected:
	explicit CGameObject(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGameObject(const CGameObject&);
	virtual ~CGameObject() DEFAULT;

public:
	virtual HRESULT												Initialize_Prototype();
	virtual HRESULT												Initialize(std::any = nullptr);
	virtual void												Tick(_float fTimeDelta);
	virtual void												Late_Tick(_float fTimeDelta);
	virtual HRESULT												Render();

	virtual HRESULT												Fetch(std::any = nullptr);

protected:
	virtual HRESULT												Ready_Components(bitset<IDX(COMPONENT::MAX)> bitFlag);
	virtual HRESULT												Ready_Behaviors(bitset<IDX(BEHAVIOR::MAX)> bitFlag);

	virtual HRESULT												Add_Component(const COMPONENT);
	virtual HRESULT												Add_Behavior(const BEHAVIOR);
	virtual HRESULT												Delete_Component(const COMPONENT);
	virtual HRESULT												Delete_Behavior(const BEHAVIOR);

protected:
	unordered_map<COMPONENT, shared_ptr<class CComponent>>		m_umapComponent;
	bitset<IDX(COMPONENT::MAX)>									m_bitComponent;

	unordered_map<BEHAVIOR, shared_ptr<class CBehavior>>		m_umapBehavior;
	bitset<IDX(BEHAVIOR::MAX)>									m_bitBehavior;

protected:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;

public:
	virtual shared_ptr<CGameObject>								Clone(std::any = nullptr)	PURE;
};

END
