#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
private:
	explicit CTransform(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTransform(const CTransform&);
	virtual ~CTransform() DEFAULT;

public:
	_float4x4						Get_Matrix() const;
	_float3							Get_State(const TRANSFORM eState) const;
	_float3							Get_Scale() const;

	void							Set_Matrix(const _matrixf mMatrix);
	void							Set_State(const TRANSFORM, const _vectorf vState);
	void							Set_Scale(const _vectorf vScale);

public:
	void							Multiply(const _matrixf mMatrix);

	void							Translate(const _vectorf vTranslation);
	void							Rotate(const _vectorf vQuaternion);
	void							Rotate(const _vectorf vAxis, const _float fDegree);
	void							Rotate(const TRANSFORM, const _float fDegree);
	void							LookAt(const _vectorf vPosition, const _bool bFixUp = true, const _bool bPipeLineUp = false);
	void							LookAt(shared_ptr<CTransform>, const _bool bFixUp = true, const _bool bPipeLineUp = false);
	void							LookAt_Interpolation(const _vectorf vPosition, const _bool bFixUp = true, const _bool bPipeLineUp = false, const _float fInterpolationDuration = g_fDefaultInterpolationDuration * 2.f, const _float fWeight = 1.f);
	void							LookAt_Interpolation(shared_ptr<CTransform>, const _bool bFixUp = true, const _bool bPipeLineUp = false, const _float fInterpolationDuration = g_fDefaultInterpolationDuration * 2.f, const _float fWeight = 1.f);
	void							LookTo(const _vectorf vDirection, const _bool bFixUp = true, const _bool bPipeLineUp = false);
	void							LookTo_Interpolation(const _vectorf vDirection, const _bool bFixUp = true, const _bool bPipeLineUp = false, const _float fInterpolationDuration = g_fDefaultInterpolationDuration * 2.f, const _float fWeight = 1.f);

	HRESULT							Bind_OnShader(shared_ptr<class CShader>);

private:
	_float4x4						m_mWorld;

public:
	static shared_ptr<CTransform>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>	Clone(any = g_aNull) override;
};

END
