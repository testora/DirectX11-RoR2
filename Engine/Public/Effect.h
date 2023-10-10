#pragma once
#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CEffect abstract : public CGameObject
{
protected:
	explicit CEffect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffect(const CEffect&);
	virtual ~CEffect() DEFAULT;
	
public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override												PURE;

	virtual HRESULT							Fetch(any = g_aNull) override;

protected:
	virtual HRESULT							Ready_Components() override;

	virtual HRESULT							Render(_uint iPassIndex) override;

	virtual void							Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull)	PURE;
	virtual	void							Update_Instance(void*, _uint iNumInstance, _float fTimeDelta)	PURE;

	virtual HRESULT							Add_Component(const COMPONENT) override;

protected:
	shared_ptr<class CTransform>			m_pTransform;
	shared_ptr<class CShader>				m_pShader;

private:
	shared_ptr<class CVIBufferInstance>		m_pVIBufferInstance;
};

END
