#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Client)

class CScene_Stage1 final : public CScene
{
private:
	explicit CScene_Stage1(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Stage1() DEFAULT;

public:
	virtual HRESULT						Initialize() override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	static shared_ptr<CScene_Stage1>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END
