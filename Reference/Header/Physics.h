#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CPhysics final : public CBehavior
{
private:
	explicit CPhysics(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPhysics(const CPhysics&);
	virtual ~CPhysics() DEFAULT;

public:
	virtual HRESULT					Initialize(any = any()) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	static shared_ptr<CPhysics>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CBehavior>	Clone(any = any()) override;
};

END
