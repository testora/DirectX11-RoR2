#include "EnginePCH.h"
#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation(const CAnimation& _rhs)
	: m_bReverse			(_rhs.m_bReverse)
	, m_fDuration			(_rhs.m_fDuration)
	, m_fTrackPosition		(_rhs.m_fTrackPosition)
	, m_fTicksPerSecond		(_rhs.m_fTicksPerSecond)
	, m_iNumChannels		(_rhs.m_iNumChannels)
	, m_vecChannelKeyFrames	(_rhs.m_vecChannelKeyFrames)
{
	strcpy_s(m_szName, MAX_PATH, _rhs.m_szName);

	for (auto pOriginal : _rhs.m_vecChannels)
	{
		m_vecChannels.push_back(pOriginal->Clone());
	}
}

#if ACTIVATE_TOOL
HRESULT CAnimation::Initialize_FromAssimp(const aiAnimation* _pAIAnimation, shared_ptr<class CModel> _pModel)
{
	HRESULT hr = S_OK;

	strcpy_s(m_szName, _pAIAnimation->mName.C_Str());

	m_fDuration			= static_cast<_float>(_pAIAnimation->mDuration);
	m_fTicksPerSecond	= static_cast<_float>(_pAIAnimation->mTicksPerSecond);

	m_iNumChannels		= _pAIAnimation->mNumChannels;
	m_vecChannels.reserve(m_iNumChannels);
	m_vecChannelKeyFrames.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		shared_ptr<CChannel> pChannel = CChannel::Create(_pAIAnimation->mChannels[i], _pModel);
		if (nullptr == pChannel)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CAnimation::Initialize_FromAssimp", "Failed to CChannel::Create");
		}

		m_vecChannels.emplace_back(pChannel);
	}

	return hr;
}
#endif

HRESULT CAnimation::Initialize_FromBinary(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(m_szName),			MAX_PATH);
	_inFile.read(reinterpret_cast<_byte*>(&m_fDuration),		sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_fTicksPerSecond),	sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumChannels),		sizeof(_uint));

	m_vecChannels.reserve(m_iNumChannels);
	m_vecChannelKeyFrames.resize(m_iNumChannels);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_vecChannels.emplace_back(CChannel::Read(_inFile));

		if (_inFile.eof())
		{
			int a = 1;
			MSG_BOX("asdf", "Asdf");
		}
	}

	return S_OK;
}

void CAnimation::Tick(_float _fTimeDelta, vector<shared_ptr<class CBone>>::iterator _itBegin, _bool _bReverse, _bool _bLoop)
{
	m_bReverse = _bReverse;

	if (false == _bReverse)
	{
		m_fTrackPosition += m_fTicksPerSecond * _fTimeDelta;
		m_fTrackPosition = min(m_fTrackPosition, m_fDuration);

		if (_bLoop && m_fTrackPosition >= m_fDuration)
		{
			m_fTrackPosition = 0.f;
		}
	}
	else
	{
		m_fTrackPosition -= m_fTicksPerSecond * _fTimeDelta;
		m_fTrackPosition = max(m_fTrackPosition, 0.f);

		if (_bLoop && m_fTrackPosition <= 0.f)
		{
			m_fTrackPosition = m_fDuration;
		}
	}
	
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_vecChannels[i]->Update_Transformation(_itBegin, m_vecChannelKeyFrames[i], m_fTrackPosition);
	}
}

_bool CAnimation::Is_ChannelExist(_uint _iBoneIndex) const
{
	for (auto& pChannel : m_vecChannels)
	{
		if (pChannel->Get_BoneIndex() == _iBoneIndex)
		{
			return true;
		}
	}

	return false;
}

void CAnimation::Fix(vector<shared_ptr<CBone>>::iterator _itBegin, _float _fRatio)
{
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		for (_uint j = 0; j < m_vecChannels[i]->Get_NumKeyFrames() - 1; ++j)
		{
			if (Function::InRange(m_fDuration * _fRatio, m_vecChannels[i]->Get_KeyFrame(j).fTime, m_vecChannels[i]->Get_KeyFrame(j + 1).fTime))
			{
				m_vecChannelKeyFrames[i] = j;
				break;
			}
		}

		m_vecChannels[i]->Update_Transformation(_itBegin, m_vecChannelKeyFrames[i], m_fDuration * _fRatio);
	}
}

