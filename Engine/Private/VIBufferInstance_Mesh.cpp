#include "EnginePCH.h"
#include "VIBufferInstance_Mesh.h"

CVIBufferInstance_Mesh::CVIBufferInstance_Mesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBufferInstance(_pDevice, _pContext, COMPONENT::VIBUFFER_INSTANCE_MESH)
{
}

CVIBufferInstance_Mesh::CVIBufferInstance_Mesh(const CVIBufferInstance_Mesh& _rhs)
	: CVIBufferInstance(_rhs)
{
}

HRESULT CVIBufferInstance_Mesh::Initialize_Prototype(const wstring& _wstrPath, _uint _iNumInstance)
{
	std::ifstream inFile(_wstrPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to Open File");
	}

	m_iNumInstances		= _iNumInstance;
	m_iInstanceStride	= sizeof(VTXPOSNORTANTEXINSTTRANSCOLORARG);

    m_iNumVB			= 2;
	m_iVertexStride		= sizeof(VTXMESH);
	m_iIndexStride		= sizeof(_uint);
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eTopology			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	inFile.read(reinterpret_cast<_byte*>(m_szName),						MAX_PATH);
	inFile.seekg(sizeof(_uint), std::ios::cur);
//	inFile.read(reinterpret_cast<_byte*>(&m_iMaterialIndex),			sizeof(_uint));
	inFile.seekg(sizeof(_uint), std::ios::cur);
//	inFile.read(reinterpret_cast<_byte*>(&m_iNumVB),					sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumVertices),				sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iIndicesCountPerInstance),	sizeof(_uint));

	m_iNumIndices		= m_iNumInstances * m_iIndicesCountPerInstance;

	m_pVertices			= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices			= Function::CreateDynamicArray<_uint>(m_iNumIndices);

	inFile.read(reinterpret_cast<_byte*>(m_pVertices.get()),			sizeof(_float3)	* m_iNumVertices);
	inFile.read(reinterpret_cast<_byte*>(m_pIndices.get()),				sizeof(_uint)	* m_iIndicesCountPerInstance);
	
#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	inFile.read(reinterpret_cast<_byte*>(pVertices.get()), m_iVertexStride * m_iNumVertices);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
	}

#pragma endregion
#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iIndexStride * m_iNumIndices;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	auto pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	for (_uint i = 0; i < m_iNumInstances; ++i)
	{
		memcpy(&pIndices[i * m_iIndicesCountPerInstance], m_pIndices.get(), sizeof(_uint) * m_iIndicesCountPerInstance);
	}

	m_tInitializeData.pSysMem = pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
	}

#pragma endregion

	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to Read File");
	}

	inFile.close();

    return S_OK;
}

HRESULT CVIBufferInstance_Mesh::Initialize(any)
{
	#pragma region INSTANCE_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumInstances * m_iInstanceStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	auto pInstanceVertices = Function::CreateDynamicArray<VTXPOSNORTANTEXINSTTRANSCOLORARG>(m_iNumInstances);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pInstanceVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pInstanceVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize", "Failed to CVIBuffer::Initialize");
	}

    return S_OK;
}

shared_ptr<CVIBufferInstance_Mesh> CVIBufferInstance_Mesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrPath, _uint _iNumInstance)
{
	shared_ptr<CVIBufferInstance_Mesh> pInstance = make_private_shared(CVIBufferInstance_Mesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_wstrPath, _iNumInstance)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBufferInstance_Mesh::Clone(any _arg)
{
	shared_ptr<CVIBufferInstance_Mesh> pInstance = make_private_shared_copy(CVIBufferInstance_Mesh, *this);

	if (FAILED(pInstance->Initialize(_arg)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Clone", "Failed to Initialize");
	}

	return pInstance;
}
