#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CChannel final
{
private:
	explicit CChannel() DEFAULT;
	virtual ~CChannel() DEFAULT;

public:
#if ACTIVATE_TOOL
	HRESULT						Initialize_FromAssimp(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
#endif
	HRESULT						Initialize_FromBinary(std::ifstream&);

public:
	void						Update_Transformation(vector<shared_ptr<class CBone>>::iterator itBegin, _uint& iCurrentKeyFrame, _float fTrackPosition);

private:
	_uint						m_iBoneIndex		= g_iMaxBones;

	_uint						m_iNumKeyFrames		= 0;
	vector<KEYFRAME>			m_vecKeyFrames;

public:
#if ACTIVATE_TOOL
	static shared_ptr<CChannel>	Create(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
#endif
	static shared_ptr<CChannel>	Read(std::ifstream&);

#if ACTIVATE_TOOL
	void						Export(std::ofstream&);
#endif
};

END
