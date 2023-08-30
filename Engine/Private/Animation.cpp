#include "EnginePCH.h"
#include "Animation.h"
#include "Channel.h"

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
			MSG_CONTINUE("CAnimation::Create", "Failed to CChannel::Create");
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

void CAnimation::Tick(_float _fTimeDelta, vector<shared_ptr<class CBone>> _vecModelBones, _bool _bLoop)
{
	m_fTrackPosition += m_fTicksPerSecond * _fTimeDelta;

	if (_bLoop && m_fTrackPosition > m_fDuration)
	{
		m_fTrackPosition = 0.f;
	}

	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		m_vecChannels[i]->Update_Transformation(_vecModelBones, m_vecChannelKeyFrames[i], m_fTrackPosition);
	}
}

#if ACTIVATE_TOOL
shared_ptr<CAnimation> CAnimation::Create(const aiAnimation* _pAIAnimation, shared_ptr<class CModel> _pModel)
{
	shared_ptr<CAnimation> pInstance = make_private_shared(CAnimation);

	if (FAILED(pInstance->Initialize_FromAssimp(_pAIAnimation, _pModel)))
	{
		MSG_RETURN(nullptr, "CAnimation::Create", "Failed to Initialize");
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
