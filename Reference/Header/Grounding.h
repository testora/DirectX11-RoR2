#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CGrounding final : public CBehavior
{
private:
	explicit CGrounding(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGrounding(const CGrounding&);
	virtual ~CGrounding() DEFAULT;

public:
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

private:
	_float3							Intersect_Terrain();

private:
	shared_ptr<class CTransform>	m_pOwnerTransform;

public:
	static shared_ptr<CGrounding>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, shared_ptr<class CGameObject> pOwner);
	virtual shared_ptr<CBehavior>	Clone(any = any()) override;
};

END
