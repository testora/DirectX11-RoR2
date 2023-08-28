#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBone final
{
private:
	explicit CBone(_uint iParentBoneIndex);
	explicit CBone(const CBone&) DEFAULT;
	virtual ~CBone() DEFAULT;

public:
	HRESULT						Initialize(const aiNode* pAINode);

public:
	const _char*				Get_Name() const								{ return m_szName; }
	_float4x4					Get_CombinedTransformation() const				{ return m_mCombinedTransformation; }

	void						Set_Transformation(_vectorf _vScale, _vectorf _vRotation, _vectorf _vTranslation);

public:
	void						Update_CombinedTransformation(vector<shared_ptr<CBone>>);

private:
	_char						m_szName[MAX_PATH]			= "";
	const _uint					m_iParentBoneIndex			= g_iMaxBones;

	_float4x4					m_mTransformation;
	_float4x4					m_mCombinedTransformation;

public:
	static shared_ptr<CBone>	Create(const aiNode* pAINode, _uint iParentBoneIndex);
	shared_ptr<CBone>			Clone();
};

END
