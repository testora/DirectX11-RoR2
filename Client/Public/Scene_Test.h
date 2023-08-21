#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Client)

class CScene_Test final : public CScene
{
private:
	explicit CScene_Test(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Test() DEFAULT;

public:
	virtual HRESULT					Initialize() override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	HRESULT							Ready_Light();
	HRESULT							Ready_Camera();
	HRESULT							Ready_Terrain();
	HRESULT							Ready_Player();

public:
	static shared_ptr<CScene_Test>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END
