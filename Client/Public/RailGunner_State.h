#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Engine)
class CTransform;
class CModel;
class CPhysics;
class CGrounding;
class CAnimator;
END

BEGIN(Client)

class CRailGunner_State final : public Engine::ISystem
{
private:
	explicit CRailGunner_State() DEFAULT;
	virtual ~CRailGunner_State() DEFAULT;

public:
	HRESULT									Initialize(shared_ptr<class CRailGunner>);
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;

public:
	_bool									Is_State(bitset<IDX(RG_STATE::MAX)>) const;
	bitset<IDX(RG_STATE::MAX)>				Get_State() const;
	void									Set_State(RG_STATE, _bool = true);

private:
	void									Handle_Aim();
	void									Handle_State();
	void									Handle_Skill(_float fTimeDelta);
	void									Handle_Backpack();

private:
	bitset<IDX(RG_STATE::MAX)>				m_bitState;
	array<_float, IDX(RG_STATE::MAX)>		m_arrStateTime{};

	weak_ptr<class CRailGunner>				m_pRailGunner;
	shared_ptr<class CTransform>			m_pTransform;
	shared_ptr<class CModel>				m_pModel;
	shared_ptr<class CPhysics>				m_pPhysics;
	shared_ptr<class CGrounding>			m_pGrounding;
	shared_ptr<class CAnimator>				m_pAnimator;

	_uint									m_iBackPackBoneIndex	= 0;
	_float4x4								m_mAimPivot;
	_float4x4								m_mNonAimPivot;

public:
	static shared_ptr<CRailGunner_State>	Create(shared_ptr<class CRailGunner>);
};

END
