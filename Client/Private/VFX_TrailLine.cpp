#include "ClientPCH.h"
#include "VFX_TrailLine.h"
#include "GameInstance.h"
#include "Bone.h"

CVFX_TrailLine::CVFX_TrailLine(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffect(_pDevice, _pContext)
{
}

CVFX_TrailLine::CVFX_TrailLine(const CVFX_TrailLine& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CVFX_TrailLine::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::TEXTURE)	| BIT(COMPONENT::VIBUFFER_INSTANCE_LINE);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINE, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE]					= make_pair(PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_LINE]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE, g_aNull);

	return S_OK;
}

HRESULT CVFX_TrailLine::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Initialize", "Failed to __super::Initialize");
	}

	m_deqLine.resize(m_iMaxInstance + 3);
	
	return S_OK;
}

void CVFX_TrailLine::Tick(_float _fTimeDelta)
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

		m_deqLine.push_front(((m_pTargetPoint ? XMLoadFloat4x4(m_pTargetPoint) : g_mUnit) * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).r[3]);
		if (m_deqLine.size() > m_iMaxInstance + 2)
		{
			m_deqLine.pop_back();
		}
	}
}

void CVFX_TrailLine::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CVFX_TrailLine::Render()
{
	m_pShader->Bind_Vector(SHADER_MTRLDIF, _float4(0.2f, 0.6f, 0.8f, 1.f));
	m_pShader->Bind_Float(SHADER_THICKNESS, 0.05f);

	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CVFX_TrailLine::Fetch(any _pair_pTarget_szBone)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Fetch", "Failed to __super::Fetch");
	}

	m_mTargetPivot	= g_mUnit;
	m_fInterval		= 0.01f;
	m_iMaxInstance	= 100;

	pair<shared_ptr<CGameObject>, const _char*> pairArg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_pair_pTarget_szBone);

	m_pTargetTransform = pairArg.first->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_pTargetPoint = pairArg.first->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(pairArg.second)->Get_CombinedTransformationPointer();
	m_mTargetPivot = pairArg.first->Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();

	return S_OK;
}

_bool CVFX_TrailLine::Return()
{
	return false;
}

void CVFX_TrailLine::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	VTXINSTTRANSCOLOR* pData = reinterpret_cast<VTXINSTTRANSCOLOR*>(_pData);
}

void CVFX_TrailLine::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	if (m_fInterval < m_fTimeAcc)
	{
		m_fTimeAcc = fmodf(m_fTimeAcc, m_fInterval);

		VTXINSTTRANSCOLOR* pData = reinterpret_cast<VTXINSTTRANSCOLOR*>(_pData);

		pData[m_iIndex].vColor = _color(1.f, 1.f, 1.f, 0.f);

		for (_uint i = 0; i < m_iMaxInstance; ++i)
		{
			pData[i].vRight			= _float4(m_deqLine[i + 0]);
			pData[i].vUp			= _float4(m_deqLine[i + 1]);
			pData[i].vLook			= _float4(m_deqLine[i + 2]);
			pData[i].vTranslation	= _float4(m_deqLine[i + 3]);

			pData[i].vColor.w		= max(0.f, 1.f - (i / static_cast<_float>(m_iMaxInstance)));
		}
	}
}

shared_ptr<CVFX_TrailLine> CVFX_TrailLine::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CVFX_TrailLine> pInstance = make_private_shared(CVFX_TrailLine, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CVFX_TrailLine::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CVFX_TrailLine::Clone(any _pTarget)
{
	shared_ptr<CVFX_TrailLine> pInstance = make_private_shared_copy(CVFX_TrailLine, *this);

	if (FAILED(pInstance->Initialize(_pTarget)))
	{
		MSG_RETURN(nullptr, "CVFX_TrailLine::Clone", "Failed to Initialize");
	}

	return pInstance;
}
