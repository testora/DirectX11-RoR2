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

_float3 Function::Rotate_By_Transform(shared_ptr<CTransform> _pTransform, _float3 _vTranslation)
{
	_vector vScale, vQuaternion, vTranslation;
	
	if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, _pTransform->Get_Matrix()))
	{
		return XMVector3Rotate(XMLoadFloat3(&_vTranslation), vQuaternion);
	}

	return _vTranslation;
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

_float Function::RelativeAngle(shared_ptr<CTransform> _pOwner, shared_ptr<CTransform> _pOpponent)
{
	_float3 vOwner		= _pOwner->Get_State(TRANSFORM::POSITION);
	_float3 vOpponent	= _pOpponent->Get_State(TRANSFORM::POSITION);

	_float3 vLook		= _pOwner->Get_State(TRANSFORM::LOOK).normalize();
	_float3 vRelative	= _float3(vOpponent.x - vOwner.x, 0.f, vOpponent.z - vOwner.z).normalize();

	_float3	vCross		= XMVector3Cross(vLook, vRelative);
	_float	fDot		= XMVectorGetX(XMVector3Dot(vLook, vRelative));
	_float	fAngle		= acosf(Function::Clamp(-1.f, 1.f, fDot));

	if (vCross.y < 0.f)	fAngle = -fAngle;

	return fAngle;
}
