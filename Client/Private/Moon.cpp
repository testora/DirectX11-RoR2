#include "ClientPCH.h"
#include "Moon.h"
#include "GameInstance.h"

CMoon::CMoon(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMoon::CMoon(const CMoon& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CMoon::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESH, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_MOON, g_aNull);

	return S_OK;
}

HRESULT CMoon::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMoon::Initialize", "Failed To __super::Initialize");
	}

	LIGHTDESC tLightDesc{};
	tLightDesc.eLightType	= LIGHTTYPE::SHADOW;
	tLightDesc.eShadowType	= SHADOWTYPE::STATIC;
	tLightDesc.vPosition	= ARENA_CENTER + _float3(0.f, 100.f, 0.f);
	tLightDesc.vPosition	= _float3(-1600.f, 1500.f, 900.f);
	tLightDesc.vDirection	= _float3(-0.64f, -0.76f, -0.12f);
//	tLightDesc.vDirection	= _float3(0.01f, -1.f, 0.01f);
//	tLightDesc.vDirection	= _float3(-0.64f, 0.f, -0.12f);
	tLightDesc.fRange		= 10.f;
	CGameInstance::Get_Instance()->Add_Light(SCENE::MOON, tLightDesc, nullptr, shared_from_gameobject());

	return S_OK;
}

void CMoon::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

#if ACTIVATE_IMGUI
	ImGui::Begin("Moon Render");
	for (_uint i = 0; i < m_pModel->Get_NumMeshes(); ++i)
	{
		std::string checkboxLabel = "Mesh " + std::to_string(i);
		_bool isHidden = m_pModel->Is_MeshHidden(i);
		if (ImGui::Checkbox(checkboxLabel.c_str(), &isHidden))
		{
			m_pModel->Hide_Mesh(i, isHidden);
		}
	}
	ImGui::End();
	ImGui::Begin("Deferred Offset");
	static float fBias = -1.f;
	ImGui::DragFloat("Bias", &fBias);
	m_pRenderer->Get_DeferredShader()->Bind_Float("fBias", fBias);
	ImGui::End();
#endif
}

void CMoon::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::SHADOW);
	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CMoon::Render()
{
	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CMoon::Render", "Failed to __super::Render");
	}

//	if (CGameInstance::Get_Instance()->Key_Hold('8'))
//	{
//		__super::Render(0);
//	}
//	else
//	{
//		static int i = 0;
//		if (CGameInstance::Get_Instance()->Key_Down('9'))
//		{
//			--i;
//		}
//		if (CGameInstance::Get_Instance()->Key_Down('0'))
//		{
//			++i;
//		}
//
//		if (shared_ptr<CShader> pShader = m_pShader)
//		{
//			if (shared_ptr<CTransform> pTransform = m_pTransform)
//			{
//				if (FAILED(pTransform->Bind_OnShader(pShader)))
//				{
//					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CTransform::Bind_OnShader");
//				}
//			}
//			if (shared_ptr<CVIBuffer> pVIBuffer = Get_Component<CVIBuffer>(COMPONENT::VIBUFFER))
//			{
//				if (FAILED(pVIBuffer->Render(pShader, 1)))
//				{
//					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CVIBuffer::Render");
//				}
//			}
//			if (shared_ptr<CModel> pModel = Get_Component<CModel>(COMPONENT::MODEL))
//			{
//				if (FAILED(pModel->Bind_ShaderResourceViews(i, m_pShader)))
//				{
//					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CModel::Bind_ShaderResourceViews");
//				}
//
//				if (FAILED(pModel->Render(i, m_pShader, 1)))
//				{
//					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CModel::Render");
//				}
//			}
//		}
//	}
	
	return S_OK;
}

HRESULT CMoon::Render_ShadowDepth()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMoon::Render_ShadowDepth", "Failed to Bind_OnShader");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 3)))
	{
		MSG_RETURN(E_FAIL, "CMoon::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMoon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Nullptr Exception: m_pRenderer");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Nullptr Exception: m_pShader");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Nullptr Exception: m_pModel");
	}

	return S_OK;
}

shared_ptr<CMoon> CMoon::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMoon> pInstance = make_private_shared(CMoon, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMoon::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMoon::Clone(any)
{
	shared_ptr<CMoon> pInstance = make_private_shared_copy(CMoon, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CMoon::Create", "Failed to Initialize");
	}

	return pInstance;
}
