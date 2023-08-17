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

class CRailGunner final : public CGameObject, public std::enable_shared_from_this<CRailGunner>
{
private:
	explicit CRailGunner(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRailGunner(const CRailGunner&);
	virtual ~CRailGunner() DEFAULT;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(any = any()) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	virtual HRESULT					Ready_Components() override;

public:
	shared_ptr<CRenderer>			m_pRendererCom;
	shared_ptr<CTransform>			m_pTransformCom;
	shared_ptr<CShader>				m_pShaderCom;
	shared_ptr<CModel>				m_pModelCom;

public:
	static shared_ptr<CRailGunner>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = any()) override;
};

END
