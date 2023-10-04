#include "ClientPCH.h"
#include "Client_Function.h"
#include "Client_Define.h"
#include "GameInstance.h"

shared_ptr<CGameObject> Function::Find_Player()
{
	shared_ptr<CGameInstance>	pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CGameObject>		pPlayer;

	pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER)->Iterate_Objects(
		[&](shared_ptr<CGameObject> _pObject)->_bool
		{
			pPlayer = _pObject;
			return false;
		}
	);

	return pPlayer;
}

_float2 Function::Clip_To_Screen(_float3 _vClipSpace, _bool _bCenterSpace)
{
	if (_bCenterSpace)
	{
		return _float2(_vClipSpace) * 0.5f * _float2(static_cast<_float>(g_iWinCX), static_cast<_float>(g_iWinCY));
	}
	else
	{
		return (_float2(_vClipSpace) * _float2(0.5f, -0.5f) + _float2(0.5f, 0.5f)) * _float2(static_cast<_float>(g_iWinCX), static_cast<_float>(g_iWinCY));
	}
}

_bool Function::Is_Cursor_In(_float2 _vPosition, _float2 _vSize)
{
	POINT	ptCursor = CGameInstance::Get_Instance()->Get_CursorPos();

	return Function::InRange(ptCursor.x, static_cast<_long>(_vPosition.x - _vSize.x * 0.5f), static_cast<_long>(_vPosition.x + _vSize.x * 0.5f))
		&& Function::InRange(ptCursor.y, static_cast<_long>(_vPosition.y - _vSize.y * 0.5f), static_cast<_long>(_vPosition.y + _vSize.y * 0.5f));
}

_float Function::Distance_Camera(_float3 _vPosition)
{
	return _float3(_vPosition - CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION)).length();
}

_float Function::Distance(shared_ptr<CTransform> _pTransformA, shared_ptr<CTransform> _pTransformB)
{
	return _float3(_pTransformA->Get_State(TRANSFORM::POSITION) - _pTransformB->Get_State(TRANSFORM::POSITION)).length();
}
