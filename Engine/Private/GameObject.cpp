#include "EnginePCH.h"
#include "GameObject.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"
#include "Scene_Manager.h"

CGameObject::CGameObject(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: m_pDevice	(_pDevice)
	, m_pContext(_pContext)
{
	m_bitComponent.reset();
	m_bitBehavior.reset();

	m_umapComponentArg.reserve(IDX(COMPONENT::MAX));
	m_umapBehaviorArg.reserve(IDX(BEHAVIOR::MAX));

	for (_uint i = 0; i < IDX(COMPONENT::MAX); ++i)
	{
		m_umapComponentArg[static_cast<COMPONENT>(i)] = make_pair(wstring(), any());
	}
	for (_uint i = 0; i < IDX(BEHAVIOR::MAX); ++i)
	{
		m_umapBehaviorArg[static_cast<BEHAVIOR>(i)] = make_pair(wstring(), any());
	}
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_pDevice		(_rhs.m_pDevice)
	, m_pContext	(_rhs.m_pContext)
	, m_bitComponent(_rhs.m_bitComponent)
	, m_bitBehavior	(_rhs.m_bitBehavior)
{
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(any _arg)
{
	if (FAILED(Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Initialize", "Failed to Ready_Components");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Initialize", "Failed to Ready_Behaviors");
	}

	return S_OK;
}

void CGameObject::Tick(_float _fTimeDelta)
{
	for(auto& iter : m_umapBehavior)
	{
		iter.second->Tick(_fTimeDelta);
	}
}

void CGameObject::Late_Tick(_float _fTimeDelta)
{
	for (auto& iter : m_umapBehavior)
	{
		iter.second->Late_Tick(_fTimeDelta);
	}
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Fetch(any _arg)
{
	return S_OK;
}

HRESULT CGameObject::Ready_Components()
{
	HRESULT hr = S_OK;

	for (size_t i = 0; i < IDX(COMPONENT::MAX); ++i)
	{
		if (m_bitComponent.test(i))
		{
			if (FAILED(Add_Component(static_cast<COMPONENT>(i))))
			{
				hr = E_FAIL;
			}
		}
	}

	if (FAILED(hr))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Ready_Components", "Failed to Add_Component");
	}

	return S_OK;
}

HRESULT CGameObject::Ready_Behaviors()
{
	HRESULT hr = S_OK;

	for (size_t i = 0; i < IDX(BEHAVIOR::MAX); ++i)
	{
		if (m_bitBehavior.test(i))
		{
			if (FAILED(Add_Behavior(static_cast<BEHAVIOR>(i))))
			{
				hr = E_FAIL;
			}
		}
	}

	if (FAILED(hr))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Ready_Behaviors", "Failed to Add_Behavior");
	}

	return S_OK;
}

HRESULT CGameObject::Add_Component(const COMPONENT _eComponent)
{
	switch (_eComponent)
	{
	case COMPONENT::RENDERER:
		m_umapComponent.emplace(_eComponent, CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Static_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	case COMPONENT::TRANSFORM:
		m_umapComponent.emplace(_eComponent, CTransform::Create(m_pDevice, m_pContext));
		break;

	case COMPONENT::SHADER:
		m_umapComponent.emplace(_eComponent, CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Static_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	case COMPONENT::TEXTURE:
		m_umapComponent.emplace(_eComponent, CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Current_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	case COMPONENT::VIBUFFER_RECT:
		m_umapComponent.emplace(_eComponent, CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Static_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	default:
		MSG_RETURN(E_FAIL, "CGameObject::Add_Component", "Invalid Range");
	}

	m_bitComponent.set(IDX(_eComponent), true);

	return S_OK;
}

HRESULT CGameObject::Add_Behavior(const BEHAVIOR _eBehavior)
{
	switch (_eBehavior)
	{
	case BEHAVIOR::PHYSICS:
		m_umapBehavior.emplace(_eBehavior, CPhysics::Create(m_pDevice, m_pContext));
		break;

	default:
		MSG_RETURN(E_FAIL, "CGameObject::Add_Component", "Invalid Range");
	}

	m_bitBehavior.set(IDX(_eBehavior), true);

	return S_OK;
}

HRESULT CGameObject::Delete_Component(const COMPONENT _eComponent)
{
	auto iter = m_umapComponent.find(_eComponent);
	if (iter == m_umapComponent.end())
	{
		return S_FALSE;
	}

	m_umapComponent.erase(iter);
	m_bitComponent.set(IDX(_eComponent), false);

	return S_OK;
}

HRESULT CGameObject::Delete_Behavior(const BEHAVIOR _eBehavior)
{
	auto iter = m_umapBehavior.find(_eBehavior);
	if (iter == m_umapBehavior.end())
	{
		return S_FALSE;
	}

	m_umapBehavior.erase(iter);
	m_bitBehavior.set(IDX(_eBehavior), false);

	return S_OK;
}
