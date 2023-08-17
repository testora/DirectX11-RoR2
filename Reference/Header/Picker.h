#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CPicker final : public CSingleton<CPicker>
{
public:
	enum STATE	{ LOCAL, WORLD, MAX };

private:
	explicit CPicker() DEFAULT;
	virtual ~CPicker() DEFAULT;

public:
	HRESULT						Initialize(HWND, ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	void						Tick();

private:
	_bool						Intersect(_Out_ _float3&, const _float3 vA, const _float3 vB, const _float3 vC, _In_opt_ const _float4x4 mWorld = g_mUnit);

private:
	_float3						m_vRay[STATE::MAX]{};
	_float3						m_vOrg[STATE::MAX]{};

	HWND						m_hWnd	= 0;

	ComPtr<ID3D11Device>		m_pDevice;
	ComPtr<ID3D11DeviceContext>	m_pContext;

	friend class CVIBuffer;
	friend CSingleton<CPicker>;
};

END
