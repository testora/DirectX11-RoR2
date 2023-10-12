#pragma once
#include "Client_Define.h"
#include "Effect.h"

BEGIN(Client)

class CVFX_ParticleMesh final : public CEffect
{
private:
	explicit CVFX_ParticleMesh(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVFX_ParticleMesh(const CVFX_ParticleMesh&);
	virtual ~CVFX_ParticleMesh() DEFAULT;

public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any wstrPath = g_aNull) override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

	virtual HRESULT										Fetch(any = g_aNull) override;
	virtual _bool										Return() override;

protected:
	virtual	void										Fetch_Instance(void*, _uint iNumInstance, any = g_aNull) override;
	virtual	void										Update_Instance(void*, _uint iNumInstance, _float fTimeDelta) override;

public:
#if ACTIVATE_TOOL
	shared_ptr<CTexture>								Get_Texture(aiTextureType) const;
	wstring												Get_TexturePath(aiTextureType) const;

	HRESULT												Set_TexturePath(aiTextureType, const wstring& wstrPath);

	virtual HRESULT										Add_Component(const COMPONENT, shared_ptr<class CComponent>) override;
#endif

private:
	shared_ptr<CTexture>								m_pTexDiffuse;
	shared_ptr<CTexture>								m_pTexNormal;

private:
	wstring												m_wstrTexDiffusePath;
	wstring												m_wstrTexNormalPath;

public:
	static shared_ptr<CVFX_ParticleMesh>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	shared_ptr<CGameObject>								Clone(any wstrPath = g_aNull) override;

	HRESULT												Read(const wstring& wstrPath);
#if ACTIVATE_TOOL
	HRESULT												Export(const wstring& wstrPath);
#endif
};

END
