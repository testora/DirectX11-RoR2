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
#ifdef _DEBUG
	virtual void					Debug() override;
#endif

private:
	HRESULT							Ready_SpawnPoint(const wstring& wstrPath);
	HRESULT							Ready_Collision();
	HRESULT							Ready_Light();
	HRESULT							Ready_Camera();
	HRESULT							Ready_Background();
	HRESULT							Ready_Terrain();
	HRESULT							Ready_Player();
	HRESULT							Ready_Monster();
	HRESULT							Ready_Interactable();
	HRESULT							Ready_Effect();

private:
	vector<_float3>					m_vecSpawnPoint;

public:
	static shared_ptr<CScene_Test>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END
