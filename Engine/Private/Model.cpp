#include "EnginePCH.h"
#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Animation.h"
#include "Texture.h"
#include "Shader.h"

CModel::CModel(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::MODEL)
{
}

CModel::CModel(const CModel& _rhs)
	: CComponent					(_rhs)
	, m_eType						(_rhs.m_eType)
	, m_mPivot						(_rhs.m_mPivot)
	, m_bManualShader				(_rhs.m_bManualShader)
	, m_iNumBones					(_rhs.m_iNumBones)
	, m_iNumMeshes					(_rhs.m_iNumMeshes)
	, m_iNumMaterials				(_rhs.m_iNumMaterials)
	, m_iNumAnimations				(_rhs.m_iNumAnimations)
	, m_vecMeshes					(_rhs.m_vecMeshes)
	, m_vecMaterials				(_rhs.m_vecMaterials)
	, m_vecMaterialDescs			(_rhs.m_vecMaterialDescs)
	, m_vecShaderDescs				(_rhs.m_vecShaderDescs)
	, m_vecSubShaderPass			(_rhs.m_vecSubShaderPass)
	, m_iSubShaderPass				(_rhs.m_iSubShaderPass)
	, m_vecBoneAnimationIndices		(_rhs.m_vecBoneAnimationIndices)
	, m_vecBonePivot				(_rhs.m_vecBonePivot)
	, m_mapHideMeshFromAnimations	(_rhs.m_mapHideMeshFromAnimations)
{
	for (auto pOriginal : _rhs.m_vecAnimations)
	{
		m_vecAnimations.push_back(pOriginal->Clone());
	}

	for (auto pOriginal : _rhs.m_vecBones)
	{
		m_vecBones.push_back(pOriginal->Clone());
	}
}

HRESULT CModel::Initialize_Prototype(const MODEL _eType, const wstring& _wstrModelPath, _matrixf _mPivot)
{
	wstring strExt(_wstrModelPath.substr(_wstrModelPath.rfind(L'.')));

	if (strExt == TEXT(".mdl"))
	{
		if (FAILED(Initialize_FromBinary(_wstrModelPath)))
		{
			MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Initialize_FromBinary");
		}
	}
#if ACTIVATE_TOOL
	else if (strExt == TEXT(".fbx"))
	{
		if (FAILED(Initialize_FromAssimp(_eType, _wstrModelPath, _mPivot)))
		{
			MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Intialize_FromAssimp");
		}
	}
#endif
	else
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize", "Invalid Extension");
	}

	return S_OK;
}

HRESULT CModel::Initialize(any _mapMeshDesc)
{
	if (_mapMeshDesc.has_value())
	{
		auto desc = any_cast<map<_uint, tuple<MATERIALDESC, _flags, function<HRESULT(shared_ptr<class CShader>)>>>>(_mapMeshDesc);
		for (auto& pair : desc)
		{
			m_vecMaterialDescs[pair.first] = std::get<0>(pair.second);

			_flags iShaderFlags = std::get<1>(pair.second);
			if (iShaderFlags)
			{
				m_mapMeshShaderFlags[pair.first] = iShaderFlags;
			}

			function<HRESULT(shared_ptr<class CShader>)> funcCallback = std::get<2>(pair.second);
			if (funcCallback)
			{
				m_mapMeshShaderBindings[pair.first] = funcCallback;
			}
		}
	}

	return S_OK;
}

HRESULT CModel::Render(shared_ptr<CShader> _pShader, _uint _iPassIndex)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
#ifdef _DEBUG
		if (std::find(m_vecMeshHide.begin(), m_vecMeshHide.end(), i) != m_vecMeshHide.end())
		{
			continue;
		}
