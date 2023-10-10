#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Client)

class CScene_Moon final : public CScene
{
private:
	explicit CScene_Moon(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Moon() DEFAULT;

public:
	virtual HRESULT					Initialize() override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	HRESULT							Ready_Effect();
	HRESULT							Ready_Light();
	HRESULT							Ready_Camera();
	HRESULT							Ready_Terrain();
	HRESULT							Ready_Player();
	HRESULT							Ready_Monster();

public:
	static shared_ptr<CScene_Moon>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END
