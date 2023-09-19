#include "ClientPCH.h"
#include "RailGunner_State.h"
#include "RailGunner.h"
#include "GameInstance.h"

HRESULT CRailGunner_State::Initialize(shared_ptr<CRailGunner> _pRailGunner)
{
	if (nullptr == _pRailGunner)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Invaild Parameter");
	}
	m_pRailGunner = _pRailGunner;

	m_pGrounding = m_pRailGunner->Get_Behavior<CGrounding>(BEHAVIOR::GROUNDING);
	if (nullptr == m_pGrounding)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Failed to Get_Behavior: GROUNDING");
	}

	m_bitState.reset();

	m_bitState.set(IDX(RG_STATE::READY_PISTOL), true);

	return S_OK;
}

void CRailGunner_State::Tick(_float _fTimeDelta)
{
	Handle_State();
	Handle_Skill(_fTimeDelta);
}

void CRailGunner_State::Late_Tick(_float _fTimeDelta)
{
}

_bool CRailGunner_State::Is_State(bitset<IDX(RG_STATE::MAX)> _bit) const
{
	return (m_bitState & _bit) == _bit;
}

bitset<IDX(RG_STATE::MAX)> CRailGunner_State::Get_State() const
{
	return m_bitState;
}

void CRailGunner_State::Set_State(RG_STATE _eState, _bool _bValue)
{
	m_bitState.set(IDX(_eState), _bValue);
}

void CRailGunner_State::Handle_State()
{
	shared_ptr<CGameInstance>	pGameInstance	= CGameInstance::Get_Instance();
	shared_ptr<CPhysics>		pPhysics		= m_pRailGunner->Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	shared_ptr<CAnimator>		pAnimator		= m_pRailGunner->Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR);

#pragma region Air
	if (m_bitState.test(IDX(RG_STATE::AIR)))
	{
		switch (pAnimator->Current_Animation<ANIMATION::RAILGUNNER>())
		{
		case ANIMATION::RAILGUNNER::JUMP_START:
		{
			if (pAnimator->Is_Finished())
			{
				pAnimator->Play_Animation(ANIMATION::RAILGUNNER::AIR_LOOP_UP, 1.f, false, 0.25f, false);
			}
		}
		break;
		case ANIMATION::RAILGUNNER::AIR_LOOP_UP:
		{
			if (pAnimator->Is_Finished())
			{
				if (pPhysics->Get_Velocity().y < 0.f)
				{
					pAnimator->Play_Animation(ANIMATION::RAILGUNNER::AIR_LOOP_DOWN, 1.f, false, 0.25f, false);
				}
			}
		}
		break;
		case ANIMATION::RAILGUNNER::AIR_LOOP_DOWN:
		break;

		default:
		{
			if (pPhysics->Get_Velocity().y >= 15.f)
			{
				pAnimator->Play_Animation(ANIMATION::RAILGUNNER::AIR_LOOP_UP, 1.f, false, 0.25f, false);
			}
			if (pPhysics->Get_Velocity().y < -15.f)
			{
				pAnimator->Play_Animation(ANIMATION::RAILGUNNER::AIR_LOOP_DOWN, 1.f, false, 0.25f, false);
			}
		}
		break;
		}
	}
	m_bitState.set(IDX(RG_STATE::AIR), !m_pGrounding->Is_Grounding());
#pragma endregion
#pragma region Sprint
	if (m_bitState.test(IDX(RG_STATE::SPRINT)))
	{
		m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::SPRINT);

		if (pGameInstance->Key_Up(CONTROL_FORWARD))
		{
			m_pRailGunner->Set_State(RG_STATE::SPRINT, false);
			m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::MAIN);
		}

		if (pGameInstance->Key_Down(CONTROL_ATTACK1))
		{
			m_pRailGunner->Set_State(RG_STATE::SPRINT, false);
			m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::MAIN);
		}

		if (pGameInstance->Key_Down(CONTROL_ATTACK2))
		{
			m_pRailGunner->Set_State(RG_STATE::SPRINT, false);
			m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::SCOPE);
		}
	}
#pragma endregion
}

void CRailGunner_State::Handle_Skill(_float _fTimeDelta)
{
	if (m_pRailGunner->Is_Crosshair(RG_CROSSHAIR::MAIN))
	{
		if (!m_bitState.test(IDX(RG_STATE::READY_PISTOL)))
		{
			m_arrStateTime[IDX(RG_STATE::READY_PISTOL)] += _fTimeDelta;
			if (RAILGUNNER_COOLTIME_PISTOL <= m_arrStateTime[IDX(RG_STATE::READY_PISTOL)])
			{
				m_arrStateTime[IDX(RG_STATE::READY_PISTOL)] = 0.f;
				m_bitState.set(IDX(RG_STATE::READY_PISTOL), true);
			}
		}
	}
	if (m_bitState.test(IDX(RG_STATE::READY_PISTOL)))
	{

	}
	if (m_bitState.test(IDX(RG_STATE::READY_PISTOL)))
	{

	}
	if (m_bitState.test(IDX(RG_STATE::READY_PISTOL)))
	{

	}
}

shared_ptr<CRailGunner_State> CRailGunner_State::Create(shared_ptr<CRailGunner> _pRailGunner)
{
	shared_ptr<CRailGunner_State> pInstance = make_private_shared(CRailGunner_State);

	if (FAILED(pInstance->Initialize(_pRailGunner)))
	{
		MSG_RETURN(nullptr, "CRailGunner_State::Create", "Failed To Initialize");
	}

	return pInstance;
}