#endif
		if (m_bManualShader)
		{
			if (m_mapMeshShaderFlags.find(i) != m_mapMeshShaderFlags.end())
			{
				_pShader->Set_Flag(m_mapMeshShaderFlags[i]);
			}
			if (m_mapMeshShaderBindings.find(i) != m_mapMeshShaderBindings.end())
			{
				if (nullptr != m_mapMeshShaderBindings[i])
				{
					if (FAILED(m_mapMeshShaderBindings[i](_pShader)))
					{
						MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind");
					}
				}
			}
		}
		else
		{
			if (FAILED(Bind_ShaderOptions(i, _pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_ShaderOptions");
			}
		}

		if (MODEL::ANIM == m_eType)
		{
			if (FAILED(Bind_BoneMatrices(i, _pShader, SHADER_BONE)))
			{
				MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_BoneMatrices");
			}
		}

		if (FAILED(Bind_ShaderResourceViews(i, _pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}

		if (FAILED(Render(i, _pShader, _iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Render");
		}
	}

	return S_OK;
}

HRESULT CModel::Render(_uint _iMeshIndex, shared_ptr<CShader> _pShader, _uint _iPassIndex)
{
	if (m_vecMeshes[_iMeshIndex])
	{
		if (MODEL::ANIM == m_eType)
		{
			for (_uint iAnimationIndex : m_usetAnimationPlayingIndices)
			{
				auto range = m_mapHideMeshFromAnimations.equal_range(iAnimationIndex);
				for (auto iter = range.first; iter != range.second; ++iter)
				{
					if (iter->second == _iMeshIndex)
					{
						return S_FALSE;
					}
				}
			}
		}

		if (m_bManualShader)
		{
			if (FAILED(_pShader->Bind_Vector(SHADER_MTRLDIF, m_vecMaterialDescs[_iMeshIndex].vDiffuse)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
			}
			if (FAILED(_pShader->Bind_Vector(SHADER_MTRLAMB, m_vecMaterialDescs[_iMeshIndex].vAmbient)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
			}
			if (FAILED(_pShader->Bind_Vector(SHADER_MTRLSPC, m_vecMaterialDescs[_iMeshIndex].vSpecular)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
			}
			if (FAILED(_pShader->Bind_Vector(SHADER_MTRLEMS, m_vecMaterialDescs[_iMeshIndex].vEmissive)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
			}
			if (FAILED(_pShader->Bind_Float(SHADER_MTRLSHN, m_vecMaterialDescs[_iMeshIndex].fShininess)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
			}
		}

		if (FAILED(m_vecMeshes[_iMeshIndex]->Render(_pShader, m_bManualShader ? _iPassIndex : m_vecSubShaderPass[_iMeshIndex] ? m_iSubShaderPass : _iPassIndex, false)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to CMesh::Render");
		}
	}

	return S_OK;
}

#if ACTIVATE_TOOL
HRESULT CModel::Initialize_FromAssimp(const MODEL _eType, const wstring& _wstrModelPath, _matrixf _mPivot)
{
	m_eType		= _eType;
	m_mPivot	= _mPivot;

	Assimp::Importer	aiImporter;
	const aiScene*		pAIScene	= nullptr;
	
	_flags iAIFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;
	if (MODEL::NONANIM == m_eType)
	{
		iAIFlag |= aiProcess_PreTransformVertices;
	}

	pAIScene = aiImporter.ReadFile(Function::ToString(_wstrModelPath).c_str(), iAIFlag);

	if (nullptr == pAIScene)
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to ReadFile");
	}

	if (FAILED(Ready_Bones(pAIScene->mRootNode, g_iMaxBones)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Bones");
	}
	m_iNumBones = static_cast<_uint>(m_vecBones.size());
	m_vecBoneAnimationIndices.resize(m_iNumBones);
	m_vecBonePivot.resize(m_iNumBones);
	std::fill(m_vecBonePivot.begin(), m_vecBonePivot.end(), g_mUnit);

	if (FAILED(Ready_Animations(pAIScene)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Animations");
	}

	if (FAILED(Ready_Meshes(pAIScene, _mPivot)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Meshes");
	}
	m_vecShaderDescs.resize(m_iNumMeshes);
	m_vecSubShaderPass.resize(m_iNumMeshes);
	m_vecMaterialDescs.resize(m_iNumMeshes);

	if (FAILED(Ready_Materials(pAIScene, _wstrModelPath)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Materials");
	}

	return S_OK;
}
#endif

HRESULT CModel::Initialize_FromBinary(const wstring& _wstrModelPath)
{
	std::ifstream inFile(_wstrModelPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Open File");
	}

	inFile.read(reinterpret_cast<_byte*>(&m_eType),				sizeof(MODEL));
	inFile.read(reinterpret_cast<_byte*>(&m_bManualShader),		sizeof(_bool));
	inFile.read(reinterpret_cast<_byte*>(&m_mPivot),			sizeof(_float4x4));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumBones),			sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumAnimations),	sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumMeshes),		sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumMaterials),		sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iSubShaderPass),	sizeof(_uint));
	
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Model");
	}

#pragma region Bones
	m_vecBones.reserve(m_iNumBones);
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		m_vecBones.emplace_back(CBone::Read(inFile));
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Bones");
	}
#pragma endregion
#pragma region Animations
	m_vecAnimations.reserve(m_iNumAnimations);
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		m_vecAnimations.emplace_back(CAnimation::Read(inFile));
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Animations");
	}
#pragma endregion
#pragma region Meshes
	m_vecMeshes.reserve(m_iNumMeshes);
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		m_vecMeshes.emplace_back(CMesh::Read(m_pDevice, m_pContext, m_eType, inFile));
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Meshes");
	}
#pragma endregion
#pragma region Materials
	m_vecMaterials.resize(m_iNumMaterials);
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			size_t	nNumTextures(0);
			inFile.read(reinterpret_cast<_byte*>(&nNumTextures),		sizeof(size_t));

			m_vecMaterials[i].pTexture[j] = CTexture::Create(m_pDevice, m_pContext, static_cast<_uint>(nNumTextures));

			for (size_t k = 0; k < nNumTextures; ++k)
			{
				size_t	nPathLength(0);
				inFile.read(reinterpret_cast<_byte*>(&nPathLength),		sizeof(size_t));

				wstring	wstrPath;
				wstrPath.resize(nPathLength);
				inFile.read(reinterpret_cast<_byte*>(wstrPath.data()),	sizeof(_wchar) * nPathLength);

				m_vecMaterials[i].pTexture[j]->Push_ShaderResourceView(wstrPath);
			}
		}
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Materials");
	}
#pragma endregion
#pragma region Shaders
	m_vecShaderDescs.resize(m_iNumMeshes);
	m_vecSubShaderPass.resize(m_iNumMeshes);
	m_vecMaterialDescs.resize(m_iNumMeshes);
	if (!m_bManualShader)
	{
		inFile.read(reinterpret_cast<_byte*>(m_vecShaderDescs.data()),	sizeof(SHADERDESC) * m_iNumMeshes);
		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			_bool bSubShaderPass(false);
			inFile.read(reinterpret_cast<_byte*>(&bSubShaderPass),		sizeof(_bool));
			m_vecSubShaderPass[i] = bSubShaderPass;
		}
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File");
	}
#pragma endregion

	inFile.close();

	m_vecBoneAnimationIndices.resize(m_iNumBones);
	m_vecBonePivot.resize(m_iNumBones);
	m_vecMaterialDescs.resize(m_iNumMeshes);
	std::fill(m_vecBonePivot.begin(), m_vecBonePivot.end(), g_mUnit);

	return S_OK;
}

