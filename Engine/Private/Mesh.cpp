#include "EnginePCH.h"
#include "Mesh.h"

CMesh::CMesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::MESH)
{
}

HRESULT CMesh::Initialize(const aiMesh* _pAIMesh, _matrixf _mPivot)
{
	m_iMaterialIndex = _pAIMesh->mMaterialIndex;

	m_iNumVB		= 1;
	m_iNumVertices	= _pAIMesh->mNumVertices;
	m_iVertexStride	= sizeof(VTXMESH);
	m_iNumIndices	= _pAIMesh->mNumFaces * 3;
	m_iIndexStride	= sizeof(_uint);
	m_eIndexFormat	= DXGI_FORMAT_R32_UINT;
	m_eTopology		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pVertices		= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices		= Function::CreateDynamicArray<_byte>(m_iNumIndices * m_iIndexStride);

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{

		memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
		pVertices[i].vPosition	= m_pVertices[i] = _float3(XMVector3TransformCoord(_float3(pVertices[i].vPosition), _mPivot));

		memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
		pVertices[i].vNormal	= _float3(XMVector3TransformNormal(_float3(pVertices[i].vNormal), _mPivot)).normalize();

		memcpy(&pVertices[i].vTexCoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2));

		memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, &m_pVB)))
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

	size_t index = 0;
	for (size_t i = 0; i < _pAIMesh->mNumFaces; ++i)
	{
		pIndices[index++] = _pAIMesh->mFaces[i].mIndices[0];
		pIndices[index++] = _pAIMesh->mFaces[i].mIndices[1];
		pIndices[index++] = _pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = pIndices.get();

	memcpy(m_pIndices.get(), pIndices.get(), m_iNumIndices * m_iIndexStride);

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

HRESULT CMesh::Initialize(any _pAIMesh_mPivot)
{
	return S_OK;
}

shared_ptr<CMesh> CMesh::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext, const aiMesh* _pAIMesh, _matrixf _mPivot)
{
	shared_ptr<CMesh> pInstance = make_private_shared(CMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pAIMesh, _mPivot)))
	{
		MSG_RETURN(nullptr, "CMesh::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CMesh::Clone(any)
{
	return shared_from_this();
}
