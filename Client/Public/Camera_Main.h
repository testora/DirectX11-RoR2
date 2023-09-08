#pragma once
#include "Client_Define.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Main final : public CCamera
{
public:
	typedef struct tagCameraMainDesc
	{
		CAMERA_DESC	tCameraDesc;
	} CAMERA_MAIN_DESC;

private:
	explicit CCamera_Main(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCamera_Main() DEFAULT;

public:
	virtual HRESULT					Initialize(any = g_aNull) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	HRESULT							Attach(shared_ptr<class CTransform>, _float4 vOffset = _float4());

	void							Rebound_Pistol();

	void							Adjust_FOV(_float fRadian, _float fDuration = 0.f, _float fWeight = 1.f);
	void							Release_FOV(_float fDuration = 0.f, _float fWeight = 1.f);

private:
	void							Handle_MouseInput(_float fTimeDelta);

	void							Debug_MouseControl(_float fTimeDelta);
	void							Debug_KeyControl(_float fTimeDelta);

	void							Smooth_Tranformation(_float fTimeDelta);

private:
	CAMERA_MAIN_DESC				m_tCameraMainDesc;

	_float4							m_vMainOffset;
	_float3							m_vShakeAxis;

	shared_ptr<class CTransform>	m_pTargetTransform;

public:
	static shared_ptr<CCamera_Main>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = g_aNull) override;
};

END
