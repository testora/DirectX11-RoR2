#include "ClientPCH.h"
#include "VFX_TrailQuad.h"
#include "GameInstance.h"
#include "Bone.h"

CVFX_TrailQuad::CVFX_TrailQuad(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffect(_pDevice, _pContext)
{
}

CVFX_TrailQuad::CVFX_TrailQuad(const CVFX_TrailQuad& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CVFX_TrailQuad::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINT, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);

	return S_OK;
}

HRESULT CVFX_TrailQuad::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Initialize", "Failed to __super::Initialize");
	}

	m_deqQuad.resize(m_iMaxInstance + 2);

	return S_OK;
}

void CVFX_TrailQuad::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fTimeAcc += _fTimeDelta;
	if (m_fInterval < m_fTimeAcc)
	{
		m_iIndex += static_cast<_uint>(m_fTimeAcc / m_fInterval);
		if (m_iIndex >= m_iMaxInstance)
		{
			m_iIndex = 0;
		}

		m_deqQuad.push_front(make_pair(
			((m_pairTargetPoint.first ? XMLoadFloat4x4(m_pairTargetPoint.first) : g_mUnit) * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).r[3],
			((m_pairTargetPoint.second ? XMLoadFloat4x4(m_pairTargetPoint.second) : g_mUnit) * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).r[3]));

		if (m_pairRelativeLength.first || m_pairRelativeLength.second)
		{
			_float4 vDirection = m_deqQuad.front().second - m_deqQuad.front().first;

			if (m_pairRelativeLength.first)
			{
				m_deqQuad.front().first += vDirection * m_pairRelativeLength.first;
			}
			if (m_pairRelativeLength.second)
			{
				m_deqQuad.front().second = m_deqQuad.front().first + vDirection * m_pairRelativeLength.second;
			}
		}

		if (m_deqQuad.size() > m_iMaxInstance + 1)
		{
			m_deqQuad.pop_back();
		}
	}
}

void CVFX_TrailQuad::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CVFX_TrailQuad::Render()
{
	m_pShader->Bind_Vector(SHADER_MTRLDIF, m_vColor);

	if (FAILED(__super::Render(4)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CVFX_TrailQuad::Fetch(any _pair_pTarget_pair_szBones)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Fetch", "Failed to __super::Fetch");
	}

	Delete_Component(COMPONENT::TEXTURE);
	m_mTargetPivot	= g_mUnit;
	m_vColor		= _color(1.f, 1.f, 1.f, 1.f);
	m_fInterval		= 0.01f;
	m_iMaxInstance	= 100;

	pair<shared_ptr<CGameObject>, pair<const _char*, const _char*>> pairArg;
	pairArg = any_cast<pair<shared_ptr<CGameObject>, pair<const _char*, const _char*>>>(_pair_pTarget_pair_szBones);

	m_pTargetTransform			= pairArg.first->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_pairTargetPoint.first		= pairArg.first->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(pairArg.second.first)->Get_CombinedTransformationPointer();
	m_pairTargetPoint.second	= pairArg.first->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(pairArg.second.second)->Get_CombinedTransformationPointer();
	m_mTargetPivot				= pairArg.first->Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();

	return S_OK;
}

_bool CVFX_TrailQuad::Return()
{
	return false;
}

void CVFX_TrailQuad::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	VTXINSTTRANSCOLORARG* pData = reinterpret_cast<VTXINSTTRANSCOLORARG*>(_pData);
}

void CVFX_TrailQuad::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	if (m_fInterval < m_fTimeAcc)
	{
		m_fTimeAcc = fmodf(m_fTimeAcc, m_fInterval);

		VTXINSTTRANSCOLORARG* pData = reinterpret_cast<VTXINSTTRANSCOLORARG*>(_pData);

		pData[m_iIndex].vColor = _color(1.f, 1.f, 1.f, 0.f);

		for (_uint i = 0; i < m_iMaxInstance; ++i)
		{
			pData[i].vRight			= _float4(m_deqQuad[i].second);
			pData[i].vUp			= _float4(m_deqQuad[i + 1].second);
			pData[i].vLook			= _float4(m_deqQuad[i + 1].first);
			pData[i].vTranslation	= _float4(m_deqQuad[i].first);

			pData[i].vColor.w		= max(0.f, 1.f - (i / static_cast<_float>(m_iMaxInstance)));
			pData[i].vArgument.x	= i / static_cast<_float>(m_iMaxInstance);
		}
	}
}

HRESULT CVFX_TrailQuad::Set_Texture(shared_ptr<CTexture> _pTexture)
{
	if (nullptr == _pTexture)
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Set_Texture", "Nullptr Exception");
	}

	if (FAILED(Add_Component(COMPONENT::TEXTURE, _pTexture)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Set_Texture", "Failed to Add_Component");
	}

	return S_OK;
}

HRESULT CVFX_TrailQuad::Set_Texture(const wstring& _wstrTexture)
{
	shared_ptr<CComponent> pTexture = CGameInstance::Get_Instance()->Clone_Component(CGameInstance::Get_Instance()->Current_Scene(), _wstrTexture);
	if (nullptr == pTexture)
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Set_Texture", "Nullptr Exception");
	}

	if (FAILED(Add_Component(COMPONENT::TEXTURE, pTexture)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Set_Texture", "Failed to Add_Component");
	}

	return S_OK;
}

shared_ptr<CVFX_TrailQuad> CVFX_TrailQuad::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CVFX_TrailQuad> pInstance = make_private_shared(CVFX_TrailQuad, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CVFX_TrailQuad::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CVFX_TrailQuad::Clone(any)
{
	shared_ptr<CVFX_TrailQuad> pInstance = make_private_shared_copy(CVFX_TrailQuad, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CVFX_TrailQuad::Clone", "Failed to Initialize");
	}

	return pInstance;
}
