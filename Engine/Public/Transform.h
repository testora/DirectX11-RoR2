#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	explicit CTransform(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CTransform() DEFAULT;

public:
	_float4x4	Get_Matrix() const;
	_float3		Get_State(const TRANSFORM eState) const;
	_float3		Get_Scale() const;

	void		Set_State(const TRANSFORM eState, const _float3 vState);
	void		Set_Scale(const _float3 vScale);

public:
	void		Multiply(const _float4x4 mMatrix);

	void		Translate(const _float3 vPosition);
	void		Rotate(const _float3 vAxis, const _float fDegree);
	void		LookAt(const _float3 vPosition);
	void		LookTo(const _float3 vDirection);

private:
	_float4x4	m_mWorld{};
};

END
