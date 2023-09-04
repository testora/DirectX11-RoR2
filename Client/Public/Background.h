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

class CBackground final : public CGameObject
{
private:
	explicit CBackground(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CBackground(const CBackground&);
	virtual ~CBackground() DEFAULT;

public:
	virtual HRESULT					Initialize(any = any()) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	virtual HRESULT					Ready_Components() override;

private:
	shared_ptr<CRenderer>			m_pRenderer;
	shared_ptr<CTransform>			m_pTransform;
	shared_ptr<CShader>				m_pShader;
	shared_ptr<CTexture>			m_pTexture;
	shared_ptr<CVIBuffer_Rect>		m_pVIBuffer;

public:
	static shared_ptr<CBackground>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = any()) override;
};

END
