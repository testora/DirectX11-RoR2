#include "EnginePCH.h"
#include "Mesh.h"
#include "Model.h"
#include "Bone.h"
#include "Event_Handler.h"

CMesh::CMesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::MESH)
{
	m_arrInterpolationMatrices.fill(g_mUnit);
}

HRESULT CMesh::Initialize(MODEL _eType, const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel, _matrixf _mPivot)
{
	strcpy_s(m_szName, _pAIMesh->mName.data);

	m_mPivot			= _mPivot;
	m_iMaterialIndex	= _pAIMesh->mMaterialIndex;

	m_iNumVB			= 1;
	m_iNumVertices		= _pAIMesh->mNumVertices;
	m_iNumIndices		= _pAIMesh->mNumFaces * 3;
	m_iIndexStride		= sizeof(_uint);
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eTopology			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pVertices			= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices			= Function::CreateDynamicArray<_uint>(m_iNumIndices);

#pragma region VERTEX_BUFFER

	switch (_eType)
	{
	case MODEL::NONANIM:
		m_iVertexStride = sizeof(VTXMESH);
		if (FAILED(Ready_VertexBuffer_NonAnim(_pAIMesh)))
		{
			MSG_RETURN(E_FAIL, "CMesh::Initialize", "Failed to Ready_VertexBuffer_NonAnim");
		}
		break;

	case MODEL::ANIM:
		m_iVertexStride = sizeof(VTXMESHANIM);
		if (FAILED(Ready_VertexBuffer_Anim(_pAIMesh, _pModel)))
		{
			MSG_RETURN(E_FAIL, "CMesh::Initialize", "Failed to Ready_VertexBuffer_Anim");
		}
		break;

	default:
		MSG_RETURN(E_FAIL, "CMesh::Initialize", "Invalid MODEL");
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
		MSG_RETURN(E_FAIL, "CMesh::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

_float4x4* CMesh::Get_BoneMatrices(vector<shared_ptr<class CBone>> _vecModelBones)
{
	m_arrBones.fill(g_mUnit);
	cout << m_fInterpolationRatio << endl;

	for (size_t i = 0; i < m_iNumBones; ++i)
	{
		_matrix mInterpolation = Function::Lerp(m_arrInterpolationMatrices[i], _vecModelBones[m_vecBoneIndices[i]]->Get_CombinedTransformation(), m_fInterpolationRatio);
		m_arrBones[i] = m_vecOffsets[i] * mInterpolation * m_mPivot;
	}

	return m_arrBones.data();
}

void CMesh::Set_Interpolation(vector<shared_ptr<class CBone>> _vecModelBones, _float _fDuration)
{
	if (0.f == _fDuration)
	{
		return;
	}

	for (size_t i = 0; i < m_iNumBones; ++i)
	{
		m_arrInterpolationMatrices[i] = Function::Lerp(m_arrInterpolationMatrices[i], _vecModelBones[m_vecBoneIndices[i]]->Get_CombinedTransformation(), m_fInterpolationRatio);
	}

	m_fInterpolationRatio = 0.f;
	
	CEvent_Handler::Get_Instance()->Erase_TickListener(shared_from_this());
	CEvent_Handler::Get_Instance()->Register_TickListener(shared_from_this(),
		[this, _fDuration](_float _fDeltaTime)->_bool
		{
			m_fInterpolationRatio += _fDeltaTime / _fDuration;

			if (m_fInterpolationRatio >= 1.f)
			{
				m_fInterpolationRatio = 1.f;
				return false;
			}

			return true;
		});
}

HRESULT CMesh::Ready_VertexBuffer_NonAnim(const aiMesh* _pAIMesh)
{
	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth				= m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
		pVertices[i].vPosition = m_pVertices[i] = _float3(XMVector3TransformCoord(_float3(pVertices[i].vPosition), m_mPivot));

		memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
		pVertices[i].vNormal = _float3(XMVector3TransformNormal(_float3(pVertices[i].vNormal), m_mPivot)).normalize();

		if (_pAIMesh->mTangents)
		{
			memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));
		}

		if (_pAIMesh->mTextureCoords[0])
		{
			memcpy(&pVertices[i].vTexCoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		}
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, &m_pVB)))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_Anim(const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel)
{
	m_iNumBones = _pAIMesh->mNumBones;

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth				= m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESHANIM>(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition,	&_pAIMesh->mVertices[i],			sizeof(_float3));
		memcpy(&pVertices[i].vNormal,	&_pAIMesh->mNormals[i],				sizeof(_float3));
		memcpy(&pVertices[i].vTangent,	&_pAIMesh->mTangents[i],			sizeof(_float3));
		memcpy(&pVertices[i].vTexCoord,	&_pAIMesh->mTextureCoords[0][i],	sizeof(_float2));
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = _pAIMesh->mBones[i];
		m_vecBoneIndices.emplace_back(_pModel->Get_BoneIndex(pAIBone->mName.data));

		_float4x4 mOffset;
		memcpy(&mOffset, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		m_vecOffsets.emplace_back(XMMatrixTranspose(mOffset));

		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_vecBoneIndices.emplace_back(_pModel->Get_BoneIndex(m_szName));

		m_vecOffsets.emplace_back(g_mUnit);
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, &m_pVB)))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

	return S_OK;
}

shared_ptr<CMesh> CMesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, MODEL _eType, const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel, _matrixf _mPivot)
{
	shared_ptr<CMesh> pInstance = make_private_shared(CMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_eType, _pAIMesh, _pModel, _mPivot)))
	{
		MSG_RETURN(nullptr, "CMesh::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CMesh::Clone(any)
{
	return shared_from_this();
}