#if ACTIVATE_TOOL
HRESULT CModel::Ready_Bones(const aiNode* _pAINode, _uint _iParentBoneIndex)
{
	shared_ptr<CBone> pBone = CBone::Create(_pAINode, _iParentBoneIndex);
	if (nullptr == pBone)
	{
		MSG_RETURN(E_FAIL, "CModel::Ready_Bones", "Failed to CBone::Create");
	}

	m_vecBones.emplace_back(pBone);

	for (size_t i = 0; i < _pAINode->mNumChildren; ++i)
	{
		if (FAILED(Ready_Bones(_pAINode->mChildren[i], Get_BoneIndex(pBone->Get_Name()))))
		{
			MSG_RETURN(E_FAIL, "CModel::Ready_Bones", "Failed to Ready_Bones");
		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations(const aiScene* _pAIScene)
{
	HRESULT hr = S_OK;

	m_iNumAnimations = _pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		shared_ptr<CAnimation> pAnimation = CAnimation::Create(_pAIScene->mAnimations[i], static_pointer_cast<CModel>(shared_from_this()));
		if (nullptr == pAnimation)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CModel::Ready_Animations", "Failed to CAnimation::Create");
		}

		m_vecAnimations.emplace_back(pAnimation);
	}

	return hr;
}

HRESULT CModel::Ready_Meshes(const aiScene* _pAIScene, _matrixf _mPivot)
{
	HRESULT hr = S_OK;

	m_iNumMeshes = _pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		shared_ptr<CMesh> pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, _pAIScene->mMeshes[i], static_pointer_cast<CModel>(shared_from_this()));
		if (nullptr == pMesh)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CModel::Ready_Meshes", "Failed to CMesh::Create");
		}

		m_vecMeshes.emplace_back(pMesh);
	}

	return hr;
}

