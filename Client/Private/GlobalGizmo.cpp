#include "ClientPCH.h"
#include "GlobalGizmo.h"
#include "GameInstance.h"

CGlobalGizmo::CGlobalGizmo(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CGlobalGizmo::CGlobalGizmo(const CGlobalGizmo& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CGlobalGizmo::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT, g_aNull);

	return S_OK;
}

HRESULT CGlobalGizmo::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CGlobalGizmo::Initialize", "Failed to __super::Initialize");
	}

	shared_ptr<CTransform> pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	pTransform->LookTo(_float3(0.f, 1.f, 0.f), false);
	Initialize_Gizmo(_int2(10, 10));

	if (FAILED(Add_Component(COMPONENT::TEXTURE, CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Gizmo.png")))))
	{
		MSG_RETURN(E_FAIL, "CGlobalGizmo::Initialize", "Failed to Add_Component");
	}

	return S_OK;
}

void CGlobalGizmo::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CGlobalGizmo::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CGlobalGizmo::Render()
{
	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CGlobalGizmo::Render", "Failed to __super::Render");
	}

	return S_OK;
}

void CGlobalGizmo::Initialize_Gizmo(_int2 _vGridSize)
{
	shared_ptr<CVIBufferInstance_Rect> pVIBuffer = Get_Component<CVIBufferInstance_Rect>(COMPONENT::VIBUFFER_INSTANCE_RECT);
	pVIBuffer->Update(
		[&](void* _pData, _uint iNumInstance)
		{
			VTXPOSSIZEINSTTRANS* pData = reinterpret_cast<VTXPOSSIZEINSTTRANS*>(_pData);
			_int2 vGridSize = _int2(std::clamp(_vGridSize.x, 1, 100), std::clamp(_vGridSize.y, 1, 100));
			size_t iInstance = 0;
			ZeroMemory(pData, sizeof(VTXPOSSIZEINSTTRANS) * iNumInstance);
			for (size_t x = 0; x < vGridSize.x; ++x)
			{
				for (size_t y = 0; y < vGridSize.y; ++y)
				{
					_float4x4 mTransformation = g_mUnit;
					mTransformation._41 = (1.f - vGridSize.x) * 0.5f + static_cast<_float>(x);
					mTransformation._42 = (vGridSize.y - 1.f) * 0.5f - static_cast<_float>(y);
					memcpy(&pData[iInstance++], &mTransformation, sizeof(_float4x4));
				}
			}
		}
	);
}

shared_ptr<CGlobalGizmo> CGlobalGizmo::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CGlobalGizmo> pInstance = make_private_shared(CGlobalGizmo, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		return nullptr;
	}

	return pInstance;
}

shared_ptr<CGameObject> CGlobalGizmo::Clone(any)
{
	shared_ptr<CGlobalGizmo> pInstance = make_private_shared_copy(CGlobalGizmo, *this);

	if (FAILED(pInstance->Initialize()))
	{
		return nullptr;
	}

	return pInstance;
}
