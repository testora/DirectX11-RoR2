#pragma once
#include "Client_Define.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

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
	virtual HRESULT					Initialize(any aDesc = g_aNull) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	HRESULT							Attach(shared_ptr<CTransform>, _float4 vOffset = _float4());

	void							Rebound_Pistol();

	void							Adjust_FOV(_float fRadian, _float fDuration = 0.f, _float fWeight = 1.f);
	void							Release_FOV(_float fDuration = 0.f, _float fWeight = 1.f);

private:
	void							Handle_MouseInput(_float fTimeDelta);

	void							Smooth_Tranformation(_float fTimeDelta);

private:
	CAMERA_MAIN_DESC				m_tCameraMainDesc{};

	shared_ptr<CTransform>			m_pTargetTransform;

	_float4							m_vMainOffset;
	_float3							m_vShakeAxis;

	_float4							m_vPistolReboundQuaternion;

public:
	static shared_ptr<CCamera_Main>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = g_aNull) override;
};

END
