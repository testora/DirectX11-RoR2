#pragma once
#include "Client_Define.h"
#include "Effect.h"

BEGIN(Client)

class CEffect_Demo_Rect final : public CEffect
{
private:
	explicit CEffect_Demo_Rect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffect_Demo_Rect(const CEffect_Demo_Rect&);
	virtual ~CEffect_Demo_Rect() DEFAULT;

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
	static shared_ptr<CEffect_Demo_Rect>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END
