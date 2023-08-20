#include "EnginePCH.h"
#include "Picker.h"
#include "PipeLine.h"

HRESULT CPicker::Initialize(HWND _hWnd, ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	m_hWnd		= _hWnd;
	m_pDevice	= _pDevice;
	m_pContext	= _pContext;

	return S_OK;
}

void CPicker::Tick()
{
	POINT	ptCursor;
	RECT	rcClient;
	GetCursorPos(&ptCursor);
	ScreenToClient(m_hWnd, &ptCursor);
	GetClientRect(m_hWnd, &rcClient);

	_float3		vCursor(2.f * ptCursor.x / rcClient.right - 1.f, -2.f * ptCursor.y / rcClient.bottom + 1.f, 0.f);

	_float4x4	mView = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW);
	_float4x4	mProj = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJ);

	m_vRay[IDX(PICKER::WORLD)] = _float3(XMVector3TransformNormal(XMVector3TransformCoord(vCursor, mProj.inverse()), mView.inverse())).normalize();
	m_vOrg[IDX(PICKER::WORLD)] = XMVector3TransformCoord(_float3(0.f, 0.f, 0.f), mView.inverse());
}

_bool CPicker::Intersect(_Out_ _float3& _vOut, const _float3 _vA, const _float3 _vB, const _float3 _vC, _In_opt_ const _float4x4 _mWorld)
{
	_vOut = _float3(0.f, 0.f, 0.f);
	PICKER eSystem = _mWorld == g_mUnit ? PICKER::WORLD : PICKER::LOCAL;

	if (PICKER::WORLD == eSystem)
	{
		m_vOrg[IDX(PICKER::LOCAL)] = XMVector3TransformCoord(m_vOrg[IDX(PICKER::WORLD)], _mWorld.inverse());
		m_vRay[IDX(PICKER::LOCAL)] = _float3(XMVector3TransformNormal(m_vRay[IDX(PICKER::WORLD)], _mWorld.inverse())).normalize();
	}

	_float fDist;
	if (TriangleTests::Intersects(m_vOrg[IDX(eSystem)], m_vRay[IDX(eSystem)], _vA, _vB, _vC, fDist))
	{
		_vOut = m_vOrg[IDX(eSystem)] + m_vRay[IDX(eSystem)] * fDist;
		if (PICKER::WORLD == eSystem)
		{
			_vOut = XMVector3TransformCoord(_vOut, _mWorld);
		}

		return true;
	}

	return false;
}
