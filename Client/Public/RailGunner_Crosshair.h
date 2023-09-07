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
	enum class STATE	{ MAIN, SCOPE, SUPER_CHARGE, SUPER_READY, SUPER_REBOOT, RELOAD, SPRINT, MAX };
	enum class ELEMENT
	{
		MAIN_BOUND_IN, MAIN_BOUND_OUT, MAIN_BRACKET, MAIN_NIBS, MAIN_FLAVOR,
		SCOPE_SCOPE, SCOPE_CROSS, SCOPE_BOUND_IN, SCOPE_BOUND_OUT,
		RELOAD_BOUND, RELOAD_STRIPE, RELOAD_BAR, RELOAD_SLIDE, RELOAD_TAG, RELOAD_BOOST,
		DOT,
		ARROW,
		MAX
	};

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

private:
	virtual HRESULT								Ready_Components() override;

	HRESULT										Render_Element(const ELEMENT, _uint iPassIndex);

	void										Visualize_Main();
	void										Visualize_Bracket();
	void										Visualize_Scope();
	void										Visualize_Reload();
	void										Visualize_Sprint();

	void										Bounce_Bracket();

private:
	STATE										m_eState				= STATE::MAIN;
	_color										m_vDiffuse[IDX(ELEMENT::MAX)];

	_float										m_fFlavorRotateSpeed	= 0.f;

	bitset<IDX(ELEMENT::MAX)>					m_bitElement;

	pair<_float, _float>						m_pairTagPositionRange;
	_float										m_fCurrentTagPosition	= 0.f;
	_bool										m_bHitTag				= false;

	shared_ptr<CTransform>						m_pTransform[IDX(ELEMENT::MAX)];
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer;

	shared_ptr<CTexture>						m_pTexSinglePixel;

	shared_ptr<CTexture>						m_pTexBounds;
	shared_ptr<CTexture>						m_pTexBracket;
	shared_ptr<CTexture>						m_pTexFlavor;
	
	shared_ptr<CTexture>						m_pTexScope;	
	shared_ptr<CTexture>						m_pTexBounds_Glowy;
	shared_ptr<CTexture>						m_pTexSniperCharge;

	shared_ptr<CTexture>						m_pTexStripes;
	shared_ptr<CTexture>						m_pTexBoost;

	shared_ptr<CTexture>						m_pTexDot;
	shared_ptr<CTexture>						m_pTexArrow;

	shared_ptr<CRenderer>						m_pRenderer;
	shared_ptr<CShader>							m_pShader;

public:
	static shared_ptr<CRailGunner_Crosshair>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
