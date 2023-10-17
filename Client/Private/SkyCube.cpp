#include "ClientPCH.h"
#include "SkyCube.h"
#include "GameInstance.h"

CSkyCube::CSkyCube(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CSkyCube::CSkyCube(const CSkyCube& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CSkyCube::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_CUBE);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXCUBEPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_CUBE]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_CUBE, g_aNull);

	return S_OK;
}

HRESULT CSkyCube::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CSkyCube::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CSkyCube::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::PRIORITY);

#if ACTIVATE_TOOL
//	if (CImGui_Manager::Get_Instance()->Is_Enable())
//	{
//
//		static _float4 vDiffuse = _color(1.f, 1.f, 1.f, 1.f);
//		ImGui::Begin("SkyBox Diffuse");
//		ImGui::InputFloat4("Diffuse", reinterpret_cast<_float*>(&vDiffuse));
//		if (ImGui::Button("Apply"))
//		{
//			m_tMaterialDesc.vDiffuse = vDiffuse;
//		}
//		ImGui::End();
//	}
#endif
}

HRESULT CSkyCube::Render()
{
	m_pTransform->Set_State(TRANSFORM::POSITION, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION));

	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texCubeDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}

	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CSkyShpere::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CSkyCube::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Ready_Components", "Failed to Get_Component");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Ready_Components", "Failed to Get_Component");
	}

	m_pTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_COMPONENT_TEXTURE_SKY0));
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Ready_Components", "Failed to Clone_Component");
	}

	return S_OK;
}

shared_ptr<CSkyCube> CSkyCube::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CSkyCube> pInstance = make_private_shared(CSkyCube, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CSkyCube::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CSkyCube::Clone(any)
{
	shared_ptr<CSkyCube> pInstance = make_private_shared_copy(CSkyCube, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CSkyCube::Create", "Failed to Initialize");
	}

	return pInstance;
}
