#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CControl final : public CBehavior
{
private:
	explicit CControl(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CControl(const CControl&);
	virtual ~CControl() DEFAULT;

public:
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, const CHARACTERDESC*);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	void							Handle_Input(_float fTimeDelta);

private:
	const CHARACTERDESC*			m_pCharacterDesc{};
	
	shared_ptr<class CTransform>	m_pTargetTransform;
	shared_ptr<class CPhysics>		m_pTargetPhysics;

public:
	static shared_ptr<CControl>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, shared_ptr<class CGameObject> pOwner, const CHARACTERDESC*);
	virtual shared_ptr<CBehavior>	Clone(any = any()) override;
};

END
