#pragma once
#include "Client_Define.h"
#include "Scene.h"

#if ACTIVATE_TOOL

BEGIN(Engine)
class CRenderer;
class CTransform;
class CShader;
class CTexture;
class CModel;
class CAnimation;
class CMesh;
class CVIBufferInstance_Mesh;
class CVIBufferInstance_Rect;
END

BEGIN(Client)

class CVFX_ParticleMesh;

enum class TOOL
{
	MODEL,
	EFFECT,
	MAX
};
enum class FX_AREA
{
	INSTANCE_MESH,
	PARTICLE_MESH,
	MAX
};

typedef struct tagVIBufferInstance
{
	string								strPath;
	_int								iMaxInstance	= 0;
	shared_ptr<CVIBufferInstance_Mesh>	pVIBuffer;
} VIINST;

class CScene_Tool final : public CScene
{
private:
	explicit CScene_Tool(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Tool() DEFAULT;

public:
	virtual HRESULT										Initialize() override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;
	virtual void										Debug() override;

private:
	void												System_Model();
	void												System_Effect();
	void												System_Option();
	void												Info_Model();
	void												Info_Effect();
	void												Control_Model();
	void												Control_Effect();
	void												SubControl_Effect();

	HRESULT												Load_BinaryMeshInstanceList(const wstring& wstrFilePath, const wstring& wstrFileName);
	HRESULT												Load_Model(const wstring& wstrFilePath, const wstring& wstrFileName, const MODEL, _matrixf mPivot);
	HRESULT												Load_ParticleMesh(const wstring& wstrFilePath, const wstring& wstrFileName);

	HRESULT												Export_BinaryMeshInstanceList(const wstring& wstrPath);
	HRESULT												Export_BinaryModel(const wstring& wstrPath);
	HRESULT												Export_BinaryParticleMesh(const wstring& wstrPath);
	HRESULT												Export_BinaryPoints(const wstring& wstrPath);

private:
	ImGuiFileDialog										m_imEmbed_Open;
	ImGuiFileDialog										m_imEmbed_Export;

	shared_ptr<class CCamera_Main>						m_pCamera;

	shared_ptr<CTransform>								m_pTransform;
	shared_ptr<class CGlobalGizmo>						m_pGlobalGizmo;

	map<string, shared_ptr<CModel>>						m_mapAnimModels;
	map<string, shared_ptr<CModel>>						m_mapNonAnimModels;

	map<string, VIINST>									m_mapExportMeshInstance;
	string												m_strSelectedExportMeshInstance;

	map<string, shared_ptr<CVFX_ParticleMesh>>			m_mapParticleMesh;

	wstring												m_wstrModelPath;
	wstring												m_wstrMeshPath;

	pair<string, shared_ptr<CModel>>					m_pairSelectedModel;
	pair<string, shared_ptr<CMesh>>						m_pairSelectedMesh;
	pair<string, shared_ptr<CVFX_ParticleMesh>>			m_pairSelectedParticleMesh;
	shared_ptr<CAnimation>								m_pSelectedAnimation;
	MATERIAL											m_tSelectedMaterial;

	shared_ptr<CRenderer>								m_pRenderer;
	shared_ptr<CShader>									m_pShader_NonAnimMesh;
	shared_ptr<CShader>									m_pShader_AnimMesh;
	shared_ptr<CShader>									m_pShader_InstMesh;

	vector<_float3>										m_vecPoint;

	TOOL												m_eTool	= TOOL::MAX;

public:
	static shared_ptr<CScene_Tool>						Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END

#endif
