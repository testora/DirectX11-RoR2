#pragma once
#include "Client_Define.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
END

namespace Function
{
	_bool					Is_Cursor_In(_float2 vPosition, _float2 vSize);

	_float					Distance_Camera(_float3 vPosition);
	_float					Distance(shared_ptr<CTransform>, shared_ptr<CTransform>);
	_float					RelativeAngle(shared_ptr<CTransform> pOwner, shared_ptr<CTransform> pOpponent);
	_float2					Clip_To_Screen(_float3 vClipSpace, _bool bCenterSpace = false);

	shared_ptr<CGameObject>	Find_Player();
}
