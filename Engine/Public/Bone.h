#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBone final
{
private:
	explicit CBone() DEFAULT;
	explicit CBone(const CBone&) DEFAULT;
	virtual ~CBone() DEFAULT;

public:
#if ACTIVATE_TOOL
	HRESULT						Initialize_FromAssimp(const aiNode* pAINode, _uint iParentBoneIndex);
#endif
	HRESULT						Initialize_FromBinary(std::ifstream&);

public:
	const _char*				Get_Name() const								{ return m_szName; }
	_float4x4					Get_CombinedTransformation() const				{ return m_mCombinedTransformation; }
	const _float4x4*			Get_CombinedTransformationPointer()				{ return &m_mCombinedTransformation; }

	void						Set_Transformation(_vectorf vScale, _vectorf vRotation, _vectorf vTranslation);
	void						Blend_Transformation(_vectorf vScale, _vectorf vRotation, _vectorf vTranslation, _matrixc mBase);

public:
	void						Update_CombinedTransformation(vector<shared_ptr<CBone>>::iterator itBegin, _matrixf mPivot = g_mUnit);

private:
	_char						m_szName[MAX_PATH]			= "";
	_uint						m_iParentBoneIndex			= g_iMaxBones;

	_float4x4					m_mTransformation			= g_mUnit;
	_float4x4					m_mCombinedTransformation	= g_mUnit;
	_float4x4					m_mBlendTransformation		= g_mUnit;

public:
#if ACTIVATE_TOOL
	static shared_ptr<CBone>	Create(const aiNode* pAINode, _uint iParentBoneIndex);
#endif
	static shared_ptr<CBone>	Read(std::ifstream&);
	shared_ptr<CBone>			Clone();

#if ACTIVATE_TOOL
	void						Export(std::ofstream&);
#endif
};

END
