#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CRailGunner_Crosshair final : public Engine::ISystem
{
private:
	enum class ELEMENT
	{
		MAIN_BOUND_IN, MAIN_BOUND_OUT, MAIN_BRACKET, MAIN_NIBS, MAIN_FLAVOR,
		SCOPE_SCOPE, SCOPE_CROSS, SCOPE_BOUND_IN, SCOPE_BOUND_OUT,
		RELOAD_BOUND, RELOAD_STRIPE, RELOAD_BAR, RELOAD_SLIDE, RELOAD_TAG, RELOAD_BOOST,
		DOT,
		ARROW,
		WEAKPOINT_BOUND, WEAKPOINT_STRIPE,
		MAX
	};

private:
	explicit CRailGunner_Crosshair(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CRailGunner_Crosshair() DEFAULT;

public:
	HRESULT										Initialize(any = g_aNull);
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

public:
	RG_CROSSHAIR								Get_State() const			{ return m_eState; }
	_bool										Is_SuccessReload() const	{ return m_bSuccessReload; }

public:
	void										Visualize(const RG_CROSSHAIR);

	void										Bounce_Bracket();
	void										Hit_Reload();
	void										Fire_Sniper();

private:
	HRESULT										Ready_Components();
	HRESULT										Ready_Transforms();
	HRESULT										Ready_Diffuses();
	HRESULT										Ready_Textures();

	HRESULT										Render_Element(const ELEMENT, _uint iPassIndex);
	HRESULT										Render_WeakPoints();

	void										Visualize_Main();
	void										Visualize_Bracket();
	void										Visualize_Flavor();
	void										Visualize_Scope();	
	void										Visualize_Reload();
	void										Visualize_Sprint();
	void										Visualize_WeakPoint();

	void										Search_WeakPoints();

private:
	RG_CROSSHAIR								m_eState					= RG_CROSSHAIR::MAX;
	bitset<IDX(ELEMENT::MAX)>					m_bitElement;
	_color										m_vDiffuse[IDX(ELEMENT::MAX)];

	pair<_float, _float>						m_pairTagPositionRange;
	_float										m_fCurrentTagPosition		= 0.f;
	_bool										m_bSuccessReload			= false;
	_bool										m_bHitTag					= false;

	_float										m_fFlavorRotateSpeed		= 0.f;

	shared_ptr<CRenderer>						m_pRenderer;
	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer;

	shared_ptr<CTransform>						m_pTransform[IDX(ELEMENT::MAX)];

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

	list<_float4x4>								m_lstWeakPoints;
	list<shared_ptr<class CMonster>>			m_lstWeakPointMonsters;

	ComPtr<ID3D11Device>						m_pDevice;
	ComPtr<ID3D11DeviceContext>					m_pContext;

public:
	static shared_ptr<CRailGunner_Crosshair>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END
