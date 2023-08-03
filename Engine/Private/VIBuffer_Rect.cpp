#include "EnginePCH.h"
#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::VIBUFFER_RECT)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& _rhs)
	: CVIBuffer(_rhs)
{
}

HRESULT CVIBuffer_Rect::Initialize()
{
	m_iNumVB		= 1;
	m_iNumVertex	= 4;
	m_iVertexStride	= sizeof(VTXPOSTEX);
	m_iNumIndex	= 6;
	m_iIndexStride	= sizeof(_ushort);
	m_eIndexFormat	= DXGI_FORMAT_R16_UINT;
	m_eTopology		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumVertex * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::MakeUniqueDynamicArray<VTXPOSTEX>(m_iNumVertex);
	ZeroMemory(pVertices.get(), m_iVertexStride * m_iNumVertex);

	pVertices[0].vPosition	= _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexCoord	= _float2(0.f, 0.f);

	pVertices[1].vPosition	= _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexCoord	= _float2(1.f, 0.f);

	pVertices[2].vPosition	= _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexCoord	= _float2(1.f, 1.f);

	pVertices[3].vPosition	= _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexCoord	= _float2(0.f, 1.f);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, &m_pVB)))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumIndex * m_iIndexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	auto pIndices = Function::MakeUniqueDynamicArray<_ushort>(m_iNumIndex);
	ZeroMemory(pIndices.get(), m_iIndexStride * m_iNumIndex);

	pIndices[0]	= 0;
	pIndices[1]	= 1;
	pIndices[2]	= 2;

	pIndices[3]	= 0;
	pIndices[4]	= 2;
	pIndices[5]	= 3;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, &m_pIB)))
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
		MSG_RETURN(nullptr, "CVIBuffer_Rect::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBuffer_Rect::Clone(any _arg)
{
	return shared_from_this();
}
