#pragma once
#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public ISystem
{
protected:
	explicit CGameObject(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGameObject(const CGameObject&);
	virtual ~CGameObject() DEFAULT;

public:
	virtual HRESULT												Initialize_Prototype();
	virtual HRESULT												Initialize(any = g_aNull);
	virtual void												Tick(_float fTimeDelta);
	virtual void												Late_Tick(_float fTimeDelta);
	virtual HRESULT												Render()			PURE;

	virtual HRESULT												Fetch(any = g_aNull);

public:
	template <typename T>
	shared_ptr<T>												Get_Component(const COMPONENT);
	template <typename T>
	shared_ptr<T>												Get_Behavior(const BEHAVIOR);

protected:
	virtual HRESULT												Render(_uint iPassIndex);

	virtual HRESULT												Ready_Components();
	virtual HRESULT												Ready_Behaviors();

	virtual HRESULT												Add_Component(const COMPONENT);
	virtual HRESULT												Add_Behavior(const BEHAVIOR);
	virtual HRESULT												Delete_Component(const COMPONENT);
	virtual HRESULT												Delete_Behavior(const BEHAVIOR);

	HRESULT														Add_RenderObject(const RENDER_GROUP);

protected:
	inline shared_ptr<CGameObject>								shared_from_gameobject();

protected:
	ENTITYDESC													m_tEntityDesc{};

	unordered_map<COMPONENT, shared_ptr<class CComponent>>		m_umapComponent;
	unordered_map<COMPONENT, pair<wstring, any>>				m_umapComponentArg;
	bitset<IDX(COMPONENT::MAX)>									m_bitComponent;

	unordered_map<BEHAVIOR, shared_ptr<class CBehavior>>		m_umapBehavior;
	unordered_map<BEHAVIOR, pair<wstring, any>>					m_umapBehaviorArg;
	bitset<IDX(BEHAVIOR::MAX)>									m_bitBehavior;

private:
	weak_ptr<class CRenderer>									m_pWeakRenderer;
	weak_ptr<class CTransform>									m_pWeakTransform;
	weak_ptr<class CShader>										m_pWeakShader;
	weak_ptr<class CCollider>									m_pWeakCollider;
	weak_ptr<class CModel>										m_pWeakModel;
	weak_ptr<class CVIBuffer>									m_pWeakVIBuffer;

protected:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;

public:
	virtual shared_ptr<CGameObject>								Clone(any = g_aNull)	PURE;
};

#include "GameObject.inl"

END
