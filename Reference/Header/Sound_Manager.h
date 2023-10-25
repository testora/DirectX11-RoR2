#pragma once
#include "Engine_Define.h"

#define MAX_CHANNEL	16

BEGIN(Engine)

class CSound_Manager final : public CSingleton<CSound_Manager>
{
private:
	explicit CSound_Manager() DEFAULT;
	virtual ~CSound_Manager();

public:
	HRESULT						Initialize(const SOUND_CHANNEL eMaxChannel);

public:
	HRESULT						Load(const wstring& wstrDirectory);

	void						Play_Sound(const wstring& wstrSoundKey, SOUND_CHANNEL eChannel, _float fVolume = 1.f, _bool bLoop = false);
	void						Play_Sound(const wstring& wstrSoundKey, SOUND_CHANNEL eChannel, _float3 vSrc, _float3 vDst, _bool bLoop = false);
	void						Stop_Sound(SOUND_CHANNEL eChannel);

	void						Set_Sound_Distance(pair<_float, _float> pairDistance)		{ m_pairDistance = pairDistance; }
	void						Set_Sound_Volume(pair<_float, _float> pairVolume)			{ m_pairVolume = pairVolume; }
	void						Set_Channel_Volume(SOUND_CHANNEL eChannel, _float fVolume);
	void						Set_Channel_Volume_Distance(SOUND_CHANNEL eChannel, _float fDistance);

	void						Reset();

private:
	FMOD::System*				m_pSystem		= { nullptr };
	vector<FMOD::Channel*>		m_vecChannels;

	map<wstring, FMOD::Sound*>	m_mapSounds;

	pair<_float, _float>		m_pairDistance	= { 0.f, 0.f };
	pair<_float, _float>		m_pairVolume	= { 0.f, 1.f };

	SOUND_CHANNEL				m_eMaxChannel	= static_cast<SOUND_CHANNEL>(0);

	friend CSingleton<CSound_Manager>;
};

END
