#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
END

BEGIN(Client)

class CSkyCube final : public CGameObject
{
private:
	explicit CSkyCube(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CSkyCube(const CSkyCube&);
	virtual ~CSkyCube() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CTexture>				m_pTexture;

public:
	static shared_ptr<CSkyCube>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
