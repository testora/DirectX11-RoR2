#include "EnginePCH.h"
#include "Camera.h"
#include "PipeLine.h"
#include "Transform.h"

CCamera::CCamera(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
	, m_pPipeLine(CPipeLine::Get_Instance())
{
}

CCamera::CCamera(const CCamera& _rhs)
	: CGameObject(_rhs)
	, m_pPipeLine		(_rhs.m_pPipeLine)
	, m_tCameraDesc		(_rhs.m_tCameraDesc)
	, m_pTransformCom	(_rhs.m_pTransformCom)
{
}

HRESULT CCamera::Initialize(std::any arg)
{
	if (FAILED(__super::Initialize(arg)))
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Failed to Initialize");
	}

	m_tCameraDesc = std::any_cast<CAMERA_DESC>(arg);

	m_pTransformCom = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransformCom)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pTransformCom");
	}

	m_pTransformCom->Set_State(CTransform::POSITION, m_tCameraDesc.vEye);
	m_pTransformCom->LookAt(m_tCameraDesc.vAt);

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Ready_Components(bitset<IDX(COMPONENT::MAX)> _bitFlag)
{
	return __super::Ready_Components(_bitFlag |= BIT(COMPONENT::TRANSFORM));
}

HRESULT CCamera::Ready_Behaviors(bitset<IDX(BEHAVIOR::MAX)> _bitFlag)
{
	return __super::Ready_Behaviors(_bitFlag);
}

void CCamera::Set_Transform()
{
	m_pPipeLine->Set_Transform(CPipeLine::WORLD, m_pTransformCom->Get_Matrix());
	m_pPipeLine->Set_Transform(CPipeLine::VIEW, XMMatrixInverse(nullptr, m_pTransformCom->Get_Matrix()));
	m_pPipeLine->Set_Transform(CPipeLine::PROJ, XMMatrixPerspectiveFovLH(m_tCameraDesc.fFovAngleY, m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar));
}

