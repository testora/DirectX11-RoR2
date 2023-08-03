#pragma once
#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	enum class	TYPE	{ PERSPECTIVE, ORTHOGONAL, MAX };

	typedef struct tagCameraDesc
	{
		TYPE			eType;
		_float3			vEye, vAt;
		_float			fFovAngleY, fAspect;
		_float			fWidth, fHeight;
		_float			fNear, fFar;
	} CAMERA_DESC;

protected:
	explicit CCamera(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCamera() DEFAULT;

public:
	virtual HRESULT						Initialize(any = any()) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	virtual HRESULT						Ready_Components() override;

protected:
	shared_ptr<class CTransform>		m_pTransformCom;
	shared_ptr<class CRenderer>			m_pRendererCom;

private:
	CAMERA_DESC							m_tCameraDesc;

	_float4x4							m_mView, m_mProj;

	shared_ptr<class CPipeLine>			m_pPipeLine;

public:
	virtual shared_ptr<CGameObject>		Clone(any = any()) override	PURE;
};

END
