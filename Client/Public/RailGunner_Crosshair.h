#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CRailGunner_Crosshair final : public CGameObject
{
private:
	enum class ELEMENT	{ BOUND_IN, BOUND_OUT, BRACKET, NIBS, FLAVOR, SCOPE, CROSS, BOUND_GLOWY_IN, BOUND_GLOWY_OUT, DOT, ARROW, MAX };
	enum class STATE	{ MAIN, SNIPER, SUPER_CHARGE, SUPER_READY, SUPER_REBOOT, SPRINT, MAX };

private:
	explicit CRailGunner_Crosshair(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRailGunner_Crosshair(const CRailGunner_Crosshair&) DEFAULT;
	virtual ~CRailGunner_Crosshair() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

public:
	void										Change_State(const STATE);

	void										Init_Bound();
	void										Init_Flavor();
	void										Init_Bracket();
	void										Init_Scope();
	void										Init_Bound_Glowy();
	void										Init_Arrow();

private:
	virtual HRESULT								Ready_Components() override;

private:
	STATE										m_eState				= STATE::MAIN;
	_color										m_vDiffuse[IDX(ELEMENT::MAX)];

	_float										m_fFlavorRotateSpeed	= 0.f;

	bitset<IDX(ELEMENT::MAX)>					m_bitElement;

	shared_ptr<CTransform>						m_pTransform[IDX(ELEMENT::MAX)];
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer;

	shared_ptr<CTexture>						m_pTexBounds;
	shared_ptr<CTexture>						m_pTexBracket;
	shared_ptr<CTexture>						m_pTexFlavor;
	shared_ptr<CTexture>						m_pTexScope;	
	shared_ptr<CTexture>						m_pTexBounds_Glowy;

	shared_ptr<CTexture>						m_pTexDot;
	shared_ptr<CTexture>						m_pTexArrow;

	shared_ptr<CRenderer>						m_pRenderer;
	shared_ptr<CShader>							m_pShader;

public:
	static shared_ptr<CRailGunner_Crosshair>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
