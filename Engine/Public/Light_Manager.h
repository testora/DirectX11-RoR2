#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CLight_Manager final : public CSingleton<CLight_Manager>
{
private:
	typedef vector<shared_ptr<class CLight>>												Lights;
	typedef map<shared_ptr<class CLight>, shared_ptr<class CGameObject>, std::owner_less<>>	ShadowObjects;
	typedef map<shared_ptr<class CGameObject>, shared_ptr<class CLight>, std::owner_less<>>	Shadows;

private:
	explicit CLight_Manager() DEFAULT;
	virtual ~CLight_Manager() DEFAULT;

public:
	HRESULT																	Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const GRAPHICDESC, const SCENE);

	void																	Tick();

public:
#ifdef _DEBUG
	ComPtr<ID3D11ShaderResourceView>										Get_Shadow(_uint iIndex) const	{ return m_vecShadowShaderResourceViews[iIndex]; }
#endif

public:

	HRESULT																	Add_Light(const SCENE, LIGHTDESC, shared_ptr<class CTransform>, shared_ptr<class CGameObject> = nullptr);
	HRESULT																	Clear_Lights(const SCENE);

	HRESULT																	Bind_Lights(shared_ptr<class CShader>, shared_ptr<class CVIBuffer_Rect>);
	
	HRESULT																	Push_ShadowResourceView(ComPtr<ID3D11ShaderResourceView>, shared_ptr<class CGameObject>);

	HRESULT																	Bind_ShadowViewMatrices(shared_ptr<class CShader>);

	HRESULT																	Render_ShadowDepth(shared_ptr<class CShader>, _uint iPassIndex, shared_ptr<class CVIBuffer>, shared_ptr<class CGameObject>);

private:
	unique_ptr<Lights[]>													m_arrLights; 
	_uint 																	m_iLightCount	= 0;

	Shadows																	m_mapShadows;
	ShadowObjects															m_mapShadowObjects;
	vector<_float4x4>														m_vecShadowViewMatrices;
	vector<_float4x4>														m_vecShadowProjectionMatrices;
	vector<ComPtr<ID3D11ShaderResourceView>>								m_vecShadowShaderResourceViews;
	map<shared_ptr<class CGameObject>, ComPtr<ID3D11ShaderResourceView>>	m_mapShadowShaderResourceViews;

	D3D11_TEXTURE2D_DESC													m_tTexture2DDesc{};

	SCENE																	m_eSceneMax		= static_cast<SCENE>(0);

	ComPtr<ID3D11Device>													m_pDevice;
	ComPtr<ID3D11DeviceContext>												m_pContext;

	friend CSingleton<CLight_Manager>;
};

END
