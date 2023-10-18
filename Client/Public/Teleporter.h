#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Client)

class CTeleporter final : public CGameObject
{
private:
	explicit CTeleporter(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTeleporter(const CTeleporter&);
	virtual ~CTeleporter() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any vPosition3) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	virtual HRESULT						Ready_Components() override;

public:
	static shared_ptr<CTeleporter>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any vPosition3) override;
};

END
