#pragma once
#include "Client_Define.h"
#include "Effect.h"

BEGIN(Client)

class CEffect_Demo_Point final : public CEffect
{
private:
	explicit CEffect_Demo_Point(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffect_Demo_Point(const CEffect_Demo_Point&);
	virtual ~CEffect_Demo_Point() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

	virtual HRESULT							Fetch(any = g_aNull) override;
	virtual _bool							Return() override;

protected:
	virtual	void							Fetch_Instance(void*, _uint iNumInstance, any = g_aNull) override;
	virtual	void							Update_Instance(void*, _uint iNumInstance, _float fTimeDelta) override;

private:
	_float									m_fTimeAcc	= 0.f;

public:
	static shared_ptr<CEffect_Demo_Point>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END