HRESULT CModel::Ready_Materials(const aiScene* _pAIScene, const wstring& _wstrModelPath)
{
	HRESULT hr = S_OK;

	m_iNumMaterials = _pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MATERIAL	tModelMaterial{};
		aiMaterial*	pAIMaterial = _pAIScene->mMaterials[i];

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			shared_ptr<CTexture> pTexture = CTexture::Create(m_pDevice, m_pContext, _wstrModelPath, pAIMaterial, aiTextureType(j));
			if (nullptr == pTexture)
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Ready_Materials", "Failed to CTexture::Create");
			}

			tModelMaterial.pTexture[j] = pTexture;
		}

		m_vecMaterials.emplace_back(tModelMaterial);
	}

	return hr;
}
#endif

_bool CModel::Is_AnimationPlaying(_uint _iAnimationIndex) const
{
	return m_usetAnimationPlayingIndices.find(_iAnimationIndex) != m_usetAnimationPlayingIndices.end();
}

_bool CModel::Is_AnimationFinished(_uint _iAnimationIndex) const
{
	return m_vecAnimations[_iAnimationIndex]->Is_Finished();
}

_uint CModel::Get_BoneIndex(const _char* _szBoneName) const
{
	for (_uint i = 0; i < m_vecBones.size(); ++i)
	{
		if (!strcmp(m_vecBones[i]->Get_Name(), _szBoneName))
		{
			return i;
		}
	}

	return static_cast<_uint>(m_vecBones.size());
}

_uint CModel::Get_MeshIndex(const _char* _szMeshName) const
{  
	for (_uint i = 0; i < m_vecMeshes.size(); ++i)
	{
		if (!strcmp(m_vecMeshes[i]->Get_Name(), _szMeshName))
		{
			return i;
		}
	}

	return static_cast<_uint>(m_vecMeshes.size());
}

#if ACTIVATE_TOOL
const _uint CModel::Get_MeshIndex(shared_ptr<CMesh> _pMesh) const
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (m_vecMeshes[i] == _pMesh)
		{
			return i;
		}
	}

	return m_iNumMeshes;
}

const _uint CModel::Get_AnimationIndex(shared_ptr<CAnimation> _pAnimation) const
{
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		if (m_vecAnimations[i] == _pAnimation)
		{
			return i;
		}
	}

	return m_iNumAnimations;
}
#endif

void CModel::Set_DefaultAnimation(_uint _iAnimationIndex)
{
	std::fill(m_vecBoneAnimationIndices.begin(), m_vecBoneAnimationIndices.end(), _iAnimationIndex);
}

