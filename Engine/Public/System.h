#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL ISystem abstract : public std::enable_shared_from_this<ISystem>
{
protected:
	explicit ISystem() DEFAULT;
	virtual ~ISystem() DEFAULT;

public:
	virtual void		Tick(_float fTimeDelta);
	virtual void		Late_Tick(_float fTimeDelta);
	virtual HRESULT		Render();
};

END

using Engine::ISystem;
