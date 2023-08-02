#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Client)

class CScene_Load final : public CScene
{
private:
	explicit CScene_Load(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const SCENE eLoadScene);
	virtual ~CScene_Load() DEFAULT;

public:
	HRESULT							Initialize() override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	shared_ptr<class CLoader>		m_pLoader;

	const SCENE						m_eLoadScene;

public:
	static shared_ptr<CScene_Load>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const SCENE eLoadScene);
};

END
