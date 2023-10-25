#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBrotherUI_HealthBar final : public CGameObject
{
private:
	explicit CBrotherUI_HealthBar(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CBrotherUI_HealthBar() DEFAULT;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any pBrotherEntityDesc) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	virtual HRESULT								Ready_Components() override;

private:
	shared_ptr<CTransform>						m_pBackground;
	shared_ptr<CTransform>						m_pHealth;

	shared_ptr<CTexture>						m_pTexHealth;

	shared_ptr<CShader>							m_pShader;
	shared_ptr<CVIBuffer_Rect>					m_pVIBuffer;

	const ENTITYDESC*							m_pEntityDesc	= nullptr;

public:
	static shared_ptr<CBrotherUI_HealthBar>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any pBrotherEntityDesc) override;
};

END
