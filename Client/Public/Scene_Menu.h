#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Client)

class CScene_Menu final : public CScene
{
private:
	explicit CScene_Menu(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Menu() DEFAULT;

public:
	virtual HRESULT						Initialize() override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	HRESULT								Ready_Background();

public:
	static shared_ptr<CScene_Menu>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END
