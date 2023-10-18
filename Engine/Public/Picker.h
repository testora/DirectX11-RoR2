#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CPicker final : public CSingleton<CPicker>
{
private:
	explicit CPicker() DEFAULT;
	virtual ~CPicker() DEFAULT;

public:
	HRESULT						Initialize(HWND);
	void						Tick();

public:
	_bool						Intersect(_Out_ _float3&, const _float3 vA, const _float3 vB, const _float3 vC, _In_opt_ const _float4x4 mWorld = g_mUnit);

private:
	_float3						m_vRay[IDX(PICKER::MAX)]{};
	_float3						m_vOrg[IDX(PICKER::MAX)]{};

	HWND						m_hWnd	= 0;

	friend CSingleton<CPicker>;
};

END
