#include "EnginePCH.h"
#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::VIBUFFER_CUBE)
{
}

HRESULT CVIBuffer_Cube::Initialize(any)
{
    m_iNumVB		= 1;
	m_iNumVertices	= 8;
	m_iVertexStride	= sizeof(VTXCUBEPOSTEX);
	m_iNumIndices	= 36;
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

	auto pVertices = Function::CreateDynamicArray<VTXCUBEPOSTEX>(m_iNumVertices);

	pVertices[0].vPosition = pVertices[0].vTexCoord = m_pVertices[0] = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[1].vPosition = pVertices[1].vTexCoord = m_pVertices[1] = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vPosition = pVertices[2].vTexCoord = m_pVertices[2] = _float3(0.5f, -0.5f, -0.5f);
	pVertices[3].vPosition = pVertices[3].vTexCoord = m_pVertices[3] = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[4].vPosition = pVertices[4].vTexCoord = m_pVertices[4] = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[5].vPosition = pVertices[5].vTexCoord = m_pVertices[5] = _float3(0.5f, 0.5f, 0.5f);
	pVertices[6].vPosition = pVertices[6].vTexCoord = m_pVertices[6] = _float3(0.5f, -0.5f, 0.5f);
	pVertices[7].vPosition = pVertices[7].vTexCoord = m_pVertices[7] = _float3(-0.5f, -0.5f, 0.5f);

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

	pIndices[0]		= 1;	pIndices[1]		= 5;	pIndices[2]		= 6;
	pIndices[3]		= 1;	pIndices[4]		= 6;	pIndices[5]		= 2;
																	
	pIndices[6]		= 4;	pIndices[7]		= 0;	pIndices[8]		= 3;
	pIndices[9]		= 4;	pIndices[10]	= 3;	pIndices[11]	= 7;

	pIndices[12]	= 4;	pIndices[13]	= 5;	pIndices[14]	= 1;
	pIndices[15]	= 4;	pIndices[16]	= 1;	pIndices[17]	= 0;

	pIndices[18]	= 3;	pIndices[19]	= 2;	pIndices[20]	= 6;
	pIndices[21]	= 3;	pIndices[22]	= 6;	pIndices[23]	= 7;

	pIndices[24]	= 5;	pIndices[25]	= 4;	pIndices[26]	= 7;
	pIndices[27]	= 5;	pIndices[28]	= 7;	pIndices[29]	= 6;

	pIndices[30]	= 0;	pIndices[31]	= 1;	pIndices[32]	= 2;
	pIndices[33]	= 0;	pIndices[34]	= 2;	pIndices[35]	= 3;

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

shared_ptr<CVIBuffer_Cube> CVIBuffer_Cube::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CVIBuffer_Cube> pInstance = make_private_shared(CVIBuffer_Cube, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CVIBuffer_Cube::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBuffer_Cube::Clone(any)
{
	return shared_from_componenet();
}
