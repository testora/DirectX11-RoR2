#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Terrain;
END

BEGIN(Engine)

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTerrain(const CTerrain&);
	virtual ~CTerrain() DEFAULT;

public:
	virtual HRESULT					Initialize(any = any()) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render(_uint iPassIndex) override;

private:
	virtual HRESULT					Ready_Components() override;

private:
	shared_ptr<CRenderer>			m_pRenderer;
	shared_ptr<CTransform>			m_pTransform;
	shared_ptr<CShader>				m_pShader;
	shared_ptr<CTexture>			m_pTexture;
	shared_ptr<CVIBuffer_Terrain>	m_pVIBuffer;

public:
	static shared_ptr<CTerrain>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = any()) override;
};

END
