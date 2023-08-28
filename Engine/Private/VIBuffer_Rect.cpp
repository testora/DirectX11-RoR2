#include "EnginePCH.h"
#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::VIBUFFER_RECT)
{
}

HRESULT CVIBuffer_Rect::Initialize(any)
{
	m_iNumVB		= 1;
	m_iNumVertices	= 4;
	m_iVertexStride	= sizeof(VTXPOSTEX);
	m_iNumIndices	= 6;
	m_iIndexStride	= sizeof(_uint);
	m_eIndexFormat	= DXGI_FORMAT_R32_UINT;
	m_eTopology		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pVertices		= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices		= Function::CreateDynamicArray<_uint>(m_iNumIndices);

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXPOSTEX>(m_iNumVertices);

	pVertices[0].vPosition	= m_pVertices[0] = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexCoord	= _float2(0.f, 0.f);

	pVertices[1].vPosition	= m_pVertices[1] = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexCoord	= _float2(1.f, 0.f);

	pVertices[2].vPosition	= m_pVertices[2] = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexCoord	= _float2(1.f, 1.f);

	pVertices[3].vPosition	= m_pVertices[3] = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexCoord	= _float2(0.f, 1.f);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumIndices * m_iIndexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	auto pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	pIndices[0]	= 0;
	pIndices[1]	= 1;
	pIndices[2]	= 2;

	pIndices[3]	= 0;
	pIndices[4]	= 2;
	pIndices[5]	= 3;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pIndices.get();

	memcpy(m_pIndices.get(), pIndices.get(), m_iNumIndices * m_iIndexStride);

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to Initialize");
	}

	return S_OK;
}

shared_ptr<CVIBuffer_Rect> CVIBuffer_Rect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CVIBuffer_Rect> pInstance = make_private_shared(CVIBuffer_Rect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CVIBuffer_Rect::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBuffer_Rect::Clone(any _arg)
{
	return shared_from_this();
}
