#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CObjectPool;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CRailGunner final : public CGameObject
{
public:
	enum class SYSTEM	{ STATE, CROSSHAIR, MAX };

private:
	explicit CRailGunner(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRailGunner(const CRailGunner&);
	virtual ~CRailGunner() DEFAULT;

public:
	virtual HRESULT												Initialize_Prototype() override;
	virtual HRESULT												Initialize(any = g_aNull) override;
	virtual void												Tick(_float fTimeDelta) override;
	virtual void												Late_Tick(_float fTimeDelta) override;
	virtual HRESULT												Render() override;

	virtual HRESULT												Render_ShadowDepth() override;

	virtual void												OnCollisionEnter(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta) override;
	virtual void												OnCollision(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta) override;
	virtual void												OnCollisionExit(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta) override;

private:
	virtual HRESULT												Ready_Components() override;
	virtual HRESULT												Ready_Behaviors() override;
	HRESULT														Ready_RailGunner();
	HRESULT														Ready_Bullets();

public:
#pragma region RailGunner State
	_bool														Is_State(bitset<IDX(RG_STATE::MAX)> bit);
	bitset<IDX(RG_STATE::MAX)>									Get_State();
	void														Set_State(RG_STATE, _bool = true);
#pragma endregion
#pragma region RailGunner Crosshair
	_bool														Is_Crosshair(RG_CROSSHAIR);
	_bool														Is_SuccessReload();
	RG_CROSSHAIR												Get_Crosshair();

	void														Visualize_Crosshair(RG_CROSSHAIR);
	void														Bounce_Bracket();
	void														Hit_Reload();
	void														Fire_Sniper();
	void														Fire_Pistol();
#pragma endregion

private:
	unordered_map<SYSTEM, shared_ptr<class Engine::ISystem>>	m_umapSystem;

	shared_ptr<class CObjectPool>								m_pPistolBulletPool;
	_float4x4													m_mPistolOffset	= g_mUnit;

	shared_ptr<CRenderer>										m_pRenderer;
	shared_ptr<CTransform>										m_pTransform;
	shared_ptr<CShader>											m_pShader;
	shared_ptr<CModel>											m_pModel;

public:
	static shared_ptr<CRailGunner>								Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>								Clone(any = g_aNull) override;
};

END
