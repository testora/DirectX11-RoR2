#include "ClientPCH.h"
#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTerrain::CTerrain(const CTerrain& _rhs)
	: CGameObject		(_rhs)
	, m_pRendererCom	(_rhs.m_pRendererCom)
	, m_pTransformCom	(_rhs.m_pTransformCom)
	, m_pShaderCom		(_rhs.m_pShaderCom)
	, m_pTextureCom		(_rhs.m_pTextureCom)
	, m_pVIBufferCom	(_rhs.m_pVIBufferCom)
{
}

HRESULT CTerrain::Initialize(any _strHeightMapPath)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::TEXTURE) | BIT(COMPONENT::VIBUFFER_TERRAIN);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX, any());
	m_umapComponentArg[COMPONENT::TEXTURE]			= make_pair(PROTOTYPE_COMPONENT_TEXTURE_TERRAIN, any());
	m_umapComponentArg[COMPONENT::VIBUFFER_TERRAIN]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_TERRAIN, _strHeightMapPath);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CTerrain::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (CGameInstance::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		_float3 vClickPos;

		if (m_pVIBufferCom->Intersect(vClickPos))
		{
//			_float3 vTerrainPos = m_pTransformCom->Get_State(CTransform::POSITION);
//			_float3 vScale = m_pTransformCom->Get_Scale();
//			_float2 vTexUV = _float2(1.f - (vClickPos.x - vTerrainPos.x) / vScale.x, (vClickPos.z - vTerrainPos.z) / vScale.z);
//
//			D3D11_TEXTURE2D_DESC tDesc{};
//
//			tDesc.Width					= 256;
//			tDesc.Height				= 256;
//			tDesc.MipLevels				= 1;
//			tDesc.ArraySize				= 1;
//			tDesc.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;
//
//			tDesc.Usage					= D3D11_USAGE_DYNAMIC;
//			tDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
//			tDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
//			tDesc.MiscFlags				= 0;
//
//			tDesc.SampleDesc.Quality	= 0;
//			tDesc.SampleDesc.Count		= 1;
//
//			D3D11_SUBRESOURCE_DATA SubResourceData{};
//
//			auto arrPixel = Function::CreateDynamicArray<DWORD>(tDesc.Width * tDesc.Height);
//			SubResourceData.pSysMem		= arrPixel.get();
//			SubResourceData.SysMemPitch	= tDesc.Width * 4;
//
//			ComPtr<ID3D11Texture2D> pNewTexture;
//			if (FAILED(m_pDevice->CreateTexture2D(&tDesc, &SubResourceData, pNewTexture.GetAddressOf())))
//			{
//				MSG_RETURN(, "CTerrain::Tick", "Failed to CreateTexture2D");
//			}
//
//			D3D11_MAPPED_SUBRESOURCE tMappedSubResource{};
//
//			if (SUCCEEDED(m_pContext->Map(pNewTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMappedSubResource)))
//			{
//				DWORD* pPixel = reinterpret_cast<DWORD*>(tMappedSubResource.pData);
//
//				pPixel[0] = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//				m_pContext->Unmap(pNewTexture.Get(), 0);
//
//				if (FAILED(m_pTextureCom->Set_Texture2D(pNewTexture, tDesc)))
//				{
//					MSG_RETURN(, "CTerrain::Teck", "Failed to Set_Texture2D");
//				}
//			}
//			else
//			{
//				MSG_RETURN(, "CTerrain::Tick", "Failed to Map");
//			}
		}
	}
}

void CTerrain::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CTerrain::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Render", "Failed to __super::Render");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_WORLD, m_pTransformCom->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_VIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_PROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJ))))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, SHADER_TEXTURE_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Render", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShaderCom->BeginPass(1)))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Render", "Failed to BeginPass");
	}

	if (FAILED(m_pVIBufferCom->Render()))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Render", "Failed to Render");
	}

	return S_OK;
}

HRESULT CTerrain::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTerrain::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransformCom = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransformCom)
	{
		MSG_RETURN(E_FAIL, "CTerrain::Ready_Components", "Nullptr Exception: m_pTransformCom");
	}

	m_pRendererCom = dynamic_pointer_cast<CRenderer>(m_umapComponent[COMPONENT::RENDERER]);
	if (nullptr == m_pRendererCom)
	{
		MSG_RETURN(E_FAIL, "CTerrain::Ready_Components", "Nullptr Exception: m_pRendererCom");
	}

	m_pShaderCom = dynamic_pointer_cast<CShader>(m_umapComponent[COMPONENT::SHADER]);
	if (nullptr == m_pShaderCom)
	{
		MSG_RETURN(E_FAIL, "CTerrain::Ready_Components", "Nullptr Exception: m_pShaderCom");
	}

	m_pTextureCom = dynamic_pointer_cast<CTexture>(m_umapComponent[COMPONENT::TEXTURE]);
	if (nullptr == m_pTextureCom)
	{
		MSG_RETURN(E_FAIL, "CTerrain::Ready_Components", "Nullptr Exception: m_pTextureCom");
	}

	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Terrain>(m_umapComponent[COMPONENT::VIBUFFER_TERRAIN]);
	if (nullptr == m_pVIBufferCom)
	{
		MSG_RETURN(E_FAIL, "CTerrain::Ready_Components", "Nullptr Exception: m_pVIBufferCom");
	}

	return S_OK;
}

shared_ptr<CTerrain> CTerrain::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTerrain> pInstance = make_private_shared(CTerrain, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTerrain::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTerrain::Clone(any _strHeightMapPath)
{
	shared_ptr<CTerrain> pInstance = make_private_shared_copy(CTerrain, *this);

	if (FAILED(pInstance->Initialize(_strHeightMapPath)))
	{
		MSG_RETURN(nullptr, "CTerrain::Clone", "Failed to Initialize");
	}

	return pInstance;
}
