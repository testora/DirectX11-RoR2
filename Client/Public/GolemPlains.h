#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CShader;
class CModel;
END

BEGIN(Client)

class CGolemPlains final : public CGameObject
{
private:
	explicit CGolemPlains(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGolemPlains(const CGolemPlains&);
	virtual ~CGolemPlains() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_ShadowDepth() override;

private:
	virtual HRESULT						Ready_Components() override;

public:
	shared_ptr<CRenderer>				m_pRenderer;
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

public:
	static shared_ptr<CGolemPlains>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
