#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CPhysics final : public CBehavior
{
    explicit CPhysics(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
    virtual ~CPhysics() DEFAULT;
};

END
