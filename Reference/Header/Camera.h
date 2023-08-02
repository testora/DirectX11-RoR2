#pragma once
#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3	vEye, vAt;
		_float	fFovAngleY, fAspect, fNear, fFar;
	} CAMERA_DESC;

private:
	explicit CCamera(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCamera(const CCamera&);
	virtual ~CCamera() DEFAULT;

public:
	virtual HRESULT						Initialize(std::any = nullptr) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

protected:
	virtual HRESULT						Ready_Components(bitset<IDX(COMPONENT::MAX)> bitFlag) override;
	virtual HRESULT						Ready_Behaviors(bitset<IDX(BEHAVIOR::MAX)> bitFlag) override;

protected:
	void								Set_Transform();
	
protected:
	shared_ptr<class CTransform>		m_pTransformCom;

private:
	shared_ptr<class CPipeLine>			m_pPipeLine;

	CAMERA_DESC							m_tCameraDesc;

public:
	virtual shared_ptr<CGameObject>		Clone(std::any = nullptr) override		PURE;
};

END
