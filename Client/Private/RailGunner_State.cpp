#include "ClientPCH.h"
#include "RailGunner_State.h"
#include "RailGunner.h"
#include "GameInstance.h"
#include "Pipeline.h"
#include "Animation.h"
#include "Channel.h"
#include "Camera_Main.h"

HRESULT CRailGunner_State::Initialize(shared_ptr<CRailGunner> _pRailGunner)
{
	if (nullptr == _pRailGunner)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Invaild Parameter");
	}
	m_pRailGunner = _pRailGunner;

	m_pTransform = _pRailGunner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Failed to Get_Component: TRANSFORM");
	}

	m_pModel = _pRailGunner->Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Failed to Get_Component: MODEL");
	}

	m_pPhysics = _pRailGunner->Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Failed to Get_Behavior: PHYSICS");
	}

	m_pGrounding = _pRailGunner->Get_Behavior<CGrounding>(BEHAVIOR::GROUNDING);
	if (nullptr == m_pGrounding)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Failed to Get_Behavior: GROUNDING");
	}

	m_pAnimator = _pRailGunner->Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR);
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_State::Initialize", "Failed to Get_Behavior: ANIMATOR");
	}

	m_bitState.reset();

	m_bitState.set(IDX(RG_STATE::READY_PISTOL), true);

	KEYFRAME tKeyFrameBody		= m_pModel->Get_Animation(IDX(ANIMATION::RAILGUNNER::IDLE))->
		Get_Channel(m_pModel->Get_BoneIndex("backpack"), true)->Get_KeyFrame(0);
	KEYFRAME tKeyFrameBackpack	= m_pModel->Get_Animation(IDX(ANIMATION::RAILGUNNER::BACKPACK_IDLE))->
		Get_Channel(m_pModel->Get_BoneIndex("backpack"), true)->Get_KeyFrame(0);

	_matrix mBody = XMMatrixAffineTransformation(
		XMLoadFloat4(&tKeyFrameBody.vScale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&tKeyFrameBody.vRotation),
		XMLoadFloat4(&tKeyFrameBody.vTranslation));
	_matrix mBackpack = XMMatrixAffineTransformation(
		XMLoadFloat4(&tKeyFrameBackpack.vScale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&tKeyFrameBackpack.vRotation),
		XMLoadFloat4(&tKeyFrameBackpack.vTranslation));

	m_iBackPackBoneIndex	= m_pModel->Get_BoneIndex("backpack");
	m_mAimPivot				= XMMatrixInverse(nullptr, mBody) * mBackpack;
	m_mNonAimPivot			= XMMatrixInverse(nullptr, mBackpack) * mBody;

	return S_OK;
}

void CRailGunner_State::Tick(_float _fTimeDelta)
{
	Handle_Backpack();
	Handle_Aim();
	Handle_State(_fTimeDelta);
	Handle_Skill(_fTimeDelta);

	m_bitPrevState = m_bitState;
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

void CRailGunner_State::Handle_Aim()
{
	if (m_pAnimator->Is_Playing(ANIMATION::RAILGUNNER::SPRINT_FORWARD)
	||	m_pAnimator->Is_Playing(ANIMATION::RAILGUNNER::SPRINT_LEFT)
	||	m_pAnimator->Is_Playing(ANIMATION::RAILGUNNER::SPRINT_RIGHT))
	{
		m_pModel->Set_BonePivot(m_iBackPackBoneIndex, m_mNonAimPivot);
		return;
	}

	m_pModel->Set_BonePivot(m_iBackPackBoneIndex, m_mAimPivot);

	_float4	vPipeLineLook	= CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::LOOK).normalize();
	_float3	vPlayerLook		= m_pTransform->Get_State(TRANSFORM::LOOK).normalize();
	_float	fPitch			= atan2f(vPipeLineLook.y, sqrtf(powf(vPipeLineLook.x, 2) + powf(vPipeLineLook.z, 2)));
	_float	fYaw			= (XMVectorGetY(XMVector3Cross(XMVectorSet(vPipeLineLook.x, 0.f, vPipeLineLook.z, 0.f), XMVectorSet(vPlayerLook.x, 0.f, vPlayerLook.z, 0.f))) < 0 ? 1.f : -1.f)
							* (1.f - XMVectorGetY(XMVector3Dot(
								XMVector2Normalize(XMVectorSet(vPipeLineLook.x, vPipeLineLook.z, 0.f, 0.f)),
								XMVector2Normalize(XMVectorSet(vPlayerLook.x, vPlayerLook.z, 0.f, 0.f)))));

	m_pAnimator->Blend_Animation(ANIMATION::RAILGUNNER::AIM_PITCH,
		Function::Clamp(0.f, 1.f, Function::ProportionalRatio(XMConvertToRadians(MAINCAM_PITCH_MIN * 0.5f), XMConvertToRadians(MAINCAM_PITCH_MAX * 0.5f), fPitch)));
	m_pAnimator->Blend_Animation(ANIMATION::RAILGUNNER::AIM_YAW,
		Function::Clamp(0.f, 1.f, Function::ProportionalRatio(XMConvertToRadians(MAINCAM_YAW_MIN * 0.25f), XMConvertToRadians(MAINCAM_YAW_MAX * 0.25f), fYaw)));
}

