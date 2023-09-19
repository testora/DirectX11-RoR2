#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Engine)
class CGrounding;
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
	void									Handle_State();
	void									Handle_Skill(_float fTimeDelta);

private:
	bitset<IDX(RG_STATE::MAX)>				m_bitState;
	array<_float, IDX(RG_STATE::MAX)>		m_arrStateTime{};

	shared_ptr<class CRailGunner>			m_pRailGunner;
	shared_ptr<class CGrounding>			m_pGrounding;

public:
	static shared_ptr<CRailGunner_State>	Create(shared_ptr<class CRailGunner>);
};

END