void CModel::Set_Animation(_uint _iAnimationIndex, _float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop)
{
	if (_iAnimationIndex >= m_iNumAnimations)
	{
		MSG_RETURN(, "CModel::Set_Animation", "Invalid Index");
	}

	for (auto& pMesh : m_vecMeshes)
	{
		pMesh->Set_InterpolationMatrix(m_vecBones.begin(), _fInterpolationDuration);
	}

	m_usetAnimationPlayingIndices.clear();
	m_vecAnimations[_iAnimationIndex]->Set_AnimationIndex(m_vecBoneAnimationIndices.begin(), _iAnimationIndex);
	for (auto iAnimationIndex : m_vecBoneAnimationIndices)
	{
		m_usetAnimationPlayingIndices.emplace(iAnimationIndex);
	}

	m_fAnimationPlaySpeed		= _fPlaySpeed;
	m_bAnimReverse				= _bReverse;
	m_bAnimLoop					= _bLoop;
}

void CModel::Fix_Animation(_uint _iAnimationIndex, _float _fRatio)
{
	if (_iAnimationIndex >= m_iNumAnimations)
	{
		MSG_RETURN(, "CModel::Fix_Animation", "Invalid Index");
	}

	m_usetAnimationPlayingIndices.clear();
	m_vecAnimations[_iAnimationIndex]->Set_AnimationIndex(m_vecBoneAnimationIndices.begin(), _iAnimationIndex);
	for (auto iAnimationIndex : m_vecBoneAnimationIndices)
	{
		m_usetAnimationPlayingIndices.emplace(iAnimationIndex);
	}

	m_umapAnimationFixRatio.emplace(_iAnimationIndex, _fRatio);
}

void CModel::Blend_Animation(_uint _iAnimationIndex, _float _fRatio)
{
	if (_iAnimationIndex >= m_iNumAnimations)
	{
		MSG_RETURN(, "CModel::Fix_Animation", "Invalid Index");
	}

	m_vecAnimations[_iAnimationIndex]->Blend(m_vecAnimations.begin(), m_vecBones.begin(), m_vecBoneAnimationIndices.begin(), _fRatio);
}

void CModel::Reset_Animation(_uint _iAnimationIndex)
{
	m_vecAnimations[_iAnimationIndex]->Reset();
	Tick_Animation(0.f);
}

void CModel::Tick_Animation(_float _fTimeDelta)
{
	if (MODEL::NONANIM == m_eType)
	{
		return;
	}

	for (_uint iAnimationIndex : m_usetAnimationPlayingIndices)
	{
		if (m_umapAnimationFixRatio.find(iAnimationIndex) == m_umapAnimationFixRatio.end())
		{
			m_vecAnimations[iAnimationIndex]->Tick(m_fAnimationPlaySpeed * _fTimeDelta, m_vecBones.begin(), m_bAnimReverse, m_bAnimLoop);
		}
		else
		{
			m_vecAnimations[iAnimationIndex]->Fix(m_vecBones.begin(), m_umapAnimationFixRatio[iAnimationIndex]);
		}
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		m_vecBones[i]->Update_CombinedTransformation(m_vecBones.begin(), m_vecBonePivot[i]);
	}

	m_umapAnimationFixRatio.clear();
}

void CModel::Iterate_Meshes(function<_bool(shared_ptr<CMesh>)> _funcCallback)
{
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		if (!_funcCallback(m_vecMeshes[i]))
		{
			return;
		}
	}
}

HRESULT CModel::Show_MeshFromAnimations(_uint _iAnimationIndex, _uint _iMeshIndex)
{
	auto range = m_mapHideMeshFromAnimations.equal_range(_iAnimationIndex);
	for (auto iter = range.first; iter != range.second; ++iter)
	{
		if (iter->second == _iMeshIndex)
		{
			m_mapHideMeshFromAnimations.erase(iter);
			return S_OK;
		}
	}

	return S_FALSE;
}

