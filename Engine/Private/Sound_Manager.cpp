#include "EnginePCH.h"
#include "Sound_Manager.h"
#include "Event_Handler.h"

#include <thread>
#include <io.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

CSound_Manager::~CSound_Manager()
{
	for (auto& myPair : m_mapSounds)
	{
		myPair.second->release();
	}
	m_mapSounds.clear();

	m_pSystem->release();
	m_pSystem->close();
}

HRESULT CSound_Manager::Initialize(const SOUND_CHANNEL _eMaxChannel)
{
	if (FMOD_OK != FMOD::System_Create(&m_pSystem))
	{
		MSG_RETURN(E_FAIL, "CSound_Manager::Initialize", "Failed to System_Create");
	}

	_uint iVersion(FMOD_VERSION);
	if (FMOD_OK != m_pSystem->getVersion(&iVersion))
	{
		MSG_RETURN(E_FAIL, "CSound_Manager::Initialize", "Failed to getVersion");
	}

	if (FMOD_OK != m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr))
	{
		MSG_RETURN(E_FAIL, "CSound_Manager::Initialize", "Failed to init");
	}

	m_eMaxChannel = _eMaxChannel;
	m_vecChannels.resize(IDX(_eMaxChannel));

	return S_OK;
}

HRESULT CSound_Manager::Load(const wstring& _wstrDirectory)
{
	// _finddata_t : presented by <io.h>, struct that stores file info
	_finddata_t fd;

	//_findfirst : presented by <io.h>, finds the first file at the path

	string	strDirPath	= Function::ToString(_wstrDirectory);
	string	strFindPath	= strDirPath + "*.*";
	auto	handle		= _findfirst(strFindPath.c_str(), &fd);

	if (handle == -1)
	{
		MSG_RETURN(E_FAIL, "CSound_Manager::Load", "Failed to _findfirst");
	}

	_int	iResult(0);
	_char	szFullPath[MAX_PATH]{};
	while (iResult != -1)
	{
		FMOD::Sound* pSound = nullptr;

		strcpy_s(szFullPath, strDirPath.c_str());
		PathAddBackslashA(szFullPath);
		strcat_s(szFullPath, fd.name);

		if (FMOD_OK == m_pSystem->createSound(szFullPath, FMOD_LOOP_NORMAL, 0, &pSound))
		{
			PathRemoveExtensionA(fd.name);
			size_t	iLength		= strlen(fd.name) + 1;
			_wchar*	pSoundKey	= new _wchar[iLength]{};

			MultiByteToWideChar(CP_ACP, 0, fd.name, (_int)iLength, pSoundKey, (_int)iLength);

			wstring wstr(pSoundKey, pSoundKey + lstrlen(pSoundKey));
			m_mapSounds.emplace(wstr, pSound);

			Safe_Delete_Array(pSoundKey);
		}

		//_findnext : presented by <io.h>, finds the next file of location, if not, returns -1
		iResult = _findnext(handle, &fd);
	}

	m_pSystem->update();

	_findclose(handle);

	return S_OK;
}

