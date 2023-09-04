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
	enum STATE	{ BOUND_IN, BOUND_OUT, MAX };

private:
	explicit CRailGunner_Crosshair(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CRailGunner_Crosshair() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

public:
	void										Init_Bound();
	void										Init_Bracket();

private:
	virtual HRESULT								Ready_Components() override;

private:
	shared_ptr<CTransform>						m_pTransform[MAX];
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer;

	shared_ptr<CTexture>						m_pTexBounds;
	shared_ptr<CTexture>						m_pTexBracket;

	shared_ptr<CRenderer>						m_pRenderer;
	shared_ptr<CShader>							m_pShader;

public:
	static shared_ptr<CRailGunner_Crosshair>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;
};

END
