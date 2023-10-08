#include "EnginePCH.h"
#include "VIBufferInstance_Point.h"

CVIBufferInstance_Point::CVIBufferInstance_Point(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBufferInstance(_pDevice, _pContext, COMPONENT::VIBUFFER_INSTANCE_POINT)
{
}

CVIBufferInstance_Point::CVIBufferInstance_Point(const CVIBufferInstance_Point& _rhs)
	: CVIBufferInstance(_rhs)
{
}

HRESULT CVIBufferInstance_Point::Initialize_Prototype(_uint _iNumInstance)
{
	m_iNumInstance				= _iNumInstance;
	m_iInstanceStride			= sizeof(VTXINSTPOINT);
	m_iIndicesCountPerInstance	= 1;

    m_iNumVB					= 2;
	m_iNumVertices				= 1;
	m_iNumIndices				= m_iNumInstance * m_iIndicesCountPerInstance;
	m_iVertexStride				= sizeof(VTXPOSSIZE);
	m_iIndexStride				= sizeof(_uint);
	m_eIndexFormat				= DXGI_FORMAT_R32_UINT;
	m_eTopology					= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	m_pVertices					= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices					= Function::CreateDynamicArray<_uint>(m_iNumIndices);

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXPOSSIZE>(m_iNumVertices);

	pVertices[0].vPosition	= _float3(0.f, 0.f, 0.f);
	pVertices[0].vSize		= _float2(1.f, 1.f);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem	= pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Rect::Initialize_Prototype", "Failed to CreateBuffer");
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

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Rect::Initialize_Prototype", "Failed to CreateBuffer");
	}

#pragma endregion

    return S_OK;
}

HRESULT CVIBufferInstance_Point::Initialize(any)
{
#pragma region INSTANCE_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumInstance * m_iInstanceStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	auto pInstanceVertices = Function::CreateDynamicArray<VTXINSTPOINT>(m_iNumInstance);
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		memcpy(&pInstanceVertices[i], &g_mUnit, sizeof(g_mUnit));
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pInstanceVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pInstanceVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Rect::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Rect::Initialize", "Failed to CVIBuffer::Initialize");
	}

    return S_OK;
}

shared_ptr<CVIBufferInstance_Point> CVIBufferInstance_Point::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext>_pContext, _uint _iNumInstance)
{
	shared_ptr<CVIBufferInstance_Point> pInstance = make_private_shared(CVIBufferInstance_Point, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_iNumInstance)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Point::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBufferInstance_Point::Clone(any _arg)
{
	shared_ptr<CVIBufferInstance_Point> pInstance = make_private_shared_copy(CVIBufferInstance_Point, *this);

	if (FAILED(pInstance->Initialize(_arg)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Point::Clone", "Failed to Initialize");
	}

	return pInstance;
}