HRESULT CModel::Hide_MeshFromAnimations(_uint _iAnimationIndex, _uint _iMeshIndex)
{
	if (m_mapHideMeshFromAnimations.end() != m_mapHideMeshFromAnimations.find(_iAnimationIndex))
	{
		auto range = m_mapHideMeshFromAnimations.equal_range(_iAnimationIndex);
		for (auto iter = range.first; iter != range.second; ++iter)
		{
			if (iter->second == _iMeshIndex)
			{
				return S_FALSE;
			}
		}
	}

	m_mapHideMeshFromAnimations.emplace(_iAnimationIndex, _iMeshIndex);
	
	return S_OK;
}

#ifdef _DEBUG
_bool CModel::Is_MeshHidden(_uint iMeshIndex) const
{
	return std::find(m_vecMeshHide.begin(), m_vecMeshHide.end(), iMeshIndex) != m_vecMeshHide.end();
}

void CModel::Hide_Mesh(_uint _iMeshIndex, _bool _bHide)
{
	if (Function::InRange(_iMeshIndex, 0u, m_iNumMeshes))
	{
		if (_bHide)
		{
			m_vecMeshHide.emplace_back(_iMeshIndex);
		}
		else
		{
			m_vecMeshHide.erase(std::find(m_vecMeshHide.begin(), m_vecMeshHide.end(), _iMeshIndex));
		}
	}
}
#endif

HRESULT CModel::Bind_ShaderResourceView(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, _uint _iTextureIndex)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}
	
	HRESULT hr = S_OK;

	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		if (nullptr == m_vecMaterials[iMaterialIndex].pTexture[i])
		{
			continue;
		}

		switch (i)
		{
		case aiTextureType_DIFFUSE:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceView(_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE, _iTextureIndex)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceView", "Failed to CTexture::Bind_ShaderResourceView")
			}
			break;

		case aiTextureType_NORMALS:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceView(_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL, _iTextureIndex)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceView", "Failed to CTexture::Bind_ShaderResourceView")
			}
			break;
		}
	}

	return hr;
}

HRESULT CModel::Bind_ShaderResourceView(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, aiTextureType _eAIType, const _char* _szConstantName, _uint _iTextureIndex)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	shared_ptr<CTexture> pTexture = m_vecMaterials[iMaterialIndex].pTexture[_eAIType];
	if (nullptr == pTexture)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return pTexture->Bind_ShaderResourceView(_pShader, _eAIType, _szConstantName, _iTextureIndex);
}

HRESULT CModel::Bind_ShaderResourceViews(_uint _iMeshIndex, shared_ptr<CShader> _pShader)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceViews", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceViews", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceViews", "Invalid Range");
	}

	HRESULT hr = S_OK;

	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		if (nullptr == m_vecMaterials[iMaterialIndex].pTexture[i])
		{
			continue;
		}
		auto a = m_vecMaterials[iMaterialIndex];
		switch (i)
		{
		case aiTextureType_DIFFUSE:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceViews(_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceViews", "Failed to CTexture::Bind_ShaderResourceViews")
			}
			break;

		case aiTextureType_NORMALS:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceViews(_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceViews", "Failed to CTexture::Bind_ShaderResourceViews")
			}
			break;
		}
	}

	return hr;
}

HRESULT CModel::Bind_ShaderResourceViews(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, aiTextureType _eAIType, const _char* _szConstantName)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	shared_ptr<CTexture> pTexture = m_vecMaterials[iMaterialIndex].pTexture[_eAIType];
	if (nullptr == pTexture)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return pTexture->Bind_ShaderResourceViews(_pShader, _eAIType, _szConstantName);
}

HRESULT CModel::Bind_BoneMatrices(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, const _char* _szConstantName)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return _pShader->Bind_MatrixArray(_szConstantName, m_vecMeshes[_iMeshIndex]->Get_BoneMatrices(m_vecBones.begin(), m_mPivot), m_vecMeshes[_iMeshIndex]->Get_NumBones());
}

