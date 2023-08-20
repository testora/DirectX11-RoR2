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

	void							Set_State(const TRANSFORM eState, const _vectorf vState);
	void							Set_Scale(const _vectorf vScale);

public:
	void							Multiply(const _matrixf mMatrix);

	void							Translate(const _vectorf vPosition);
	void							Rotate(const _vectorf vAxis, const _float fDegree);
	void							LookAt(const _vectorf vPosition);
	void							LookTo(const _vectorf vDirection);

private:
	_float4x4						m_mWorld;

public:
	static shared_ptr<CTransform>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>	Clone(any = any()) override;
};

END
