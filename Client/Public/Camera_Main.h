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
	virtual HRESULT					Initialize(any = any()) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render(_uint) override;

public:
	HRESULT							Attach(shared_ptr<class CTransform>, _float4x4 vOffset = _float4x4());

private:
	void							Debug_MouseControl(_float fTimeDelta);
	void							Debug_KeyControl(_float fTimeDelta);

private:
	CAMERA_MAIN_DESC				m_tCameraMainDesc;

	shared_ptr<class CTransform>	m_pTargetTransform;
	_float4x4						m_vOffset;

public:
	static shared_ptr<CCamera_Main>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = any()) override;
};

END
