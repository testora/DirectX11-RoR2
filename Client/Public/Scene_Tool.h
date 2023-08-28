#pragma once
#include "Client_Define.h"
#include "Scene.h"

#ifdef _DEBUG
#if ACTIVATE_IMGUI

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CScene_Tool final : public CScene
{
private:
	explicit CScene_Tool(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Tool() DEFAULT;

public:
	virtual HRESULT						Initialize() override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	void								System_Model();
	void								Info_Model();

	HRESULT								Load_Model(const wstring& strFilePath, const wstring& strFileName, const MODEL, _matrixf mPivot);
	HRESULT								Export_BinaryModel(const wstring& strFilePath, const wstring& strFileName);

private:
	ImGuiFileDialog						m_imEmbed_Open;
	ImGuiFileDialog						m_imEmbed_Export;

	wstring								m_strModelPath;

	map<string, shared_ptr<CModel>>		m_mapAnimModels;
	map<string, shared_ptr<CModel>>		m_mapNonAnimModels;
	pair<string, shared_ptr<CModel>>	m_pairSelectedModel;
	MATERIAL							m_tSelectedMaterial;

public:
	static shared_ptr<CScene_Tool>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END

#endif
#endif