#include "EnginePCH.h"
#include "Channel.h"
#include "Model.h"
#include "Bone.h"

#if ACTIVATE_TOOL
HRESULT CChannel::Initialize_FromAssimp(const aiNodeAnim* _pAIChannel, shared_ptr<CModel> _pModel)
{
	m_iBoneIndex	= _pModel->Get_BoneIndex(_pAIChannel->mNodeName.C_Str());
	m_iNumKeyFrames	= Function::Max(_pAIChannel->mNumScalingKeys, _pAIChannel->mNumRotationKeys, _pAIChannel->mNumPositionKeys);

	m_vecKeyFrames.reserve(m_iNumKeyFrames);

	_float3	vScale;
	_float4	vRotation;
	_float3	vTranslation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME tKeyFrame{};

		if (i < _pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &_pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			tKeyFrame.fTime = static_cast<_float>(_pAIChannel->mScalingKeys[i].mTime);
		}

		if (i < _pAIChannel->mNumRotationKeys)
		{
			vRotation.x		= _pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y		= _pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z		= _pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w		= _pAIChannel->mRotationKeys[i].mValue.w;
			tKeyFrame.fTime	= static_cast<_float>(_pAIChannel->mRotationKeys[i].mTime);
		}

		if (i < _pAIChannel->mNumPositionKeys)
		{
			memcpy(&vTranslation, &_pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			tKeyFrame.fTime = static_cast<_float>(_pAIChannel->mPositionKeys[i].mTime);
		}

		memcpy(&tKeyFrame.vScale, &vScale, sizeof(_float3));
		memcpy(&tKeyFrame.vRotation, &vRotation, sizeof(_float4));
		memcpy(&tKeyFrame.vTranslation, &vTranslation, sizeof(_float3));
		tKeyFrame.vTranslation.w = 1.f;

		m_vecKeyFrames.push_back(tKeyFrame);
	}
		
	return S_OK;
}
#endif

HRESULT CChannel::Initialize_FromBinary(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_iBoneIndex),			sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumKeyFrames),		sizeof(_uint));

	m_vecKeyFrames.resize(m_iNumKeyFrames);

	_inFile.read(reinterpret_cast<_byte*>(m_vecKeyFrames.data()),	sizeof(KEYFRAME) * m_iNumKeyFrames);

	return S_OK;
}

void CChannel::Update_Transformation(vector<shared_ptr<CBone>> _vecModelBones, _uint& _iCurrentKeyFrame, _float _fTrackPosition)
{
	if (0.f == _fTrackPosition)
	{
		_iCurrentKeyFrame = 0;
	}
	
	_float	fRatio;
	_vector	vScale, vRotation, vTranslation;

	if (_fTrackPosition < m_vecKeyFrames.back().fTime)
	{
		while (_fTrackPosition > m_vecKeyFrames[_iCurrentKeyFrame + 1].fTime)
		{
			++_iCurrentKeyFrame;
		}

		fRatio			= Function::ProportionalRatio(m_vecKeyFrames[_iCurrentKeyFrame].fTime, m_vecKeyFrames[_iCurrentKeyFrame + 1].fTime, _fTrackPosition);

		vScale			= XMVectorLerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vScale), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vScale), fRatio);
		vRotation		= XMQuaternionSlerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vRotation), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vRotation), fRatio);
		vTranslation	= XMVectorLerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vTranslation), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vTranslation), fRatio);
	}
	else
	{
		vScale			= _float4(m_vecKeyFrames.back().vScale);
		vRotation		= _float4(m_vecKeyFrames.back().vRotation);
		vTranslation	= _float4(m_vecKeyFrames.back().vTranslation);
	}

	_vecModelBones[m_iBoneIndex]->Set_Transformation(vScale, vRotation, vTranslation);
}

#if ACTIVATE_TOOL
shared_ptr<CChannel> CChannel::Create(const aiNodeAnim* _pAIChannel, shared_ptr<CModel> _pModel)
{
	shared_ptr<CChannel> pInstance = make_private_shared(CChannel);

	if (FAILED(pInstance->Initialize_FromAssimp(_pAIChannel, _pModel)))
	{
		MSG_RETURN(nullptr, "CChannel::Create", "Failed to Initialize_FromAssimp");
	}

	return pInstance;
}
#endif

shared_ptr<CChannel> CChannel::Read(std::ifstream& _inFile)
{
	shared_ptr<CChannel> pInstance = make_private_shared(CChannel);

	if (FAILED(pInstance->Initialize_FromBinary(_inFile)))
	{
		MSG_RETURN(nullptr, "CChannel::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
void CChannel::Export(std::ofstream& _outFile)
{
	_outFile.write(reinterpret_cast<const _byte*>(&m_iBoneIndex),			sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumKeyFrames),		sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecKeyFrames.data()),	sizeof(KEYFRAME) * m_iNumKeyFrames);
}
#endif
