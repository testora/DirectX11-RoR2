#include "EnginePCH.h"
#include "Camera.h"
#include "PipeLine.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"

CCamera::CCamera(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject	(_pDevice, _pContext)
	, m_pPipeLine	(CPipeLine::Get_Instance())
	, m_mView		(g_mUnit)
	, m_mProj		(g_mUnit)
{
}

HRESULT CCamera::Initialize(any _arg)
{
	auto arg = any_cast<CAMERA_DESC>(_arg);

	m_tCameraDesc = arg;

	m_bitComponent |= BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::RENDERER);
	
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Failed to Initialize");
	}

	switch (m_tCameraDesc.eType)
	{
	case TYPE::PERSPECTIVE:
		m_mProj = XMMatrixPerspectiveFovLH(m_tCameraDesc.fFovAngleY, m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar);
		break;
	}

	m_pTransform->Set_State(TRANSFORM::POSITION, m_tCameraDesc.vEye);
	m_pTransform->LookAt(m_tCameraDesc.vAt);

	return S_OK;
}

void CCamera::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CCamera::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CCamera::Render(_uint _iPassIndex)
{
	switch (m_tCameraDesc.eType)
	{
	case TYPE::PERSPECTIVE:
		m_pPipeLine->Set_Transform(PIPELINE::WORLD, m_pTransform->Get_Matrix());
		m_pPipeLine->Set_Transform(PIPELINE::VIEW, m_pTransform->Get_Matrix().inverse());
		m_pPipeLine->Set_Transform(PIPELINE::PROJECTION, m_mProj);
		break;
	}

	return S_OK;
}

HRESULT CCamera::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCamera::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pTransform");
	}

	m_pRenderer = dynamic_pointer_cast<CRenderer>(m_umapComponent[COMPONENT::RENDERER]);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pRenderer");
	}

	return S_OK;
}