void CAnimation::Blend(vector<shared_ptr<CAnimation>>::iterator _itAnimationBegin, vector<shared_ptr<CBone>>::iterator _itBoneBegin, vector<_uint>::iterator _itBoneAnimationBegin, _float _fRatio)
{
	_uint iKeyFrameIndex(0);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		for (_uint j = 0; j < m_vecChannels[i]->Get_NumKeyFrames() - 1; ++j)
		{
			if (Function::InRange(m_fDuration * _fRatio, m_vecChannels[i]->Get_KeyFrame(j).fTime, m_vecChannels[i]->Get_KeyFrame(j + 1).fTime))
			{
				iKeyFrameIndex = j;
				break;
			}
		}

		m_vecChannels[i]->Blend_Transformation(_itBoneBegin, iKeyFrameIndex, m_fDuration * _fRatio, _itAnimationBegin[_itBoneAnimationBegin[m_vecChannels[i]->Get_BoneIndex()]]);
	}
}

shared_ptr<CChannel> CAnimation::Get_Channel(_uint _iChannelIndex, _bool _bBoneIndex) const
{
	if (false == _bBoneIndex)
	{
		return m_vecChannels[_iChannelIndex];
	}
	else
	{
		for (auto& pChannel : m_vecChannels)
		{
			if (pChannel->Get_BoneIndex() == _iChannelIndex)
			{
				return pChannel;
			}
		}
	}

	return nullptr;
}

#if ACTIVATE_TOOL
_uint CAnimation::Get_ChannelIndex(_uint _iBoneIndex) const
{
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (m_vecChannels[i]->Get_BoneIndex() == _iBoneIndex)
		{
			return i;
		}
	}

	return m_iNumChannels;
}
#endif

void CAnimation::Set_AnimationIndex(vector<_uint>::iterator _itBegin, _uint _iAnimationIndex)
{
	for (auto& pChannel : m_vecChannels)
	{
		_itBegin[pChannel->Get_BoneIndex()] = _iAnimationIndex;
	}
}

#if ACTIVATE_TOOL
HRESULT CAnimation::Remove_Channel(_uint _iChannelIndex)
{
	if (_iChannelIndex >= m_iNumChannels)
	{
		MSG_RETURN(E_FAIL, "CAnimation::Remove_Channel", "Invalid Channel Index");
	}

	--m_iNumChannels;
	m_vecChannels.erase(m_vecChannels.begin() + _iChannelIndex);
	m_vecChannelKeyFrames.erase(m_vecChannelKeyFrames.begin() + _iChannelIndex);

	return S_OK;
}
#endif

#if ACTIVATE_TOOL
shared_ptr<CAnimation> CAnimation::Create(const aiAnimation* _pAIAnimation, shared_ptr<class CModel> _pModel)
{
	shared_ptr<CAnimation> pInstance = make_private_shared(CAnimation);

	if (FAILED(pInstance->Initialize_FromAssimp(_pAIAnimation, _pModel)))
	{
		MSG_RETURN(nullptr, "CAnimation::Create", "Failed to Initialize_FromAssimp");
	}

	return pInstance;
}
#endif

shared_ptr<CAnimation> CAnimation::Read(std::ifstream& _inFile)
{
	shared_ptr<CAnimation> pInstance = make_private_shared(CAnimation);

	if (FAILED(pInstance->Initialize_FromBinary(_inFile)))
	{
		MSG_RETURN(nullptr, "CAnimation::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

shared_ptr<CAnimation> CAnimation::Clone()
{
	return make_private_shared_copy(CAnimation, *this);
}

#if ACTIVATE_TOOL
void CAnimation::Export(std::ofstream& _outFile)
{
	_outFile.write(reinterpret_cast<const _byte*>(m_szName),			MAX_PATH);
	_outFile.write(reinterpret_cast<const _byte*>(&m_fDuration),		sizeof(_float));
	_outFile.write(reinterpret_cast<const _byte*>(&m_fTicksPerSecond),	sizeof(_float));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumChannels),		sizeof(_uint));

	for (auto& pChannel : m_vecChannels)
	{
		pChannel->Export(_outFile);
	}
}
#endif