void CRailGunner_State::Handle_State(_float _fTimeDelta)
{
	shared_ptr<CGameInstance>	pGameInstance	= CGameInstance::Get_Instance();
	shared_ptr<CRailGunner>		pRailGunner		= m_pRailGunner.lock();

#pragma region Air
	if (m_bitState.test(IDX(RG_STATE::AIR)))
	{
		m_fAirTimeAcc += _fTimeDelta;

		if (!m_pAnimator->Is_Playing(ANIMATION::RAILGUNNER::JUMP_START))
		{
			if (m_pPhysics->Get_Velocity().y >= 0.f)
			{
				m_pAnimator->Play_Animation(ANIMATION::RAILGUNNER::AIR_LOOP_UP, 1.f, false, 0.25f);
			}
			else if (0.35f < m_fAirTimeAcc)
			{
				m_pAnimator->Play_Animation(ANIMATION::RAILGUNNER::AIR_LOOP_DOWN, 1.f, false, 0.25f);
			}
		}
	}
	else
	{
		m_fAirTimeAcc = 0.f;
	}

	m_bitState.set(IDX(RG_STATE::AIR), !m_pGrounding->Is_Grounding());
#pragma endregion
#pragma region Sprint
	if (m_bitState.test(IDX(RG_STATE::SPRINT)))
	{
		pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::SPRINT);

		if (pGameInstance->Key_Up(CONTROL_FORWARD))
		{
			pRailGunner->Set_State(RG_STATE::SPRINT, false);
			pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::MAIN);
		}

		if (pGameInstance->Key_Down(CONTROL_ATTACK1))
		{
			pRailGunner->Set_State(RG_STATE::SPRINT, false);
			pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::MAIN);
		}

		if (pGameInstance->Key_Down(CONTROL_ATTACK2))
		{
			pRailGunner->Set_State(RG_STATE::SPRINT, false);
			pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::SCOPE);
		}
	}
#pragma endregion
#pragma region FOV
	if (m_bitPrevState.test(IDX(RG_STATE::SPRINT)) != m_bitState.test(IDX(RG_STATE::SPRINT)))
	{
		if (m_bitState.test(IDX(RG_STATE::SPRINT)))
		{
			CPipeLine::Get_Instance()->Get_Camera<CCamera_Main>()->Adjust_FOV(XMConvertToRadians(65.f), 0.5f, 0.5f);
		}
		else
		{
			CPipeLine::Get_Instance()->Get_Camera<CCamera_Main>()->Release_FOV(0.25f, 0.5f);
		}
	}
#pragma endregion
}

void CRailGunner_State::Handle_Skill(_float _fTimeDelta)
{
	shared_ptr<CRailGunner>		pRailGunner		= m_pRailGunner.lock();

	if (pRailGunner->Is_Crosshair(RG_CROSSHAIR::MAIN))
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

void CRailGunner_State::Handle_Backpack()
{
	m_pAnimator->Play_Animation(ANIMATION::RAILGUNNER::BACKPACK_CHARGED);
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
