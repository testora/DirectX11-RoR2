#include "EnginePCH.h"
#include "GameObject.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"
#include "Scene_Manager.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Model.h"

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

	m_tMaterialDesc.vDiffuse	= _color(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vAmbient	= _color(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vSpecular	= _color(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vEmissive	= _color(0.f, 0.f, 0.f, 0.f);
	m_tMaterialDesc.fShininess	= 32.f;
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_pDevice			(_rhs.m_pDevice)
	, m_pContext		(_rhs.m_pContext)
	, m_tCharacterDesc	(_rhs.m_tCharacterDesc)
	, m_bitComponent	(_rhs.m_bitComponent)
	, m_bitBehavior		(_rhs.m_bitBehavior)
	, m_umapComponentArg(_rhs.m_umapComponentArg)
	, m_umapBehaviorArg	(_rhs.m_umapBehaviorArg)
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

HRESULT CGameObject::Render(_uint _iPassIndex)
{
	if (shared_ptr<CShader> pShader = m_pShader.lock())
	{
		if (shared_ptr<CTransform> pTransform = m_pTransform.lock())
		{
			if (FAILED(pShader->Bind_Matrix(SHADER_MATWORLD, pTransform->Get_Matrix())))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Matrix");
			}
			if (FAILED(pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Matrix");
			}
			if (FAILED(pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJ))))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Matrix");
			}
		}

		if (FAILED(pShader->Bind_Vector(SHADER_MTRLDIF, m_tMaterialDesc.vDiffuse)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Vector: SHADER_MTRLDIF");
		}
		if (FAILED(pShader->Bind_Vector(SHADER_MTRLAMB, m_tMaterialDesc.vAmbient)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Vector: SHADER_MTRLDIF");
		}
		if (FAILED(pShader->Bind_Vector(SHADER_MTRLSPC, m_tMaterialDesc.vSpecular)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Vector: SHADER_MTRLDIF");
		}
		if (FAILED(pShader->Bind_Vector(SHADER_MTRLEMS, m_tMaterialDesc.vEmissive)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Vector: SHADER_MTRLDIF");
		}
		if (FAILED(pShader->Bind_RawValue(SHADER_MTRLSHN, &m_tMaterialDesc.fShininess, sizeof m_tMaterialDesc.fShininess)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_RawValue: SHADER_MTRLSHN");
		}

		if (shared_ptr<CModel> pModel = m_pModel.lock())
		{
			for (_uint i = 0; i < pModel->Get_NumMeshes(); ++i)
			{
				if (FAILED(pModel->Bind_ShaderResourceViews(i, pShader, aiTextureType_DIFFUSE, SHADER_TEXDIF)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_ShaderResourceViews");
				}

				if (FAILED(pModel->Bind_BoneMatrices(i, pShader, SHADER_BONE)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_BoneMatrices");
				}

				if (FAILED(pShader->BeginPass(_iPassIndex)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to BeginPass");
				}

				if (FAILED(pModel->Render(i)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Render");
				}
			}
		}
		else
		{
			if (shared_ptr<CVIBuffer> pVIBuffer = m_pVIBuffer.lock())
			{
				if (FAILED(pShader->BeginPass(_iPassIndex)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to BeginPass");
				}

				if (FAILED(pVIBuffer->Render()))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Render");
				}
			}

		}
	}

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
	case COMPONENT::TRANSFORM:
		m_umapComponent.emplace(_eComponent, CTransform::Create(m_pDevice, m_pContext));
		break;

	case COMPONENT::RENDERER:
	case COMPONENT::SHADER:
	case COMPONENT::VIBUFFER_RECT:
		m_umapComponent.emplace(_eComponent, CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Static_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	case COMPONENT::TEXTURE:
	case COMPONENT::MODEL:
	case COMPONENT::MESH:
	case COMPONENT::VIBUFFER_TERRAIN:
		m_umapComponent.emplace(_eComponent, CComponent_Manager::Get_Instance()->Clone_Component(CScene_Manager::Get_Instance()->Current_Scene(),
			m_umapComponentArg[_eComponent].first, m_umapComponentArg[_eComponent].second));
		break;

	default:
		MSG_RETURN(E_FAIL, "CGameObject::Add_Component", "Invalid Range");
	}

	switch (_eComponent)
	{
	case COMPONENT::MESH:
	case COMPONENT::VIBUFFER_RECT:
	case COMPONENT::VIBUFFER_TERRAIN:
		m_umapComponent.emplace(COMPONENT::VIBUFFER, m_umapComponent[_eComponent]);
		m_bitComponent.set(IDX(COMPONENT::VIBUFFER), true);
		break;
	}

	switch (_eComponent)
	{
	case COMPONENT::SHADER:
		m_pShader		= Get_Component<CShader>(_eComponent);
		break;
	case COMPONENT::TRANSFORM:
		m_pTransform	= Get_Component<CTransform>(_eComponent);
		break;
	case COMPONENT::MESH:
	case COMPONENT::VIBUFFER_RECT:
	case COMPONENT::VIBUFFER_TERRAIN:
		m_pVIBuffer		= Get_Component<CVIBuffer>(_eComponent);
		break;
	case COMPONENT::MODEL:
		m_pModel		= Get_Component<CModel>(_eComponent);
		break;
	}

	m_bitComponent.set(IDX(_eComponent), true);

	return S_OK;
}

HRESULT CGameObject::Add_Behavior(const BEHAVIOR _eBehavior)
{
	switch (_eBehavior)
	{
	case BEHAVIOR::PHYSICS:
		m_umapBehavior.emplace(_eBehavior, CPhysics::Create(m_pDevice, m_pContext, shared_from_this()));
		break;

	case BEHAVIOR::CONTROL:
		m_umapBehavior.emplace(_eBehavior, CControl::Create(m_pDevice, m_pContext, shared_from_this(), &m_tCharacterDesc));
		break;

	case BEHAVIOR::GROUNDING:
		m_umapBehavior.emplace(_eBehavior, CGrounding::Create(m_pDevice, m_pContext, shared_from_this()));
		break;

	default:
		MSG_RETURN(E_FAIL, "CGameObject::Add_Behavior", "Invalid Range");
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

	switch (_eComponent)
	{
	case COMPONENT::MESH:
	case COMPONENT::VIBUFFER_RECT:
	case COMPONENT::VIBUFFER_TERRAIN:
		m_umapComponent.erase(m_umapComponent.find(COMPONENT::VIBUFFER));
		m_bitComponent.set(IDX(COMPONENT::VIBUFFER), false);

	default:
		m_umapComponent.erase(iter);
		m_bitComponent.set(IDX(_eComponent), false);
		break;
	}

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
