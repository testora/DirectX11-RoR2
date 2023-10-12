#pragma once
#include "Client_Define.h"
#include "Effect.h"

BEGIN(Client)

class CVFX_ParticleMesh final : public CEffect
{
public:
	enum class TYPE
	{
		BOUNCE,
		MAX
	};

	typedef struct tagBounceDesc
	{
		_float4x4	mInitialTransformation	= g_mUnit;
		_float4x4	mPeakTransformation		= g_mUnit;
		_float4x4	mFinalTransformation	= g_mUnit;

		_float		fBeginDuration			= 1.f;
		_float		fPeakDuration			= 1.f;
		_float		fEndDuration			= 1.f;
		_float		fBeginWeight			= 1.f;
		_float		fEndWeight				= 1.f;
	} BOUNCEDESC;

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
	BOUNCEDESC											Get_BounceDesc(_uint iIndex) const							{ return m_vecBounceDesc[iIndex]; }
	_float												Get_TotalBounceTime() const									{ return m_fTotalBounceTime; };

	void												Set_Type(TYPE eType)										{ m_eType = eType; }
	HRESULT												Set_TexturePath(aiTextureType, const wstring& wstrPath);
	void												Set_BounceDesc(_uint iIndex, const BOUNCEDESC& tBounceDesc)	{ m_vecBounceDesc[iIndex] = tBounceDesc; }
	void												Set_TimeLock(_bool bTimeLock = true)						{ m_bTimeLock = bTimeLock; }
	void												Set_Time(_float fTime)										{ m_fTimeAcc = fTime; }

	virtual void										Set_ActivateInstances(_uint iNumInstance) override;

	virtual HRESULT										Add_Component(const COMPONENT, shared_ptr<class CComponent>) override;

	void												Update();
	void												Update(_float fTrack);
#endif

private:
	_float												m_fTimeAcc			= 0.f;
	
	shared_ptr<CTexture>								m_pTexDiffuse;
	shared_ptr<CTexture>								m_pTexNormal;

	_float												m_fTotalBounceTime	= 0.f;
	_bool												m_bTimeLock			= false;

private:
	TYPE												m_eType				= TYPE::BOUNCE;

	wstring												m_wstrTexDiffusePath;
	wstring												m_wstrTexNormalPath;

	vector<BOUNCEDESC>									m_vecBounceDesc;

public:
	static shared_ptr<CVFX_ParticleMesh>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any wstrPath = g_aNull) override;

	HRESULT												Read(const wstring& wstrPath);
#if ACTIVATE_TOOL
	HRESULT												Export(const wstring& wstrPath);
#endif
};

END