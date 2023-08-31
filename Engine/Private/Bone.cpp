#include "EnginePCH.h"
#include "Bone.h"

#if ACTIVATE_TOOL
HRESULT CBone::Initialize_FromAssimp(const aiNode* _pAINode, _uint _iParentBoneIndex)
{
	strcpy_s(m_szName, _pAINode->mName.C_Str());

	memcpy(&m_mTransformation, &_pAINode->mTransformation, sizeof(_float4x4));

	m_mTransformation			= XMMatrixTranspose(m_mTransformation);
	m_mCombinedTransformation	= m_mTransformation;
	m_iParentBoneIndex			= _iParentBoneIndex;

	return S_OK;
}
#endif

HRESULT CBone::Initialize_FromBinary(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(m_szName),			MAX_PATH);
	_inFile.read(reinterpret_cast<_byte*>(&m_iParentBoneIndex),	sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_mTransformation),	sizeof(_float4x4));

	m_mCombinedTransformation	= m_mTransformation;

	return S_OK;
}

void CBone::Set_Transformation(_vectorf _vScale, _vectorf _vRotation, _vectorf _vTranslation)
{
	m_mTransformation = XMMatrixAffineTransformation(_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), _vRotation, _vTranslation);
}

void CBone::Update_CombinedTransformation(vector<shared_ptr<CBone>> _vecModelBones)
{
	if (m_iParentBoneIndex != g_iMaxBones)
	{
		m_mCombinedTransformation = m_mTransformation * _vecModelBones[m_iParentBoneIndex]->m_mCombinedTransformation;
	}
}

#if ACTIVATE_TOOL
shared_ptr<CBone> CBone::Create(const aiNode* _pAINode, _uint _iParentBoneIndex)
{
	shared_ptr<CBone> pInstance = make_private_shared(CBone);

	if (FAILED(pInstance->Initialize_FromAssimp(_pAINode, _iParentBoneIndex)))
	{
		MSG_RETURN(nullptr, "CBone::Create", "Failed to Initialize_FromAssimp");
	}

	return pInstance;
}
#endif

shared_ptr<CBone> CBone::Read(std::ifstream& _inFile)
{
	shared_ptr<CBone> pInstance = make_private_shared(CBone);

	if (FAILED(pInstance->Initialize_FromBinary(_inFile)))
	{
		MSG_RETURN(nullptr, "CBone::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

shared_ptr<CBone> CBone::Clone()
{
	return make_private_shared_copy(CBone, *this);
}

#if ACTIVATE_TOOL
void CBone::Export(std::ofstream& _outFile)
{
	_outFile.write(reinterpret_cast<const _byte*>(m_szName),			MAX_PATH);
	_outFile.write(reinterpret_cast<const _byte*>(&m_iParentBoneIndex),	sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_mTransformation),	sizeof(_float4x4));
}
#endif