void CSound_Manager::Play_Sound(const wstring& _wstrSoundKey, SOUND_CHANNEL _eChannel, _float _fVolume, _float _fSpeed, _bool _bLoop, _bool _bSingleAudio, _float _fStart, _float _fEnd)
{
	if (_bSingleAudio)
	{
		_bool bIsPlaying;
		m_vecChannels[IDX(_eChannel)]->isPlaying(&bIsPlaying);
		if (bIsPlaying)
		{
			return;
		}
	}

	auto iter = m_mapSounds.find(_wstrSoundKey);
	if (iter == m_mapSounds.end())
	{
		return;
	}

	m_pSystem->playSound(iter->second, nullptr, true, &m_vecChannels[IDX(_eChannel)]);

	_uint	_iLength = 0;
	_float	_fOriginalFrequency = 0.f;
	iter->second->getLength(&_iLength, FMOD_TIMEUNIT_MS);
	m_vecChannels[IDX(_eChannel)]->setPosition(static_cast<_uint>(_iLength * _fStart), FMOD_TIMEUNIT_MS);
	m_vecChannels[IDX(_eChannel)]->setLoopPoints(static_cast<_uint>(_iLength * _fStart), FMOD_TIMEUNIT_MS, static_cast<_uint>(_iLength * _fEnd), FMOD_TIMEUNIT_MS);
	m_vecChannels[IDX(_eChannel)]->setMode(_bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	m_vecChannels[IDX(_eChannel)]->setVolume(_fVolume);
	m_vecChannels[IDX(_eChannel)]->getFrequency(&_fOriginalFrequency);
	m_vecChannels[IDX(_eChannel)]->setFrequency(_fOriginalFrequency * _fSpeed);

	m_vecChannels[IDX(_eChannel)]->setPaused(false);

	m_pSystem->update();

	if (!_bLoop)
	{
		_uint iStartPosition		= static_cast<_uint>(_iLength * _fStart);
		_uint iEndPosition			= static_cast<_uint>(_iLength * _fEnd);
		_uint iFadeStartPosition	= static_cast<_uint>(_iLength * _fEnd * 0.9f);

		std::thread([this, _eChannel, iEndPosition, iFadeStartPosition, _fVolume]
			{
				_uint iCurPosition = 0;
				while (true)
				{
					m_vecChannels[IDX(_eChannel)]->getPosition(&iCurPosition, FMOD_TIMEUNIT_MS);

					if (iCurPosition >= iFadeStartPosition)
					{
						_float fFadeFactor	= static_cast<_float>(iEndPosition - iCurPosition) / static_cast<_float>(iEndPosition - iFadeStartPosition);
						_float fFadeVolume	= _fVolume * fFadeFactor;
						Set_Channel_Volume(_eChannel, fFadeVolume);
					}

					if (iCurPosition >= iEndPosition)
					{
						m_vecChannels[IDX(_eChannel)]->setPaused(true);
						break;
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
		).detach();
	}
}

void CSound_Manager::Play_Sound(const wstring& _wstrSoundKey, SOUND_CHANNEL _eChannel, _float3 _vSrc, _float3 _vDst, _float _fSpeed, _bool _bLoop, _bool _bSingleAudio, _float _fStart, _float _fEnd)
{
	if (_bSingleAudio)
	{
		_bool bIsPlaying;
		m_vecChannels[IDX(_eChannel)]->isPlaying(&bIsPlaying);
		if (bIsPlaying)
		{
			return;
		}
	}

	_float3	vDistance		= _vSrc - _vDst;
	_float	fDistance		= vDistance.length();
	_float	fClampDistance	= std::clamp(fDistance, m_pairDistance.first, m_pairDistance.second);

	if (0.f < fClampDistance)
	{
		fClampDistance = m_pairVolume.second - std::clamp(fClampDistance / m_pairDistance.second, m_pairVolume.first, m_pairVolume.second);
	}

	Play_Sound(_wstrSoundKey, _eChannel, fClampDistance, _fSpeed, _bLoop, _bSingleAudio, _fSpeed, _fEnd);
}

void CSound_Manager::Play_Sounds(const wstring& _wstrSoundKeyA, const wstring& _wstrSoundKeyB, SOUND_CHANNEL _eChannel, _float _fVolume, _float _fSpeed, _float _fStart, _float _fEnd)
{
	static _bool bPlayA = true;

	_bool bIsPlaying;
	m_vecChannels[IDX(_eChannel)]->isPlaying(&bIsPlaying);
	if (bIsPlaying)
	{
		return;
	}

	Play_Sound(bPlayA ? _wstrSoundKeyA : _wstrSoundKeyB, _eChannel, _fVolume, _fSpeed, false, false, _fStart, _fEnd);

	bPlayA = !bPlayA;
}

void CSound_Manager::Play_Sounds(const wstring& _wstrSoundKeyA, const wstring& _wstrSoundKeyB, SOUND_CHANNEL _eChannel, _float3 _vSrc, _float3 _vDst, _float _fSpeed, _float _fStart, _float _fEnd)
{
	static _bool bPlayA = true;

	_bool bIsPlaying;
	m_vecChannels[IDX(_eChannel)]->isPlaying(&bIsPlaying);
	if (bIsPlaying)
	{
		return;
	}

	Play_Sound(bPlayA ? _wstrSoundKeyA : _wstrSoundKeyB, _eChannel, _vSrc, _vDst, _fSpeed, false, false, _fSpeed, _fEnd);

	bPlayA = !bPlayA;
}

void CSound_Manager::Stop_Sound(SOUND_CHANNEL _eChannel)
{
	if (m_eMaxChannel == _eChannel)
	{
		for (auto& pChannel : m_vecChannels)
		{
			pChannel->stop();
		}
	}
	else
	{
		m_vecChannels[IDX(_eChannel)]->stop();
	}

	m_pSystem->update();
}

void CSound_Manager::Set_Channel_Volume(SOUND_CHANNEL _eChannel, _float _fVolume)
{
	m_vecChannels[IDX(_eChannel)]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSound_Manager::Set_Channel_Volume_Distance(SOUND_CHANNEL _eChannel, _float _fDistance)
{
	_float fVolume = 0.f;

	if (m_pairDistance.second > 0.f)
	{
		fVolume = std::clamp(_fDistance / m_pairDistance.second, m_pairVolume.first, m_pairVolume.second);
	}

	fVolume = std::clamp(m_pairVolume.second - fVolume + m_pairVolume.first, m_pairVolume.first, m_pairVolume.second);

	Set_Channel_Volume(_eChannel, fVolume);
}

void CSound_Manager::Set_Channel_Position(SOUND_CHANNEL _eChannel, _float _fPosition)
{
	_bool bIsPlaying;
	m_vecChannels[IDX(_eChannel)]->isPlaying(&bIsPlaying);
	if (!bIsPlaying)
	{
		return;
	}

	FMOD::Sound* pSound	= nullptr;
	m_vecChannels[IDX(_eChannel)]->getCurrentSound(&pSound);
	if (nullptr == pSound)
	{
		return;
	}

	_uint _iLength = 0;
	pSound->getLength(&_iLength, FMOD_TIMEUNIT_MS);
	if (_fPosition < 0 || _fPosition > _iLength)
	{
		return;
	}

	m_vecChannels[IDX(_eChannel)]->setPosition(static_cast<_uint>(_iLength * _fPosition), FMOD_TIMEUNIT_MS);

	m_pSystem->update();
}

void CSound_Manager::Reset()
{
	m_pairVolume	= { 0.f, 1.f };
	m_pairDistance	= { 0.f, 0.f };

	for (auto& pChannel : m_vecChannels)
	{
		pChannel->setVolume(1.f);
	}

	Stop_Sound(m_eMaxChannel);
}