HRESULT CModel::Bind_ShaderOptions(_uint _iMeshIndex, shared_ptr<CShader> _pShader)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	_pShader->Set_Flag(m_vecShaderDescs[_iMeshIndex].iShaderFlag);

	_float fDifTiling[8], fNorTiling[8];
	std::fill(fDifTiling, fDifTiling + 8, m_vecShaderDescs[_iMeshIndex].fDiffuseTiling);
	std::fill(fNorTiling, fNorTiling + 8, m_vecShaderDescs[_iMeshIndex].fNormalTiling);

	if (FAILED(_pShader->Bind_FloatArray(SHADER_TILING_DIFFUSE, fDifTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderOptions", "Failed to Bind_Float");
	}
	if (FAILED(_pShader->Bind_FloatArray(SHADER_TILING_NORMAL, fNorTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderOptions", "Failed to Bind_Float");
	}

	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLDIF, m_vecShaderDescs[_iMeshIndex].tMaterialDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderOptions", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLAMB, m_vecShaderDescs[_iMeshIndex].tMaterialDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderOptions", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLSPC, m_vecShaderDescs[_iMeshIndex].tMaterialDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderOptions", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLEMS, m_vecShaderDescs[_iMeshIndex].tMaterialDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderOptions", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_MTRLSHN, m_vecShaderDescs[_iMeshIndex].tMaterialDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderOptions", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	return S_OK;
}

shared_ptr<CModel> CModel::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const MODEL _eType, const wstring& _wstrModelPath, _matrixf _mPivot)
{
	shared_ptr<CModel> pInstance = make_private_shared(CModel, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_eType, _wstrModelPath, _mPivot)))
	{
		MSG_RETURN(nullptr, "CModel::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CModel::Clone(any _mapDesc)
{
	shared_ptr<CModel> pInstance = make_private_shared_copy(CModel, *this);

	if (FAILED(pInstance->Initialize(_mapDesc)))
	{
		MSG_RETURN(nullptr, "CModel::Clone", "Failed to Initialize");
	}
	
	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CModel::Export(const wstring& _wstrPath)
{
	std::ofstream outFile(_wstrPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Open File");
	}

#pragma region Model
	outFile.write(reinterpret_cast<const _byte*>(&m_eType),				sizeof(MODEL));
	outFile.write(reinterpret_cast<const _byte*>(&m_bManualShader),		sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_mPivot),			sizeof(_float4x4));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumBones),			sizeof(_uint));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumAnimations),	sizeof(_uint));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumMeshes),		sizeof(_uint));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumMaterials),		sizeof(_uint));
	outFile.write(reinterpret_cast<const _byte*>(&m_iSubShaderPass),	sizeof(_uint));

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Model");
	}
#pragma endregion
#pragma region Bones
	for (const auto& pBone : m_vecBones)
	{
		pBone->Export(outFile);
	}
	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Bones");
	}
#pragma endregion
#pragma region Animations
	for(const auto& pAnimation : m_vecAnimations)
	{
		pAnimation->Export(outFile);
	}
	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Animations");
	}
#pragma endregion
#pragma region Meshes
	for (const auto& pMesh : m_vecMeshes)
	{
		pMesh->Export(outFile, m_eType);
	}
	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Meshes");
	}
#pragma endregion
#pragma region Materials
	for (const auto& tMaterial : m_vecMaterials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			tMaterial.pTexture[i]->Export(outFile);
		}
	}
	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Materials");
	}
#pragma endregion
#pragma region Shaders
	if (!m_bManualShader)
	{
		outFile.write(reinterpret_cast<const _byte*>(m_vecShaderDescs.data()),	sizeof(SHADERDESC) * m_iNumMeshes);
		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			_bool bSubShaderPass = m_vecSubShaderPass[i];;
			outFile.write(reinterpret_cast<const _byte*>(&bSubShaderPass), sizeof(_bool));
		}
	}
	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Shaders");
	}
#pragma endregion

	outFile.close();

	return S_OK;
}
#endif
