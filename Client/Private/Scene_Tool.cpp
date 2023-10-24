#include "ClientPCH.h"
#include "Scene_Tool.h"
#include "Tool_Define.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Model.h"
#include "Bone.h"
#include "Animation.h"
#include "Channel.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera_Main.h"
#include "Mesh.h"
#include "VFX_ParticleMesh.h"
#include "VIBufferInstance_Mesh.h"
#include "GlobalGizmo.h"

#if ACTIVATE_TOOL

CScene_Tool::CScene_Tool(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::TOOL)
{
}

static shared_ptr<CMesh> pMesh;
static LIGHTDESC tLightDesc{};

HRESULT CScene_Tool::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to __super::Initialize");
	}

	m_pRenderer				= dynamic_pointer_cast<CRenderer>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Clone_Component: RENDERER");
	}

	m_pGlobalGizmo			= dynamic_pointer_cast<CGlobalGizmo>(CGlobalGizmo::Create(m_pDevice, m_pContext)->Clone());

	m_pShader_NonAnimMesh	= dynamic_pointer_cast<CShader>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXMESH));
	m_pShader_AnimMesh		= dynamic_pointer_cast<CShader>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM));
	m_pShader_InstMesh		= dynamic_pointer_cast<CShader>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESH));

	if (nullptr == m_pShader_NonAnimMesh
	||	nullptr == m_pShader_AnimMesh
	||	nullptr == m_pShader_InstMesh)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Clone_Component: SHADER");
	}

	CImGui_Manager::Get_Instance()->Enable();

	shared_ptr<CObjectLayer> pCamLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TOOL, LAYER_CAMERA);
	if (nullptr == pCamLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Add_Layer: LAYER_CAMERA");
	}
	
	if (FAILED(pCamLayer->Add(m_pCamera = dynamic_pointer_cast<CCamera_Main>(CCamera_Main::Create(m_pDevice, m_pContext)->Clone()))))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Add: CCamer_Main");
	}

	m_pCamera->Set_Debug();
	m_pCamera->Set_DebugSensitivity(_float2(0.1f, 0.1f));

	CGameInstance::Get_Instance()->Fix_Cursor(false);

	m_pTransform = CTransform::Create(m_pDevice, m_pContext);

	tLightDesc.eLightType	= LIGHTTYPE::DIRECTIONAL;
	tLightDesc.vDirection	= _float3(-0.64f, -0.76f, -0.12f);
	tLightDesc.vDiffuse		= _color(0.6f, 0.6f, 0.6f, 1.f);
	tLightDesc.vSpecular	= _color(0.3f, 0.3f, 0.3f, 1.f);
	tLightDesc.vAmbient		= _color(0.5f, 0.5f, 0.5f, 1.f);

	if (FAILED(CGameInstance::Get_Instance()->Add_Light(SCENE::TOOL, tLightDesc, nullptr)))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Add_Light");
	}

	return S_OK;
}

static _bool bTriplanerPass(false);
static _bool bTickParticle(false);
static _bool bGlobalGizmo(false);
static _int2 vGismoSize(10, 10);

void CScene_Tool::Tick(_float _fTimeDelta)
{
	if (bGlobalGizmo)
	{
		m_pGlobalGizmo->Tick(_fTimeDelta);
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::MenuItem("Edit Model"))
		{
			m_eTool = TOOL::MODEL;
		}

		if (ImGui::MenuItem("Effect"))
		{
			m_eTool = TOOL::EFFECT;
		}

		if (ImGui::MenuItem("Hide"))
		{
			m_eTool = TOOL::MAX;
		}

		if (ImGui::BeginMenu("Triplaner"))
		{
			if (ImGui::MenuItem("Regular Shader"))
			{
				bTriplanerPass = false;
			}
			if (ImGui::MenuItem("TriPlaner Shader"))
			{
				bTriplanerPass = true;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	switch (m_eTool)
	{
	case Client::TOOL::MODEL:
	{
		System_Model();
		Info_Model();
		Control_Model();

		if (m_pairSelectedModel.second)
		{
			m_pairSelectedModel.second->Tick_Animation(_fTimeDelta);
		}
	}
	break;
	case Client::TOOL::EFFECT:
	{
		System_Effect();
		Info_Effect();
		Control_Effect();
		SubControl_Effect();

		if (m_pairSelectedParticleMesh.second)
		{
			if (bTickParticle)
			{
				m_pairSelectedParticleMesh.second->Tick(_fTimeDelta);
			}
		}
	}
	break;
	}
}

void CScene_Tool::Late_Tick(_float _fTimeDelta)
{
	if (bGlobalGizmo)
	{
		m_pGlobalGizmo->Late_Tick(_fTimeDelta);
	}

	switch (m_eTool)
	{
	case Client::TOOL::MODEL:
	{
		m_pRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, shared_from_this());
	}
	break;
	case Client::TOOL::EFFECT:
	{
		m_pRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, shared_from_this());
		if (m_pairSelectedParticleMesh.second)
		{
			m_pairSelectedParticleMesh.second->Late_Tick(_fTimeDelta);
		}
	}
	break;
	}
}

static MODEL	eType(MODEL::NONANIM);
static FX_AREA	eArea(FX_AREA::MAX);
static _bool	bRenderFullModel(true);
static _bool	bToolShader(false);

HRESULT Bind_EmptyMaterialDesc(shared_ptr<CShader> _pShader)
{
	MATERIALDESC tMtrlDesc;
	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLDIF, tMtrlDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "Bind_EmptyMaterialDesc", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLAMB, tMtrlDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "Bind_EmptyMaterialDesc", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLSPC, tMtrlDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "Bind_EmptyMaterialDesc", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_MTRLEMS, tMtrlDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "Bind_EmptyMaterialDesc", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_MTRLSHN, tMtrlDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "Bind_EmptyMaterialDesc", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	return S_OK;
}

HRESULT CScene_Tool::Render()
{
	switch (m_eTool)
	{
	case Client::TOOL::MODEL:
	{
		if (m_pairSelectedModel.second)
		{
			switch (eType)
			{
			case MODEL::ANIM:
				if (FAILED(Bind_EmptyMaterialDesc(m_pShader_AnimMesh)))
				{
					MSG_RETURN(E_FAIL, "CScene_Tool::Render", "Failed to Bind_EmptyMaterialDesc");
				}
				if (FAILED(m_pTransform->Bind_OnShader(m_pShader_AnimMesh)))
				{
					MSG_RETURN(E_FAIL, "CScene_Tool::Render", "Failed to CTransform::Bind_OnShader");
				}
				if (bRenderFullModel)
				{
					m_pairSelectedModel.second->Render(m_pShader_AnimMesh, 0);
				}
				else
				{
					m_pairSelectedMesh.second->Render(m_pShader_NonAnimMesh, 0);
				}
				break;
			case MODEL::NONANIM:
				if (FAILED(Bind_EmptyMaterialDesc(m_pShader_NonAnimMesh)))
				{
					MSG_RETURN(E_FAIL, "CScene_Tool::Render", "Failed to Bind_EmptyMaterialDesc");
				}
				if(FAILED(m_pTransform->Bind_OnShader(m_pShader_NonAnimMesh)))
				{
					MSG_RETURN(E_FAIL, "CScene_Tool::Render", "Failed to CTransform::Bind_OnShader");
				}
				if (bRenderFullModel)
				{
					m_pairSelectedModel.second->Render(m_pShader_NonAnimMesh, bTriplanerPass ? 1 : 0);
				}
				else
				{
					m_pairSelectedMesh.second->Render(m_pShader_NonAnimMesh, 0);
				}
				break;
			}
		}
	}
	break;
	case Client::TOOL::EFFECT:
	{
		switch (eArea)
		{
		case Client::FX_AREA::INSTANCE_MESH:
		{
			if (FAILED(Bind_EmptyMaterialDesc(m_pShader_InstMesh)))
			{
				MSG_RETURN(E_FAIL, "CScene_Tool::Render", "Failed to Bind_EmptyMaterialDesc");
			}
			if (!m_strSelectedExportMeshInstance.empty())
			{
				if (FAILED(m_pTransform->Bind_OnShader(m_pShader_InstMesh)))
				{
					MSG_RETURN(E_FAIL, "CScene_Tool::Render", "Failed to CTransform::Bind_OnShader");
				}
				if (FAILED(m_mapExportMeshInstance[m_strSelectedExportMeshInstance].pVIBuffer->Render(m_pShader_InstMesh, 0)))
				{
					MSG_RETURN(E_FAIL, "CScene_Tool::Render", "Failed to CMesh::Render");
				}
			}
		}
		break;
		case Client::FX_AREA::PARTICLE_MESH:
		{

		}
		break;
		}
	}
	break;
	}

	return S_OK;
}

void CScene_Tool::Debug()
{
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		ImGui::Begin("MATERIAL");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_DIFFUSE).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_AMBIENT).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_SPECULAR).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_EMISSIVE).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("NONBLEND");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_NORMAL).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_DEPTH).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("LIGHT");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_DIFFUSE).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_AMBIENT).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_SPECULAR).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_SHADOWDEPTH).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("MASK");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_PREPROCESS).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MASK).Get(), ImVec2(200, 200));
		ImGui::End();
	}
}

static _int iSelectedKeyFrame(0);

void CScene_Tool::System_Model()
{
	ImGui::SetNextWindowPos(ImVec2(0.f, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SetNextWindowSize(ImVec2(540.f, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing()));

	ImGui::Begin("System:Model", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

#pragma region Open Model
	
	if (ImGui::Button("Load"))
	{
		if (!m_imEmbed_Open.IsOpened())
		{
			if (m_imEmbed_Export.IsOpened())
			{
				m_imEmbed_Export.Close();
			}

			const _char* szFilters = "Models (*.fbx, *.mdl){.fbx,.mdl},FBX (*.fbx){.fbx},Binary (*.mdl){.mdl},All files{.*}";
			m_imEmbed_Open.OpenDialog(DIALOG_MODEL_LOAD, "Open Model", szFilters, "Bin/Resources/", 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType			|
				ImGuiFileDialogFlags_NoDialog				|
				ImGuiFileDialogFlags_DisableBookmarkMode	|
				ImGuiFileDialogFlags_ReadOnlyFileNameField);
		}
		else
		{
			m_imEmbed_Open.Close();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Export"))
	{
		if (!m_imEmbed_Export.IsOpened())
		{
			if (m_imEmbed_Open.IsOpened())
			{
				m_imEmbed_Open.Close();
			}

			const _char* szFilters = "Binary (*.mdl){.mdl},Models (*.fbx, *.mdl){.fbx,.mdl},All files{.*}";
			m_imEmbed_Export.OpenDialog(DIALOG_MODEL_EXPORT, "Export Model", szFilters, "Bin/Resources/", "",
				[](const char*, void*, bool*)
				{
					ImGui::Text("");
				},
				60, 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType			|
				ImGuiFileDialogFlags_NoDialog				|
				ImGuiFileDialogFlags_DisableBookmarkMode	|
				ImGuiFileDialogFlags_ConfirmOverwrite);
		}
		else
		{
			m_imEmbed_Export.Close();
		}
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Anim", eType == MODEL::ANIM))
	{
		eType = MODEL::ANIM;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("NonAnim", eType == MODEL::NONANIM))
	{
		eType = MODEL::NONANIM;
	}

	_float fWindowWidth = ImGui::GetContentRegionAvail().x;
	_float fButtonWidth = ImGui::CalcTextSize("Pivot").x + ImGui::GetStyle().FramePadding.x * 2.f;

	ImGui::SameLine(fWindowWidth - fButtonWidth + ImGui::GetStyle().FramePadding.x);

	static _bool		bShowPivotSettings = false;
	static _float3		vPivotScale(1.f, 1.f, 1.f);
	static _float3		vPivotRotation(0.f, 0.f, 0.f);
	static _float3		vPivotTranslation(0.f, 0.f, 0.f);
	static _float4x4	mPivot = g_mUnit;

	if (ImGui::Button("Pivot", ImVec2(fButtonWidth, 0)))
	{
		bShowPivotSettings = !bShowPivotSettings;
	}

	ImGui::Separator();

	static _uint iTab = 1;
	if (bShowPivotSettings)
	{
		if (ImGui::BeginTabBar("Tab:Pivot"))
		{
			if (ImGui::BeginTabItem("Value"))
			{
				if (ImGui::Button("Reset Pivot"))
				{
					vPivotScale			= _float3(1.f, 1.f, 1.f);
					vPivotRotation		= _float3(0.f, 0.f, 0.f);
					vPivotTranslation	= _float3(0.f, 0.f, 0.f);
				}

				if (iTab == 2)
				{
					iTab = 1;
					_vector vScale, vQuaternion, vTranslation;
					if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mPivot))
					{
						vPivotScale			= vScale;
						vPivotRotation		= Function::QuaternionToEuler(vQuaternion, false);
						vPivotTranslation	= vTranslation;
					}
					else
					{
						MSG_BOX("CScene_Tool:System_Model", "Failed to XMMatrixDecompose");
					}
				}
				ImGui::DragFloat3("Scale",				reinterpret_cast<_float*>(&vPivotScale));
				ImGui::DragFloat3("Rotation(Degree)",	reinterpret_cast<_float*>(&vPivotRotation));
				ImGui::DragFloat3("Translation",		reinterpret_cast<_float*>(&vPivotTranslation));
				ImGui::EndTabItem();

				mPivot = XMMatrixAffineTransformation(vPivotScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
					_float3(XMConvertToRadians(vPivotRotation.x), XMConvertToRadians(vPivotRotation.y), XMConvertToRadians(vPivotRotation.z))), vPivotTranslation);
			}

			if (ImGui::BeginTabItem("Matrix"))
			{
				if (ImGui::Button("Reset Pivot"))
				{
					vPivotScale			= _float3(1.f, 1.f, 1.f);
					vPivotRotation		= _float3(0.f, 0.f, 0.f);
					vPivotTranslation	= _float3(0.f, 0.f, 0.f);
					mPivot				= g_mUnit;
				}

				if (iTab == 1)
				{
					iTab	= 2;
					mPivot	= XMMatrixAffineTransformation(vPivotScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
						_float3(XMConvertToRadians(vPivotRotation.x), XMConvertToRadians(vPivotRotation.y), XMConvertToRadians(vPivotRotation.z))), vPivotTranslation);
				}
				ImGui::DragFloat4("Right",		reinterpret_cast<_float*>(&mPivot._11));
				ImGui::DragFloat4("Up",			reinterpret_cast<_float*>(&mPivot._21));
				ImGui::DragFloat4("Look",		reinterpret_cast<_float*>(&mPivot._31));
				ImGui::DragFloat4("Position",	reinterpret_cast<_float*>(&mPivot._41));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::Separator();
	}

	if (m_imEmbed_Open.Display(DIALOG_MODEL_LOAD, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imEmbed_Open.IsOk())
		{
			if (FAILED(Load_Model(Function::ToWString(m_imEmbed_Open.GetCurrentPath()), Function::ToWString(m_imEmbed_Open.GetCurrentFileName()), eType, mPivot)))
			{
				MSG_BOX("CScene_Tool::Tick", "Failed to Load_Model");
			}
		}
	}

	if (m_imEmbed_Export.Display(DIALOG_MODEL_EXPORT, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imEmbed_Export.IsOk())
		{
			if (FAILED(Export_BinaryModel(Function::ToWString(m_imEmbed_Export.GetFilePathName()))))
			{
				MSG_BOX("CScene_Tool::Tick", "Failed to Export_Model");
			}
		}
	}

	if (m_imEmbed_Open.IsOpened() || m_imEmbed_Export.IsOpened())
	{
		ImGui::Separator();
	}

#pragma endregion
#pragma region Model List

	ImGui::SeparatorText("Model List");

	_float fWindowWidth0 = ImGui::GetWindowWidth();
	_float fButtonWidth0 = ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemInnerSpacing.y;
	_float fButtonSpace0 = ImGui::GetStyle().ItemSpacing.x;

	ImGui::Text("Anim: ");
	ImGui::SameLine(fWindowWidth0 - fButtonSpace0 - fButtonWidth0);
	if (ImGui::Button("-##EraseFromAnim", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		m_mapAnimModels.erase(m_pairSelectedModel.first);
		m_pairSelectedModel.first.clear();
		m_pairSelectedModel.second = nullptr;
		iSelectedKeyFrame = -1;
	}
	if (ImGui::BeginListBox("Anim", ImVec2(-FLT_MIN, 0.f)))
	{
		for (auto pair : m_mapAnimModels)
		{
			if (ImGui::Selectable(pair.first.c_str(), m_pairSelectedModel == pair))
			{
				bRenderFullModel			= true;
				m_pairSelectedMesh.first.clear();
				m_pairSelectedMesh.second	= nullptr;
				m_pSelectedAnimation		= nullptr;
				m_pairSelectedModel			= pair;
				bToolShader					= !pair.second->Is_ManualShader();
			}

			if (m_pairSelectedModel == pair)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
	
	ImGui::Text("NonAnim: ");
	ImGui::SameLine(fWindowWidth0 - fButtonSpace0 - fButtonWidth0);
	if (ImGui::Button("-##EraseFromNonAnim", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		m_mapNonAnimModels.erase(m_pairSelectedModel.first);
		m_pairSelectedModel.first.clear();
		m_pairSelectedModel.second = nullptr;
	}
	if (ImGui::BeginListBox("NonAnim", ImVec2(-FLT_MIN, 0.f)))
	{
		for (auto pair : m_mapNonAnimModels)
		{
			if (ImGui::Selectable(pair.first.c_str(), m_pairSelectedModel == pair))
			{
				bRenderFullModel			= true;
				m_pairSelectedMesh.first.clear();
				m_pairSelectedMesh.second	= nullptr;
				m_pSelectedAnimation		= nullptr;
				m_pairSelectedModel			= pair;
				bToolShader					= !pair.second->Is_ManualShader();
			}

			if (m_pairSelectedModel == pair)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}

#pragma endregion

	System_Option();

	ImGui::End();
}

void CScene_Tool::System_Effect()
{
	ImGui::SetNextWindowPos(ImVec2(0.f, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SetNextWindowSize(ImVec2(540.f, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing()));

	ImGui::Begin("System:Effect", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

#pragma region FileDialog

	if (ImGui::Button("Load"))
	{
		if (!m_imEmbed_Open.IsOpened())
		{
			if (m_imEmbed_Export.IsOpened())
			{
				m_imEmbed_Export.Close();
			}

			const _char* szFilters = "Binary (*.dat, *.vfx,){.dat,.vfx},Instance (*.dat){.dat},Effects (*.vfx){.vfx},All files{.*}";
			m_imEmbed_Open.OpenDialog(DIALOG_EFFECT_LOAD, "Open Effect", szFilters, "Bin/Resources/", 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType			|
				ImGuiFileDialogFlags_NoDialog				|
				ImGuiFileDialogFlags_DisableBookmarkMode	|
				ImGuiFileDialogFlags_ReadOnlyFileNameField);
		}
		else
		{
			m_imEmbed_Open.Close();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Export"))
	{
		if (!m_imEmbed_Export.IsOpened())
		{
			if (m_imEmbed_Open.IsOpened())
			{
				m_imEmbed_Open.Close();
			}

			const _char* szFilters = "Binary (*.dat, *.vfx,){.dat,.vfx},Instance (*.dat){.dat},Effects (*.vfx){.vfx},All files{.*}";
			m_imEmbed_Export.OpenDialog(DIALOG_EFFECT_EXPORT, "Export Effect", szFilters, "Bin/Resources/", "",
				[](const char*, void*, bool*)
				{
					ImGui::Text("");
				},
				60, 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType			|
				ImGuiFileDialogFlags_NoDialog				|
				ImGuiFileDialogFlags_DisableBookmarkMode	|
				ImGuiFileDialogFlags_ConfirmOverwrite);
		}
		else
		{
			m_imEmbed_Export.Close();
		}
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(120.f);
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemInnerSpacing.x - 120.f);

	static const _char* szEffectType = "ParticleMesh";
	if (ImGui::BeginCombo("Options", szEffectType))
	{
		const _char* items[] = { "ParticleMesh", "Effect" };
		for (_uint i = 0; i < IM_ARRAYSIZE(items); i++)
		{
			_bool isSelected = (szEffectType == items[i]);

			if (ImGui::Selectable(items[i], isSelected))
			{
				szEffectType = items[i];
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (m_imEmbed_Open.Display(DIALOG_EFFECT_LOAD, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imEmbed_Open.IsOk())
		{
			string strExt;
			Function::SplitPath(m_imEmbed_Open.GetCurrentFileName(), nullptr, nullptr, nullptr, &strExt);

			if (".dat" == strExt)
			{
				if (FAILED(Load_BinaryMeshInstanceList(Function::ToWString(m_imEmbed_Open.GetCurrentPath()), Function::ToWString(m_imEmbed_Open.GetCurrentFileName()))))
				{
					MSG_BOX("CScene_Tool::System_Effect", "Failed to Load_BinaryMeshInstanceList");
				}
			}
			else if(".vfx" == strExt)
			{
				if (FAILED(Load_ParticleMesh(Function::ToWString(m_imEmbed_Open.GetCurrentPath()), Function::ToWString(m_imEmbed_Open.GetCurrentFileName()))))
				{
					MSG_BOX("CScene_Tool::System_Effect", "Failed to Load_ParticleMesh");
				}
			}
		}
	}

	if (m_imEmbed_Export.Display(DIALOG_EFFECT_EXPORT, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imEmbed_Export.IsOk())
		{
			switch (eArea)
			{
			case Client::FX_AREA::INSTANCE_MESH:
				if (FAILED(Export_BinaryMeshInstanceList(Function::ToWString(m_imEmbed_Export.GetFilePathName()))))
				{
					MSG_BOX("CScene_Tool::System_Effect", "Failed to Export_BinaryMeshInstanceList");
				}
				break;
			case Client::FX_AREA::PARTICLE_MESH:
				if (FAILED(Export_BinaryParticleMesh(Function::ToWString(m_imEmbed_Export.GetFilePathName()))))
				{
					MSG_BOX("CScene_Tool::System_Effect", "Failed to Export_BinaryParticleMesh");
				}
				break;
			}
		}
	}

	if (m_imEmbed_Open.IsOpened() || m_imEmbed_Export.IsOpened())
	{
		ImGui::Separator();
	}

#pragma endregion
#pragma region ListBox
#pragma region VIBufferInstance

	ImGui::SeparatorText("VIBufferInstance Mesh List");

	_float fWindowWidth0 = ImGui::GetWindowWidth();
	_float fButtonWidth0 = ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemInnerSpacing.y;
	_float fButtonSpace0 = ImGui::GetStyle().ItemSpacing.x;
	
	static _char szVIBufferInstanceKey[MAX_PATH];
	static _char szVIBufferInstancePath[MAX_PATH];
	static _int iMaxInstance(0);

	ImGui::Text("VIBufferInstance Mesh: ");
	ImGui::SameLine(fWindowWidth0 - fButtonSpace0 * 2.f - fButtonWidth0 * 2.f + ImGui::GetStyle().ItemInnerSpacing.y);
	if (ImGui::Button("+##CreateVIBufferInstance", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		ZeroMemory(szVIBufferInstanceKey, sizeof(_char) * MAX_PATH);
		ZeroMemory(szVIBufferInstancePath, sizeof(_char) * MAX_PATH);
		strcpy_s(szVIBufferInstanceKey, "Prototype:Component:VIBuffer:Instance:");
		strcpy_s(szVIBufferInstancePath, "Bin/Resources");
		iMaxInstance = 0;

		ImGui::OpenPopup("Push VIBufferInstance");
	}
	ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
	if (ImGui::Button("-##EraseFromVIBufferInstance", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		m_mapExportMeshInstance.erase(m_strSelectedExportMeshInstance);
		m_strSelectedExportMeshInstance.clear();
	}
	if (ImGui::BeginListBox("VIBufferInstance Mesh", ImVec2(-FLT_MIN, 0.f)))
	{
		for (auto pair : m_mapExportMeshInstance)
		{
			if (ImGui::Selectable(pair.first.c_str(), m_strSelectedExportMeshInstance == pair.first, ImGuiSelectableFlags_AllowDoubleClick))
			{
				eArea = FX_AREA::INSTANCE_MESH;

				m_strSelectedExportMeshInstance = pair.first;
			}

			if (m_strSelectedExportMeshInstance == pair.first)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}

#pragma endregion

	ImGui::SeparatorText("Effect List");

	static _bool bOpenNameModal = false;
	static _char szEffectName[MAX_PATH];

	ImGui::Text("ParticleMesh: ");
	ImGui::SameLine(fWindowWidth0 - fButtonSpace0 * 2.f - fButtonWidth0 * 2.f + ImGui::GetStyle().ItemInnerSpacing.y);
	if (ImGui::Button("+##CreateParticleMesh", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		ImGui::OpenPopup("Create ParticleMesh");
	}
	ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
	if (ImGui::Button("-##EraseFromParticleMesh", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		m_mapParticleMesh.erase(m_pairSelectedParticleMesh.first);
		m_pairSelectedParticleMesh.first.clear();
		m_pairSelectedParticleMesh.second = nullptr;
	}
	if (ImGui::BeginListBox("ParticleMesh", ImVec2(-FLT_MIN, 0.f)))
	{
		for (auto pair : m_mapParticleMesh)
		{
			if (ImGui::Selectable(pair.first.c_str(), m_pairSelectedParticleMesh == pair, ImGuiSelectableFlags_AllowDoubleClick))
			{
				eArea = FX_AREA::PARTICLE_MESH;

				m_pairSelectedParticleMesh = pair;
				strcpy_s(szEffectName, pair.first.c_str());

				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					bOpenNameModal = true;
				}
			}

			if (m_pairSelectedParticleMesh == pair)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}

	if (bOpenNameModal)
	{
		ImGui::OpenPopup("Change Effect Name");
		bOpenNameModal = false;
	}

#pragma region PopUp
	if (ImGui::BeginPopupModal("Push VIBufferInstance", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Key:");
		ImGui::InputText("##PushKey", szVIBufferInstanceKey, IM_ARRAYSIZE(szVIBufferInstanceKey));
		ImGui::Text("Path:");
		ImGui::InputText(".msh##PushPath", szVIBufferInstancePath, IM_ARRAYSIZE(szVIBufferInstancePath));
		ImGui::Text("Instance Number:");
		ImGui::InputInt("##PushMaxInstance", &iMaxInstance);
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120.f, 0)))
		{
			shared_ptr<CVIBufferInstance_Mesh> pInstance = dynamic_pointer_cast<CVIBufferInstance_Mesh>(
				CVIBufferInstance_Mesh::Create(m_pDevice, m_pContext, Function::ToWString(szVIBufferInstancePath + string(".msh")), iMaxInstance)->Clone());
			if (nullptr == pInstance)
			{
				MSG_BOX("CScene_Tool::System_Effect", "Failed to Create: CVIBufferInstance_Mesh");
			}
			else
			{
				m_mapExportMeshInstance.emplace(szVIBufferInstanceKey, VIINST{ szVIBufferInstancePath + string(".msh"), iMaxInstance, pInstance });

				CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TOOL, Function::ToWString(szVIBufferInstanceKey), pInstance);
			}

			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	static _char szParticleMeshName[MAX_PATH] = "Name";
	if (ImGui::BeginPopupModal("Create ParticleMesh", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Effect Name:");
		ImGui::InputText("Name", szParticleMeshName, IM_ARRAYSIZE(szParticleMeshName));
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120.f, 0)))
		{
			shared_ptr<CVFX_ParticleMesh> pInstance = dynamic_pointer_cast<CVFX_ParticleMesh>(
				CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TOOL, PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH));

			if (!m_strSelectedExportMeshInstance.empty())
			{
				if (FAILED(pInstance->Add_Component(COMPONENT::VIBUFFER_INSTANCE_MESH, m_mapExportMeshInstance[m_strSelectedExportMeshInstance].pVIBuffer)))
				{
					MSG_BOX("CScene_Tool::System_Effect", "Failed to Add_Component: VIBufferInstance_Mesh");
				}
				pInstance->Set_VIBufferInstanceMeshTag(Function::ToWString(m_strSelectedExportMeshInstance));
			}

			m_mapParticleMesh.emplace(szParticleMeshName, pInstance);

			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Change Effect Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Change Effect Name:");
		ImGui::InputText("Name", szEffectName, IM_ARRAYSIZE(szEffectName));
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120.f, 0)))
		{
			m_mapParticleMesh.erase(m_pairSelectedParticleMesh.first);
			m_pairSelectedParticleMesh.first = szEffectName;
			m_mapParticleMesh.emplace(m_pairSelectedParticleMesh);

			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
#pragma endregion
#pragma endregion

	System_Option();

	ImGui::End();
}

static list<_float3>	lstPoint;
static _float3			vPicked;
static _bool			bPicked(false);
static _bool			bCollide(false);
static _bool			bPickerEnable(false);

void CScene_Tool::System_Option()
{
	ImGui::SeparatorText("Utility##System");

	ImGui::Checkbox("Global Gizmo##System", &bGlobalGizmo);
	if (bGlobalGizmo)
	{
		if (ImGui::Button("Apply##System"))
		{
			m_pGlobalGizmo->Initialize_Gizmo(vGismoSize);
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::InputInt2("Gismo Size##System", reinterpret_cast<int*>(&vGismoSize));
		ImGui::PopItemWidth();
	}

	if (ImGui::TreeNode("Light##System"))
	{
		ImGui::InputFloat3("Direction##System", reinterpret_cast<_float*>(&tLightDesc.vDirection));
		ImGui::InputFloat4("Diffuse##System", reinterpret_cast<_float*>(&tLightDesc.vDiffuse));
		ImGui::InputFloat4("Ambient##System", reinterpret_cast<_float*>(&tLightDesc.vAmbient));
		ImGui::InputFloat4("Specular##System", reinterpret_cast<_float*>(&tLightDesc.vSpecular));

		if (ImGui::Button("Apply##System"))
		{
			CGameInstance::Get_Instance()->Clear_Lights(SCENE::TOOL);
			CGameInstance::Get_Instance()->Add_Light(SCENE::TOOL, tLightDesc, nullptr);
		}

		ImGui::TreePop();
	}

#pragma region Picker
	if (ImGui::TreeNode("Picker"))
	{
		lstPoint.clear();
		bCollide = false;

		ImGui::Checkbox("Enable##Picker", &bPickerEnable);

		if (bPickerEnable)
		{
			ImGui::Text(bPicked ? "Pick Success!" : "Pick Fail!");
			ImGui::InputFloat3("Position##Picked", reinterpret_cast<_float*>(&vPicked), "%.3f", ImGuiInputTextFlags_ReadOnly);

			if (CGameInstance::Get_Instance()->Key_Down(VK_LBUTTON) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			{
				for (auto& pModel : m_mapNonAnimModels)
				{
					pModel.second->Iterate_Meshes(
						[&](shared_ptr<CMesh> _pMesh)->_bool
						{
							_float3 vOut;
							if (_pMesh->Intersect(vOut))
							{
								bCollide = true;
								lstPoint.emplace_back(vOut);
							}

							return true;
						}
					);
				}

				if (true == bCollide)
				{
					_float3 vCamPos = _float3(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION));
					_float3 vNearest = vCamPos;

					for (auto& vPoint : lstPoint)
					{
						if (vCamPos == vNearest)
						{
							vNearest = vPoint;
						}

						if (_float3(vCamPos - vPoint).length() < _float3(vCamPos - vNearest).length())
						{
							vNearest = vPoint;
						}
					}

					vPicked = vNearest;
				}

				bPicked = bCollide;
			}
		}

		ImGui::TreePop();
	}
#pragma endregion
}

void CScene_Tool::Info_Model()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 360.f, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SetNextWindowSize(ImVec2(360.f, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing()));

	ImGui::Begin("Information:Model", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	static _uint			iSelectedItem(-1);
	static _uint			iItemIdx;
	
	static _uint			iSelectedAnimation(-1);
	static _uint			iSelectedMaterial(-1);
	static _uint			iAnimationIdx;
	static _uint			iMaterialIdx;

	static _uint			iCurrentChannelIdx(0);
	static _uint			iKeyFrameIdx;
	static _uint			iSelectedTexture(-1);
	static _uint			iTextureIdx;

	static _uint			iRangeBone;
	static _uint			iRangeAnimation;
	static _uint			iRangeMesh;
	static _uint			iRangeMaterial;

	static SHADERDESC		tShaderDesc;
	static _uint			iSubShaderPassIdx(0);

	static aiTextureType	eTexType(aiTextureType_DIFFUSE);

	if (m_pairSelectedModel.second)
	{
		ImGui::SeparatorText("Properties");

		iItemIdx = 0;

#pragma region Bone List
		if (ImGui::CollapsingHeader("Bones"))
		{
			if (ImGui::BeginListBox("Bone: ", ImVec2(-FLT_MIN, 0.f)))
			{
				for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumBones(); ++i)
				{
					if (ImGui::Selectable(m_pairSelectedModel.second->Get_Bone(i)->Get_Name(), iSelectedItem == iItemIdx))
					{
						iSelectedItem = iItemIdx;
					}
					++iItemIdx;
				}
				ImGui::EndListBox();
			}
		}
		else
		{
			iItemIdx += m_pairSelectedModel.second->Get_NumBones();
		}

		iRangeBone = iItemIdx;
#pragma endregion
#pragma region Animation List
		if (ImGui::CollapsingHeader("Animations"))
		{
			if (ImGui::BeginListBox("Animation: ", ImVec2(-FLT_MIN, 0.f)))
			{
				iAnimationIdx = 0;
				for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumAnimations(); ++i)
				{
					if (ImGui::Selectable(m_pairSelectedModel.second->Get_Animation(iAnimationIdx)->Get_Name(), iSelectedItem == iItemIdx))
					{
						iSelectedItem			= iItemIdx;
						iSelectedAnimation		= iAnimationIdx;
						iCurrentChannelIdx		= 0;
						iSelectedKeyFrame		= 0;
						m_pSelectedAnimation	= m_pairSelectedModel.second->Get_Animation(i);

						if (m_pairSelectedModel.second)
						{
							m_pairSelectedModel.second->Set_Animation(i);
						}
					}
					++iItemIdx;
					++iAnimationIdx;
				}
				ImGui::EndListBox();
			}
		}
		else
		{
			iItemIdx += m_pairSelectedModel.second->Get_NumAnimations();
		}

		iRangeAnimation = iItemIdx;
#pragma endregion
#pragma region Mesh List
		if (ImGui::CollapsingHeader("Meshes"))
		{
			if (ImGui::BeginListBox("Mesh: ", ImVec2(-FLT_MIN, 0.f)))
			{
				static _uint iMeshIndex(0);

				for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumMeshes(); ++i)
				{
					if (ImGui::Selectable(string(std::to_string(i) + m_pairSelectedModel.second->Get_Mesh(i)->Get_Name()).c_str(), iSelectedItem == iItemIdx))
					{
						iSelectedItem = iItemIdx;

						m_pairSelectedMesh.first = string(std::to_string(i) + m_pairSelectedModel.second->Get_Mesh(i)->Get_Name());
						m_pairSelectedMesh.second = m_pairSelectedModel.second->Get_Mesh(i);

						tShaderDesc = m_pairSelectedModel.second->Get_ShaderDesc(i);
					}
					++iItemIdx;
				}
				ImGui::EndListBox();
			}
		}
		else
		{
			iItemIdx += m_pairSelectedModel.second->Get_NumMeshes();
		}

		iRangeMesh = iItemIdx;
#pragma endregion
#pragma region Material List
		if (ImGui::CollapsingHeader("Materials"))
		{
			if (ImGui::BeginListBox("Material: ", ImVec2(-FLT_MIN, 0.f)))
			{
				iMaterialIdx = 0;
				for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumMaterials(); ++i)
				{
					if (ImGui::Selectable(string("Material " + std::to_string(iMaterialIdx)).c_str(), iSelectedItem == iItemIdx))
					{
						iSelectedItem		= iItemIdx;
						iSelectedMaterial	= iMaterialIdx;
						iSelectedTexture	= -1;
						m_tSelectedMaterial	= m_pairSelectedModel.second->Get_Material(i);
					}
					++iItemIdx;
					++iMaterialIdx;
				}
				ImGui::EndListBox();
			}
		}
		else
		{
			iItemIdx += m_pairSelectedModel.second->Get_NumMaterials();
		}

		iRangeMaterial = iItemIdx;
#pragma endregion
	}

	if (m_pairSelectedModel.second)
	{
#pragma region Bones
		if (Function::InRange(iSelectedItem, 0u, iRangeBone))
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Bones");
		}
#pragma endregion
#pragma region Animations
		if (Function::InRange(iSelectedItem, iRangeBone, iRangeAnimation))
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Animations");
	
			if (m_pSelectedAnimation)
			{
				if (ImGui::TreeNodeEx(m_pSelectedAnimation->Get_Name(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::SetNextItemWidth(120.f);
					if (ImGui::BeginCombo(string("Channel(" + std::to_string(m_pSelectedAnimation->Get_NumChannels()) + ")").c_str(), string("Channel " + std::to_string(iCurrentChannelIdx)).c_str()))
					{
						for (_uint i = 0; i < m_pSelectedAnimation->Get_NumChannels(); ++i)
						{
							const _bool bSelected = iCurrentChannelIdx == i;
							if (ImGui::Selectable((string("Channel ") + std::to_string(i)).c_str(), bSelected))
							{
								iCurrentChannelIdx = i;
								iSelectedKeyFrame = 0;
							}
	
							if (bSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::IsItemHovered())
					{
						if (_float fWheelDeltaV = ImGui::GetIO().MouseWheel)
						{
							if (0 > fWheelDeltaV)
							{
								if (iCurrentChannelIdx < m_pSelectedAnimation->Get_NumChannels() - 1)
								{
									iCurrentChannelIdx += 1;
									iSelectedKeyFrame = 0;
								}
							}
							else
							{
								if (iCurrentChannelIdx > 0)
								{
									iCurrentChannelIdx -= 1;
									iSelectedKeyFrame = 0;
								}
							}
						}
	
						if (CGameInstance::Get_Instance()->Key_Down(VK_DOWN))
						{
							if (iSelectedKeyFrame < static_cast<_int>(m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Get_NumKeyFrames() - 1))
							{
								iSelectedKeyFrame += 1;
							}
						}
						if (CGameInstance::Get_Instance()->Key_Down(VK_UP))
						{
							if (iSelectedKeyFrame > 0)
							{
								iSelectedKeyFrame -= 1;
							}
						}
						if (CGameInstance::Get_Instance()->Key_Down(VK_LEFT))
						{
							if (iCurrentChannelIdx > 0)
							{
								iCurrentChannelIdx -= 1;
								iSelectedKeyFrame = 0;
							}
						}
						if (CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
						{
							if (iCurrentChannelIdx < m_pSelectedAnimation->Get_NumChannels() - 1)
							{
								iCurrentChannelIdx += 1;
								iSelectedKeyFrame = 0;
							}
						}
	
						if (CGameInstance::Get_Instance()->Key_Down(VK_DELETE))
						{
							if (iCurrentChannelIdx != -1)
							{
								if (FAILED(m_pSelectedAnimation->Remove_Channel(iCurrentChannelIdx)))
								{
									MSG_BOX("CScene_Tool::Info_Model", "Failed to Remove_Channel");
								}
	
								if (iCurrentChannelIdx == m_pSelectedAnimation->Get_NumChannels())
								{
									iCurrentChannelIdx -= 1;
								}
	
								iSelectedKeyFrame = 0;
							}
						}
					}
	
					_float fWindowWidth1 = ImGui::GetWindowWidth();
					_float fButtonWidth1 = ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemInnerSpacing.y;
					_float fButtonSpace1 = ImGui::GetStyle().ItemInnerSpacing.x;
	
					static _char szBoneIndex[32]	= "0";
					static _uint iFindBoneIndex		= 0;
					ImGui::SameLine(fWindowWidth1 - fButtonWidth1 - ImGui::GetStyle().WindowPadding.x);
					ImGui::Button("~##FindChannel", ImVec2(fButtonWidth1, fButtonWidth1));
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("Find by Bone Index:");
						if (ImGui::InputText("##Input_FindChannel", szBoneIndex, IM_ARRAYSIZE(szBoneIndex), ImGuiInputTextFlags_CharsDecimal))
						{
							_uint iChannelIdx = m_pSelectedAnimation->Get_ChannelIndex(atoi(szBoneIndex));
							if (m_pSelectedAnimation->Get_NumChannels() != iChannelIdx)
							{
								iCurrentChannelIdx = iChannelIdx;
							}
						}
						if (ImGui::Button("Close"))
						{
							ImGui::CloseCurrentPopup();
						}
	
						ImGui::EndPopup();
					}
	
					if (iCurrentChannelIdx != -1)
					{
						ImGui::Text("Channel: %d, KeyFrame: %d, BoneIndex: %d", iCurrentChannelIdx, iSelectedKeyFrame, m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Get_BoneIndex());
	
						ImGui::SameLine(fWindowWidth1 - fButtonWidth1 - ImGui::GetStyle().WindowPadding.x);
						if (ImGui::Button("-##RemoveChannel", ImVec2(fButtonWidth1, fButtonWidth1)))
						{
							if (FAILED(m_pSelectedAnimation->Remove_Channel(iCurrentChannelIdx)))
							{
								MSG_BOX("CScene_Tool::Info_Model", "Failed to Remove_Channel");
							}
	
							if (iCurrentChannelIdx == m_pSelectedAnimation->Get_NumChannels())
							{
								iCurrentChannelIdx -= 1;
							}
	
							iSelectedKeyFrame = 0;
						}
	
						if (ImGui::BeginListBox("Keyframe Listbox", ImVec2(-FLT_MIN, 0.f)))
						{
							iKeyFrameIdx = 0;
							for (_uint i = 0; i < m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Get_NumKeyFrames(); ++i)
							{
								ImGui::PushID(iKeyFrameIdx);
								if (ImGui::Selectable(string("KeyFrame " + std::to_string(i)).c_str(), iSelectedKeyFrame == iKeyFrameIdx))
								{
									iSelectedKeyFrame = iKeyFrameIdx;
								}
								ImGui::PopID();
	
								++iKeyFrameIdx;
							}
							ImGui::EndListBox();
						}
	
						if (iSelectedKeyFrame != -1)
						{
							KEYFRAME tSelectKeyFrame{}, tCompareKeyFrame{};
	
							if (iSelectedKeyFrame < static_cast<_int>(m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Get_NumKeyFrames()))
							{
								ImGui::SeparatorText(m_pairSelectedModel.second->Get_Bone(m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Get_BoneIndex())->Get_Name());
								ImGui::BeginGroup();
								{
									tSelectKeyFrame = m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Get_KeyFrame(iSelectedKeyFrame);
	
									ImGui::InputFloat4	("Scale",		reinterpret_cast<_float*>(&tSelectKeyFrame.vScale),				"%.3f", ImGuiInputTextFlags_ReadOnly);
									ImGui::InputFloat4	("Rotation",	reinterpret_cast<_float*>(&tSelectKeyFrame.vRotation),			"%.3f", ImGuiInputTextFlags_ReadOnly);
									ImGui::InputFloat4	("Translation",	reinterpret_cast<_float*>(&tSelectKeyFrame.vTranslation),		"%.3f", ImGuiInputTextFlags_ReadOnly);
									ImGui::InputFloat	("Time",		reinterpret_cast<_float*>(&tSelectKeyFrame.fTime), 0.f, 0.f,	"%.3f", ImGuiInputTextFlags_ReadOnly);
	
									m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Set_KeyFrame(iSelectedKeyFrame, tSelectKeyFrame);
								}
								ImGui::EndGroup();
								ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(110, 110, 128, 128));
	
								_uint iCompareAnimation = 0;
	
								shared_ptr<CChannel> pCompareChannel = m_pairSelectedModel.second->Get_Animation(iCompareAnimation)->Get_Channel(
									m_pSelectedAnimation->Get_Channel(iCurrentChannelIdx)->Get_BoneIndex(), true);
								if (nullptr != pCompareChannel
								&&	iSelectedKeyFrame < static_cast<_int>(pCompareChannel->Get_NumKeyFrames()))
								{
									ImGui::SeparatorText(string("Compare: " + string(m_pairSelectedModel.second->Get_Animation(iCompareAnimation)->Get_Name())).c_str());
									ImGui::BeginGroup();
									{
										tCompareKeyFrame = pCompareChannel->Get_KeyFrame(iSelectedKeyFrame);
	
										ImGui::InputFloat4	("Scale##Compare",			reinterpret_cast<_float*>(&tCompareKeyFrame.vScale),			"%.3f", ImGuiInputTextFlags_ReadOnly);
										ImGui::InputFloat4	("Rotation##Compare",		reinterpret_cast<_float*>(&tCompareKeyFrame.vRotation),			"%.3f", ImGuiInputTextFlags_ReadOnly);
										ImGui::InputFloat4	("Translation##Compare",	reinterpret_cast<_float*>(&tCompareKeyFrame.vTranslation),		"%.3f", ImGuiInputTextFlags_ReadOnly);
										ImGui::InputFloat	("Time##Compare",			reinterpret_cast<_float*>(&tCompareKeyFrame.fTime), 0.f, 0.f,	"%.3f", ImGuiInputTextFlags_ReadOnly);
									}
									ImGui::EndGroup();
									ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(110, 110, 128, 128));
	
									_bool bKeyFrameEqual =
										Function::NearZero4(XMVectorSubtract(XMLoadFloat4(&tSelectKeyFrame.vScale),			XMLoadFloat4(&tCompareKeyFrame.vScale))) &&
										Function::NearZero4(XMVectorSubtract(XMLoadFloat4(&tSelectKeyFrame.vRotation),		XMLoadFloat4(&tCompareKeyFrame.vRotation))) &&
										Function::NearZero4(XMVectorSubtract(XMLoadFloat4(&tSelectKeyFrame.vTranslation),	XMLoadFloat4(&tCompareKeyFrame.vTranslation)));
									
									ImGui::Text(bKeyFrameEqual ? "Equal" : "Not Equal");
								}
							}
						}
					}
					
					ImGui::TreePop();
				}
			}
		}
	
#pragma endregion
#pragma region Meshes
		if (Function::InRange(iSelectedItem, iRangeAnimation, iRangeMesh))
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Meshes");

			if (m_pairSelectedMesh.second)
			{
				ImGui::Text("Material Index: ", m_pairSelectedMesh.second->Get_MaterialIndex());
				ImGui::SameLine();
				if (ImGui::Button(string("Material " + std::to_string(m_pairSelectedMesh.second->Get_MaterialIndex())).c_str()))
				{
					iSelectedItem		= iRangeMaterial - 1;
					iSelectedMaterial	= m_pairSelectedMesh.second->Get_MaterialIndex();
					iSelectedTexture	= -1;
					m_tSelectedMaterial = m_pairSelectedModel.second->Get_Material(iSelectedMaterial);
				}
			}

			if (ImGui::TreeNode("Hide"))
			{
				ImGui::BeginChild("##ScrollingRegion", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 8), true);

				for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumMeshes(); ++i)
				{
					string	strLabel	= "Mesh " + std::to_string(i);
					_bool	bHidden		= m_pairSelectedModel.second->Is_MeshHidden(i);
					if (ImGui::Checkbox(strLabel.c_str(), &bHidden))
					{
						m_pairSelectedModel.second->Hide_Mesh(i, bHidden);
					}
				}

				ImGui::EndChild();
				ImGui::TreePop();
			}

			if (m_pairSelectedMesh.second)
			{
				static _bool	bApplyShaderDesc(false);

				if (ImGui::TreeNode("Shader"))
				{
					if (ImGui::Checkbox("Tool Shader", &bToolShader))
					{
						m_pairSelectedModel.second->Set_ManualShader(!bToolShader);
					}

					if (!bToolShader)
					{
						ImGui::Text("Manual Shader!");
					}
					else
					{
						if (ImGui::TreeNode("SubShader"))
						{
							if (ImGui::Button("Apply##SubShaderIdx"))
							{
								iSubShaderPassIdx = std::clamp(iSubShaderPassIdx, 0u, 100u);
								m_pairSelectedModel.second->Set_SubShaderPassIndex(iSubShaderPassIdx);
							}
							ImGui::SameLine();
							ImGui::PushItemWidth(100);
							if (ImGui::InputInt("SubShader Index", reinterpret_cast<_int*>(&iSubShaderPassIdx)))
							{
								iSubShaderPassIdx = std::clamp(iSubShaderPassIdx, 0u, 100u);
							}
							ImGui::PopItemWidth();

							if (ImGui::TreeNode("SubShader Properties"))
							{
								ImGui::BeginChild("##SubShaderScrollingRegion", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 8), true);

								for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumMeshes(); ++i)
								{
									string	strLabel	= "Mesh " + std::to_string(i);
									_bool	isSubShader	= m_pairSelectedModel.second->Is_SubShaderPass(i);
									if (ImGui::Checkbox(strLabel.c_str(), &isSubShader))
									{
										m_pairSelectedModel.second->Set_SubShaderPass(i, isSubShader);
									}
								}

								ImGui::EndChild();
								ImGui::TreePop();
							}

							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Shader Flag"))
						{
							if (ImGui::CheckboxFlags("Texture Diffuse##Shader",			&tShaderDesc.iShaderFlag,	SHADER_FLAG_TEXDIFFUSE))			bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Texture Normal##Shader",			&tShaderDesc.iShaderFlag,	SHADER_FLAG_TEXNORMAL))				bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner X Positive##Shader",	&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_POSITIVE_X))	bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner Y Positive##Shader",	&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_POSITIVE_Y))	bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner Z Positive##Shader",	&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_POSITIVE_Z))	bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner X Negative##Shader",	&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_NEGATIVE_X))	bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner Y Negative##Shader",	&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_NEGATIVE_Y))	bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("TriPlaner Z Negative##Shader",	&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_NEGATIVE_Z))	bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner X-Share##Shader",		&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_SHARE_X))		bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner Y-Share##Shader",		&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_SHARE_Y))		bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner Z-Share##Shader",		&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_SHARE_Z))		bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner XZ-Share##Shader",		&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_SHARE_X_Z))	bApplyShaderDesc = true;
							if (ImGui::CheckboxFlags("Triplaner XZ-Sync##Shader",		&tShaderDesc.iShaderFlag,	SHADER_FLAG_TRIPLANER_SYNC_XZ))		bApplyShaderDesc = true;

							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Texture Tiling"))
						{
							if (ImGui::DragFloat("Diffuse##Shader", &tShaderDesc.fDiffuseTiling, 0.1f, 0.f, 10.f))	bApplyShaderDesc = true;
							if (ImGui::DragFloat("Normal##Shader", &tShaderDesc.fNormalTiling, 0.1f, 0.f, 10.f))	bApplyShaderDesc = true;

							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Shader Material"))
						{
							if (ImGui::TreeNode("Diffuse##ShaderMaterial"))
							{
								if (ImGui::ColorPicker4("Diffuse##Picker", reinterpret_cast<_float*>(&tShaderDesc.tMaterialDesc.vDiffuse),
									ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB))					bApplyShaderDesc = true;
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Ambient##ShaderMaterial"))
							{
								if (ImGui::ColorPicker4("Ambient##Picker", reinterpret_cast<_float*>(&tShaderDesc.tMaterialDesc.vAmbient),
									ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB))					bApplyShaderDesc = true;
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Specular##ShaderMaterial"))
							{
								if (ImGui::ColorPicker4("Specular##Picker", reinterpret_cast<_float*>(&tShaderDesc.tMaterialDesc.vSpecular),
									ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB))					bApplyShaderDesc = true;
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Emissive##ShaderMaterial"))
							{
								if (ImGui::ColorPicker4("Emissive##Picker", reinterpret_cast<_float*>(&tShaderDesc.tMaterialDesc.vEmissive),
									ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB))					bApplyShaderDesc = true;
								ImGui::TreePop();
							}

							if (ImGui::InputFloat("Shine##Shader", &tShaderDesc.tMaterialDesc.fShininess))			bApplyShaderDesc = true;

							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::Button("Apply##ShaderDesc"))
				{
					m_pairSelectedModel.second->Set_ShaderDesc(m_pairSelectedModel.second->Get_MeshIndex(m_pairSelectedMesh.second), tShaderDesc);
				}

			//	if (bApplyShaderDesc)
			//	{
			//		bApplyShaderDesc = false;
			//
			//		m_pairSelectedModel.second->Set_ShaderDesc(m_pairSelectedModel.second->Get_MeshIndex(m_pairSelectedMesh.second), tShaderDesc);
			//	}

				ImGui::Separator();

				if (ImGui::Button("Export"))
				{
					const _char* szFilters = "Meshes (*.msh,*.fbx){.msh},Binary (*.msh){.msh},FBX (*.fbx){.fbx},All files{.*}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_EXPORT_MESH, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}
			}
		}
#pragma endregion
#pragma region Materials
		if (Function::InRange(iSelectedItem, iRangeMesh, iRangeMaterial))
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Materials");
	
			static _int		iCurrentMaterialIdx(0);
			const _char*	szTextureTypes[]	= { "Diffuse", "Normal" };
			const _char*	szPreview			= szTextureTypes[iCurrentMaterialIdx];
			if (ImGui::TreeNodeEx(string("Material " + std::to_string(iSelectedMaterial)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SetNextItemWidth(120.f);
				if (ImGui::BeginCombo("Type", szPreview))
				{
					for (int i = 0; i < IM_ARRAYSIZE(szTextureTypes); ++i)
					{
						const _bool bSelected = iCurrentMaterialIdx == i;
						if (ImGui::Selectable(szTextureTypes[i], bSelected))
						{
							iSelectedTexture	= -1;
							iCurrentMaterialIdx	= i;
							szPreview			= szTextureTypes[i];
						}
	
						if (bSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::IsItemHovered())
				{
					if (_float fWheelDeltaV = ImGui::GetIO().MouseWheel)
					{
						if (0 > fWheelDeltaV && iCurrentMaterialIdx == 0)
						{
							iSelectedTexture	= -1;
							iCurrentMaterialIdx = 1;
						}
						if (0 < fWheelDeltaV && iCurrentMaterialIdx == 1)
						{
							iSelectedTexture	= -1;
							iCurrentMaterialIdx = 0;
						}
					}
				}
	
				switch (iCurrentMaterialIdx)
				{
				case 0:
					eTexType = aiTextureType_DIFFUSE;
					break;
				case 1:
					eTexType = aiTextureType_NORMALS;
					break;
				}
	
				_float fWindowWidth1 = ImGui::GetWindowWidth();
				_float fButtonWidth1 = ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemInnerSpacing.y;
				_float fButtonSpace1 = ImGui::GetStyle().ItemInnerSpacing.x;
			
				ImGui::SameLine(fWindowWidth1 - fButtonWidth1 * 5.f - fButtonSpace1 * 4.f - ImGui::GetStyle().WindowPadding.x);
				if (ImGui::Button("*##DuplicateFromMaterial", ImVec2(fButtonWidth1, fButtonWidth1)))
				{
					if (iSelectedTexture != -1)
					{
						if (FAILED(m_tSelectedMaterial.pTexture[IDX(eTexType)]->Push_ShaderResourceView(m_tSelectedMaterial.pTexture[IDX(eTexType)]->Get_TexturePath(iSelectedTexture))))
						{
							MSG_BOX("CScene_Tool::Info_Model", "Failed to Remove Push_ShaderResourceView");
						}
					}
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::Button("+##AddFromMaterial", ImVec2(fButtonWidth1, fButtonWidth1)))
				{
					const _char* szFilters = "All files{.*},WIC files(*.png *.jpg *.jpeg){.png,.jpg,.jpeg},DDS files(*.dds){.dds}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_OPEN_TEXTURE, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::Button("-##EraseFromMaterial", ImVec2(fButtonWidth1, fButtonWidth1)))
				{
					if (iSelectedTexture != -1)
					{
						if (SUCCEEDED(m_tSelectedMaterial.pTexture[IDX(eTexType)]->Remove_ShaderResourceView(iSelectedTexture)))
						{
							iSelectedTexture = -1;
						}
						else
						{
							MSG_BOX("CScene_Tool::Info_Model", "Failed to Remove ShaderResourceView");
						}
					}
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::ArrowButton("##UpFromMaterial", ImGuiDir_Up))
				{
					if (m_tSelectedMaterial.pTexture[IDX(eTexType)]->Swap_ShaderResourceView(iSelectedTexture, iSelectedTexture - 1))
					{
						--iSelectedTexture;
					}
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::ArrowButton("##DownFromMaterial", ImGuiDir_Down))
				{
					if (m_tSelectedMaterial.pTexture[IDX(eTexType)]->Swap_ShaderResourceView(iSelectedTexture, iSelectedTexture + 1))
					{
						++iSelectedTexture;
					}
				}
	
				if (ImGui::BeginListBox("Texture Listbox", ImVec2(-FLT_MIN, 0.f)))
				{
					iTextureIdx = 0;
					if (m_tSelectedMaterial.pTexture[IDX(eTexType)])
					{
						for (_uint i = 0; i < m_tSelectedMaterial.pTexture[IDX(eTexType)]->Get_NumTextures(); ++i)
						{
							wstring wstrTexturePath = m_tSelectedMaterial.pTexture[IDX(eTexType)]->Get_TexturePath(i);
							wstring wstrFileName, wstrExtension;
							Function::SplitPath(wstrTexturePath, nullptr, nullptr, &wstrFileName, &wstrExtension);

							ImGui::PushID(iTextureIdx);
							if (ImGui::Selectable(Function::ToString(wstrFileName + wstrExtension).c_str(), iSelectedTexture == iTextureIdx))
							{
								iSelectedTexture = iTextureIdx;
							}
							ImGui::PopID();

							++iTextureIdx;
						}
						ImGui::EndListBox();
					}
				}
	
				if (iSelectedTexture != -1)
				{
					ImVec2 imgSize;
	
					D3D11_TEXTURE2D_DESC tTexture2dDesc{};
					m_tSelectedMaterial.pTexture[IDX(eTexType)]->Get_Texture2D(iSelectedTexture)->GetDesc(&tTexture2dDesc);
	
					_float fWidth	= static_cast<_float>(tTexture2dDesc.Width);
					_float fHeight	= static_cast<_float>(tTexture2dDesc.Height);
					_float fAspect	= fWidth / fHeight;
	
					if (fAspect > 1.f)
					{
						imgSize.x = 300.f;
						imgSize.y = 300.f / fAspect;
					}
					else
					{
						imgSize.x = 300.f * fAspect;
						imgSize.y = 300.f;
					}
	
					ImGui::Image(m_tSelectedMaterial.pTexture[IDX(eTexType)]->Get_ShaderResourceView(iSelectedTexture).Get(), imgSize);
					if (ImGui::BeginItemTooltip())
					{
						ImGui::Text("%.0fx%.0f", fWidth, fHeight);
						ImGui::EndTooltip();
					}
				}
	
				ImGui::TreePop();
			}
		}
#pragma endregion
	}
#pragma region File Dialog

	if (ImGuiFileDialog::Instance()->Display(DIALOG_OPEN_TEXTURE))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(m_tSelectedMaterial.pTexture[IDX(eTexType)]->Push_ShaderResourceView(Function::ToWString(ImGuiFileDialog::Instance()->GetFilePathName()))))
			{
				MSG_BOX("Scene_Tool::Info_Model", "Failed to DIALOG_OPEN_TEXTURE");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

	if (ImGuiFileDialog::Instance()->Display(DIALOG_EXPORT_MESH))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(m_pairSelectedMesh.second->Export(Function::ToWString(ImGuiFileDialog::Instance()->GetFilePathName()))))
			{
				MSG_BOX("Scene_Tool::Info_Model", "Failed to DIALOG_EXPORT_MESH");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion
	ImGui::End();
}

static _int			iSelectedParticleInstance(0);
static _float4x4	mCopy;

void CScene_Tool::Info_Effect()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 360.f, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SetNextWindowSize(ImVec2(360.f, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing()));

	ImGui::Begin("Information:Effect", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

#pragma region Texture

	static aiTextureType eTexture = AI_TEXTURE_TYPE_MAX;

	static _bool bTexDiffuseSelected	= eTexture == aiTextureType_DIFFUSE;
	static _bool bTexNormalSelected		= eTexture == aiTextureType_NORMALS;

	if (m_pairSelectedParticleMesh.second)
	{
		if (ImGui::CollapsingHeader("Texture##ParticleMeshTexture"))
		{
			if (ImGui::BeginListBox("Texture##ParticleMeshTextureListBox", ImVec2(-FLT_MIN, 0.f)))
			{
				if (ImGui::Selectable("Diffuse", &bTexDiffuseSelected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					bTexNormalSelected = false;

					eTexture = aiTextureType_DIFFUSE;

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						const _char* szFilters = "All files{.*},WIC files(*.png *.jpg *.jpeg){.png,.jpg,.jpeg},DDS files(*.dds){.dds}";
						ImGuiFileDialog::Instance()->OpenDialog(DIALOG_SET_TEXTURE, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
					}
				}
				if (ImGui::Selectable("Normal", &bTexNormalSelected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					bTexDiffuseSelected = false;

					eTexture = aiTextureType_NORMALS;

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						const _char* szFilters = "All files{.*},WIC files(*.png *.jpg *.jpeg){.png,.jpg,.jpeg},DDS files(*.dds){.dds}";
						ImGuiFileDialog::Instance()->OpenDialog(DIALOG_SET_TEXTURE, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
					}
				}

				ImGui::EndListBox();
			}
		}

		if (eTexture == aiTextureType_DIFFUSE || eTexture == aiTextureType_NORMALS)
		{
			ImVec2 imgSize;

			D3D11_TEXTURE2D_DESC tTexture2dDesc{};
			shared_ptr<CTexture> pTexture = m_pairSelectedParticleMesh.second->Get_Texture(eTexture);
			if (pTexture)
			{
				pTexture->Get_Texture2D()->GetDesc(&tTexture2dDesc);

				_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
				_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
				_float fAspect = fWidth / fHeight;

				if (fAspect > 1.f)
				{
					imgSize.x = 300.f;
					imgSize.y = 300.f / fAspect;
				}
				else
				{
					imgSize.x = 300.f * fAspect;
					imgSize.y = 300.f;
				}

				ImGui::Image(pTexture->Get_ShaderResourceView().Get(), imgSize);
				if (ImGui::BeginItemTooltip())
				{
					ImGui::Text("%.0fx%.0f", fWidth, fHeight);
					ImGui::EndTooltip();
				}
			}
		}

#pragma endregion
#pragma region Properties

		static CVFX_ParticleMesh::TYPE eParticleType(CVFX_ParticleMesh::TYPE::BOUNCE);
		static _int iTypeRadioOption(0);
		static _int iActiveInstance(m_pairSelectedParticleMesh.second->Get_ActivateInstances());

		if (ImGui::CollapsingHeader("Properties##ParticleMeshProperties"))
		{
			if (ImGui::Button("Apply##Area0"))
			{
				m_pairSelectedParticleMesh.second->Set_Type(eParticleType);
				m_pairSelectedParticleMesh.second->Set_ActivateInstances(iActiveInstance);
				m_pairSelectedParticleMesh.second->Update();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Bounce", &iTypeRadioOption, 0))
			{
				iTypeRadioOption = 0;
				eParticleType = CVFX_ParticleMesh::TYPE::BOUNCE;
			}

			ImGui::InputInt("Active Instance", &iActiveInstance);
			iActiveInstance = Function::Clamp(0, static_cast<_int>(m_pairSelectedParticleMesh.second->Get_Component<CVIBuffer>(COMPONENT::VIBUFFER)->Get_NumInstances()), iActiveInstance);

			if (ImGui::BeginListBox("Instance Bounce", ImVec2(-FLT_MIN, 0.f)))
			{
				for (_uint i = 0; i < m_pairSelectedParticleMesh.second->Get_ActivateInstances(); ++i)
				{
					ImGui::PushID(iSelectedParticleInstance);
					if (ImGui::Selectable((string("Instance ") + std::to_string(i)).c_str(), iSelectedParticleInstance == i))
					{
						iSelectedParticleInstance = i;
					}
					ImGui::PopID();
				}
				ImGui::EndListBox();
			}

			ImGui::Separator();

			static _float3		vScale(0.f, 0.f, 0.f);
			static _float3		vRotation(0.f, 0.f, 0.f);
			static _float3		vTranslation(0.f, 0.f, 0.f);
			static _float4x4	mTransformation = g_mUnit;

			const _char*		szItems[] = { "Initial", "Peak", "Final" };
			static _int			iItem = 0;

			ImGui::Combo("Option", &iItem, szItems, IM_ARRAYSIZE(szItems));

			if (ImGui::IsItemHovered())
			{
				if (_float fWheelDeltaV = ImGui::GetIO().MouseWheel)
				{
					if (0 > fWheelDeltaV)
					{
						if (iItem < 2)
						{
							++iItem;
						}
					}
					else
					{
						if (iItem > 0)
						{
							--iItem;
						}
					}
				}
			}

			CVFX_ParticleMesh::BOUNCEDESC tBounceDesc;
			if (m_pairSelectedParticleMesh.second->Get_ActivateInstances())
			{
				tBounceDesc = m_pairSelectedParticleMesh.second->Get_BounceDesc(iSelectedParticleInstance);
			}

			switch (iItem)
			{
			case 0:
				mTransformation = tBounceDesc.mInitialTransformation;
				break;
			case 1:
				mTransformation = tBounceDesc.mPeakTransformation;
				break;
			case 2:
				mTransformation = tBounceDesc.mFinalTransformation;
				break;
			}
			_vector vScl, vQuat, vTrans;
			if (XMMatrixDecompose(&vScl, &vQuat, &vTrans, mTransformation))
			{
				vScale			= vScl;
				vRotation		= Function::QuaternionToEuler(vQuat, false);
				vTranslation	= vTrans;
			}
			else
			{
				ImGui::Text("Failed to XMMatrixDecompose");
			}

			static _uint iTab = 1;
			if (ImGui::BeginTabBar("Transformation"))
			{
				if (ImGui::BeginTabItem("Value"))
				{
					if (ImGui::Button("Reset"))
					{
						vScale			= _float3(1.f, 1.f, 1.f);
						vRotation		= _float3(0.f, 0.f, 0.f);
						vTranslation	= _float3(0.f, 0.f, 0.f);
					}

					if (iTab == 2)
					{
						iTab = 1;
						_vector vScl, vQuat, vTrans;
						if (XMMatrixDecompose(&vScl, &vQuat, &vTrans, mTransformation))
						{
							vScale			= vScl;
							vRotation		= Function::QuaternionToEuler(vQuat, false);
							vTranslation	= vTrans;
						}
						else
						{
							ImGui::Text("Failed to XMMatrixDecompose");
						}
					}
					ImGui::DragFloat3("Scale",				reinterpret_cast<_float*>(&vScale), 0.1f);
					ImGui::DragFloat3("Rotation(Degree)",	reinterpret_cast<_float*>(&vRotation), 1.f, -89.f, 89.f);
					ImGui::DragFloat3("Translation",		reinterpret_cast<_float*>(&vTranslation), 0.1f);
					ImGui::EndTabItem();

					mTransformation = XMMatrixAffineTransformation(vScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
						_float3(XMConvertToRadians(vRotation.x), XMConvertToRadians(vRotation.y), XMConvertToRadians(vRotation.z))), vTranslation);
				}

				if (ImGui::BeginTabItem("Matrix"))
				{
					if (ImGui::Button("Reset"))
					{
						vScale			= _float3(1.f, 1.f, 1.f);
						vRotation		= _float3(0.f, 0.f, 0.f);
						vTranslation	= _float3(0.f, 0.f, 0.f);
						mTransformation	= g_mUnit;
					}

					if (iTab == 1)
					{
						iTab = 2;
						mTransformation = XMMatrixAffineTransformation(vScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
							_float3(XMConvertToRadians(vRotation.x), XMConvertToRadians(vRotation.y), XMConvertToRadians(vRotation.z))), vTranslation);
					}
					ImGui::DragFloat4("Right",		reinterpret_cast<_float*>(&mTransformation._11), 0.1f);
					ImGui::DragFloat4("Up",			reinterpret_cast<_float*>(&mTransformation._21), 0.1f);
					ImGui::DragFloat4("Look",		reinterpret_cast<_float*>(&mTransformation._31), 0.1f);
					ImGui::DragFloat4("Position",	reinterpret_cast<_float*>(&mTransformation._41), 0.1f);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			
			ImGui::InputFloat("Begin Duration",	&tBounceDesc.fBeginDuration);
			ImGui::InputFloat("Peak Duration",	&tBounceDesc.fPeakDuration);
			ImGui::InputFloat("End Duration",	&tBounceDesc.fEndDuration);
			ImGui::InputFloat("Begin Weight",	&tBounceDesc.fBeginWeight);
			ImGui::InputFloat("End Weight",		&tBounceDesc.fEndWeight);

			static _float fRight(0.f);
			static _float fUp(0.f);
			static _float fLook(0.f);

			ImGui::DragFloat("Right",	&fRight,	0.1f);
			ImGui::DragFloat("Up",		&fUp,		0.1f);
			ImGui::DragFloat("Look",	&fLook,		0.1f);

			if (1 == iTab)
			{
				mTransformation = XMMatrixAffineTransformation(vScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
					_float3(XMConvertToRadians(vRotation.x), XMConvertToRadians(vRotation.y), XMConvertToRadians(vRotation.z))), vTranslation);
			}

			*reinterpret_cast<_float4*>(&mTransformation._41) += mTransformation.row(0) * fRight;
			*reinterpret_cast<_float4*>(&mTransformation._41) += mTransformation.row(1) * fUp;
			*reinterpret_cast<_float4*>(&mTransformation._41) += mTransformation.row(2) * fLook;

			fRight	= 0.f;
			fUp		= 0.f;
			fLook	= 0.f;

			if (ImGui::Button("Copy"))
			{
				mCopy = mTransformation;
			}
			ImGui::SameLine();
			if (ImGui::Button("Paste"))
			{
				mTransformation = mCopy;
			}

			switch (iItem)
			{
			case 0:
				tBounceDesc.mInitialTransformation = mTransformation;
				break;
			case 1:
				tBounceDesc.mPeakTransformation = mTransformation;
				break;
			case 2:
				tBounceDesc.mFinalTransformation = mTransformation;
				break;
			}

			m_pairSelectedParticleMesh.second->Set_BounceDesc(iSelectedParticleInstance, tBounceDesc);

			if (ImGui::Button("Apply All Transformation"))
			{
				for (_uint i = 0; i < m_pairSelectedParticleMesh.second->Get_ActivateInstances(); ++i)
				{
					CVFX_ParticleMesh::BOUNCEDESC tDesc = m_pairSelectedParticleMesh.second->Get_BounceDesc(i);
					tDesc.mInitialTransformation	= tBounceDesc.mInitialTransformation;
					tDesc.mPeakTransformation		= tBounceDesc.mPeakTransformation;
					tDesc.mFinalTransformation		= tBounceDesc.mFinalTransformation;
					m_pairSelectedParticleMesh.second->Set_BounceDesc(i, tDesc);
				}
				m_pairSelectedParticleMesh.second->Update();
			}
			ImGui::SameLine();
			if (ImGui::Button("Apply All Option"))
			{
				for (_uint i = 0; i < m_pairSelectedParticleMesh.second->Get_ActivateInstances(); ++i)
				{
					CVFX_ParticleMesh::BOUNCEDESC tDesc = m_pairSelectedParticleMesh.second->Get_BounceDesc(i);
					tDesc.fBeginDuration	= tBounceDesc.fBeginDuration;
					tDesc.fPeakDuration		= tBounceDesc.fPeakDuration;
					tDesc.fEndDuration		= tBounceDesc.fEndDuration;
					tDesc.fBeginWeight		= tBounceDesc.fBeginWeight;
					tDesc.fEndWeight		= tBounceDesc.fEndWeight;
					m_pairSelectedParticleMesh.second->Set_BounceDesc(i, tDesc);
				}
				m_pairSelectedParticleMesh.second->Update();
			}

			m_pairSelectedParticleMesh.second->Update();

			ImGui::Separator();
		}
#pragma endregion
	}
	else
	{
		eTexture = AI_TEXTURE_TYPE_MAX;
	}
#pragma region Dialog

	if (ImGuiFileDialog::Instance()->Display(DIALOG_SET_TEXTURE))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(m_pairSelectedParticleMesh.second->Set_TexturePath(eTexture, Function::ToWString(ImGuiFileDialog::Instance()->GetFilePathName()))))
			{
				MSG_BOX("Scene_Tool::Info_Effect", "Failed to DIALOG_SET_TEXTURE");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion

	ImGui::End();
}

void CScene_Tool::Control_Model()
{
	ImGui::SetNextWindowPos(ImVec2(540.f, ImGui::GetIO().DisplaySize.y - 215.f));
	ImGui::SetNextWindowSize(ImVec2(1020.f, 215.f));

	ImGui::Begin("Control:Model", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	static _bool bAnimationFix = false;
	static _bool bRegisterPoint = false;
	static _float fTrack = 0.f;

	if (m_pairSelectedMesh.second)
	{
		ImGui::Checkbox("Render Full Model", &bRenderFullModel);
	}

	if (m_pairSelectedModel.second)
	{
		if (MODEL::NONANIM == m_pairSelectedModel.second->Get_Type())
		{
			ImGui::Checkbox("Register Point", &bRegisterPoint);

			if (bRegisterPoint)
			{
				if (ImGui::Button("Register##Point"))
				{
					m_vecPoint.emplace_back(vPicked);
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear##Point"))
				{
					m_vecPoint.clear();
				}
				ImGui::SameLine();
				if (ImGui::Button("Export##Point"))
				{
					const _char* szFilters = "Data (*.dat){.dat},All files{.*}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_EXPORT_POINTS, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}

				if (ImGui::BeginListBox("Point List##Point", ImVec2(-FLT_MIN, 0.f)))
				{
					for (auto vPoint : m_vecPoint)
					{
						_char szLabel[MAX_PATH];
						snprintf(szLabel, sizeof(szLabel), "(%.3f, %.3f, %.3f)", vPoint.x, vPoint.y, vPoint.z);
						ImGui::Selectable(szLabel, false, ImGuiSelectableFlags_Disabled);
					}

					ImGui::EndListBox();
				}
			}
		}
		else if (MODEL::ANIM == m_pairSelectedModel.second->Get_Type())
		{
			ImGui::Checkbox("Fix Mode", &bAnimationFix);
		}
	}

	if (m_pSelectedAnimation)
	{
		if (bAnimationFix)
		{
			ImGui::SliderFloat("##AnimationTrackPositionSlider", &fTrack, 0.f, m_pSelectedAnimation->Get_Duration());
			m_pairSelectedModel.second->Fix_Animation(m_pairSelectedModel.second->Get_AnimationIndex(m_pSelectedAnimation), fTrack / m_pSelectedAnimation->Get_Duration());
		}
	}

#pragma region File Dialog

	if (ImGuiFileDialog::Instance()->Display(DIALOG_EXPORT_POINTS))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(Export_BinaryPoints(Function::ToWString(ImGuiFileDialog::Instance()->GetFilePathName()))))
			{
				MSG_BOX("Scene_Tool::Control_Model", "Failed to DIALOG_EXPORT_POINTS");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion

	ImGui::End();
}

void CScene_Tool::Control_Effect()
{
	ImGui::SetNextWindowPos(ImVec2(540.f, ImGui::GetIO().DisplaySize.y - 215.f));
	ImGui::SetNextWindowSize(ImVec2(660.f, 215.f));

	ImGui::Begin("Control:Effect", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Checkbox("Tick", &bTickParticle);

	if (m_pairSelectedParticleMesh.second)
	{
		if (ImGui::Button("Fetch"))
		{
			m_pairSelectedParticleMesh.second->Fetch();
		}

		static _float fTrack = 0.f;
		CVFX_ParticleMesh::BOUNCEDESC tDesc = m_pairSelectedParticleMesh.second->Get_BounceDesc(iSelectedParticleInstance);
		vector<_float> vecTags;

		ImGui::Text("Track Position");
		if (ImGui::SliderFloat("##EffectTrackPositionSlider", &fTrack, 0.f, m_pairSelectedParticleMesh.second->Get_TotalBounceTime()))
		{
		}
		ImVec2 vWidgetPos = ImGui::GetItemRectMin();
		ImVec2 vWidgetSize = ImGui::GetItemRectSize();
		vecTags.emplace_back(tDesc.fBeginDuration);
		vecTags.emplace_back(tDesc.fBeginDuration + tDesc.fPeakDuration);
		vecTags.emplace_back(tDesc.fBeginDuration + tDesc.fPeakDuration + tDesc.fEndDuration);

		for (_float fTag : vecTags)
		{
			_float fNormalizedTag = fTag / m_pairSelectedParticleMesh.second->Get_TotalBounceTime();
			_float fTagX = vWidgetPos.x + vWidgetSize.x * fNormalizedTag;

			ImGui::GetWindowDrawList()->AddLine(
				ImVec2(fTagX, vWidgetPos.y),
				ImVec2(fTagX, vWidgetPos.y + vWidgetSize.y),
				IM_COL32(255, 255, 255, 255), 1.2f);

			_char szLabel[32];
			snprintf(szLabel, sizeof(szLabel), "%.2f", fTag);
			ImGui::GetWindowDrawList()->AddText(
				ImVec2(fTagX + 2, vWidgetPos.y + vWidgetSize.y + 2.f),  // y ÁÂÇ¥ º¯°æ
				IM_COL32(255, 255, 255, 255),
				szLabel);
		}
		ImGui::SameLine();
		static _bool bTimeLock = false;
		if (ImGui::Checkbox("Lock", &bTimeLock))
		{
			m_pairSelectedParticleMesh.second->Set_TimeLock(bTimeLock);
		}
		if (bTimeLock)
		{
			m_pairSelectedParticleMesh.second->Set_Time(fTrack);
		}
	}

	ImGui::End();
}

void CScene_Tool::SubControl_Effect()
{
	ImGui::SetNextWindowPos(ImVec2(1200.f, ImGui::GetIO().DisplaySize.y - 180.f));
	ImGui::SetNextWindowSize(ImVec2(360.f, 180.f));

	ImGui::Begin("SubControl:Effect", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (ImGui::TreeNode("Clipboard"))
	{
		ImGui::InputFloat4("Right##Clipboard",			reinterpret_cast<_float*>(&mCopy._11), "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat4("Up##Clipboard",				reinterpret_cast<_float*>(&mCopy._21), "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat4("Look##Clipboard",			reinterpret_cast<_float*>(&mCopy._31), "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat4("Transformation##Clipboard",	reinterpret_cast<_float*>(&mCopy._41), "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::TreePop();
	}

	ImGui::End();
}

HRESULT CScene_Tool::Load_BinaryMeshInstanceList(const wstring& _wstrFilePath, const wstring& _wstrFileName)
{
	std::ifstream inFile(_wstrFilePath + wstring(TEXT("\\")) + _wstrFileName, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_BinaryMeshInstanceList", "Failed to Open File");
	}

#pragma region Path

	_int	iMaxInstance(0);
	size_t	nSize(0), nLength(0);
	_wchar	wszBuffer[MAX_PATH];

	inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
		//	KEY
		ZeroMemory(wszBuffer, sizeof(_wchar) * MAX_PATH);
		inFile.read(reinterpret_cast<_byte*>(&nLength), sizeof(size_t));
		inFile.read(reinterpret_cast<_byte*>(wszBuffer), sizeof(_wchar) * nLength);
		wstring	wstrKey = wszBuffer;
		string	strKey = Function::ToString(wstrKey);
		//	PATH
		ZeroMemory(wszBuffer, sizeof(_wchar) * MAX_PATH);
		inFile.read(reinterpret_cast<_byte*>(&nLength), sizeof(size_t));
		inFile.read(reinterpret_cast<_byte*>(wszBuffer), sizeof(_wchar) * nLength);
		wstring wstrPath = wszBuffer;
		string	strPath = Function::ToString(wstrPath);
		//	MAX INSTANCE
		inFile.read(reinterpret_cast<_byte*>(&iMaxInstance), sizeof(_int));

		shared_ptr<CVIBufferInstance_Mesh> pPrototype = CVIBufferInstance_Mesh::Create(m_pDevice, m_pContext, wstrPath, iMaxInstance);
		if (nullptr == pPrototype)
		{
			MSG_CONTINUE("CScene_Tool::Load_BinaryMeshInstanceList", "Failed to CVIBufferInstance_Mesh::Create");
		}
		if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TOOL, wstrKey, pPrototype)))
		{
			MSG_CONTINUE("CScene_Tool::Load_BinaryMeshInstanceList", "Failed to Add_Component_Prototype");
		}
		shared_ptr<CVIBufferInstance_Mesh> pInstance = dynamic_pointer_cast<CVIBufferInstance_Mesh>(pPrototype->Clone());
		if (nullptr == pInstance)
		{
			MSG_CONTINUE("CScene_Tool::Load_BinaryMeshInstanceList", "Failed to CVIBufferInstance_Mesh::Clone");
		}
		m_mapExportMeshInstance.emplace(strKey, VIINST{ strPath, iMaxInstance, pInstance });
	}

#pragma endregion

	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_BinaryMeshInstanceList", "Failed to Read File");
	}

	inFile.close();

	return S_OK;
}

HRESULT CScene_Tool::Load_Model(const wstring& _wstrFilePath, const wstring& _wstrFileName, const MODEL _eType, _matrixf _mPivot)
{
	if (!Function::InRange(_eType, static_cast<MODEL>(0), MODEL::MAX))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Invalid MODEL");
	}

	shared_ptr<CModel> pModel = CModel::Create(m_pDevice, m_pContext, _eType, _wstrFilePath + wstring(TEXT("\\")) + _wstrFileName, _mPivot);

	if (nullptr == pModel)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Failed to Create");
	}

	switch (_eType)
	{
	case MODEL::ANIM:
		m_mapAnimModels.emplace(Function::ToString(_wstrFileName), pModel);
		break;

	case MODEL::NONANIM:
		m_mapNonAnimModels.emplace(Function::ToString(_wstrFileName), pModel);
		break;

	default:
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Invalid MODEL");
		break;
	}

	m_wstrModelPath = _wstrFilePath;
	
	return S_OK;
}

HRESULT CScene_Tool::Load_ParticleMesh(const wstring& _wstrFilePath, const wstring& _wstrFileName)
{
	shared_ptr<CVFX_ParticleMesh> pVFX = dynamic_pointer_cast<CVFX_ParticleMesh>(
		CVFX_ParticleMesh::Create(m_pDevice, m_pContext)->Clone(_wstrFilePath + wstring(TEXT("\\")) + _wstrFileName));

	if (nullptr == pVFX)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Failed to Create");
	}

	m_mapParticleMesh.emplace(Function::ToString(_wstrFileName), pVFX);

	m_wstrMeshPath = _wstrFilePath;
	
	return S_OK;
}

HRESULT CScene_Tool::Export_BinaryMeshInstanceList(const wstring& _wstrPath)
{
	std::ofstream outFile(_wstrPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_BinaryMeshInstanceList", "Failed to Open File");
	}

#pragma region Export

	size_t nLength = m_mapExportMeshInstance.size();
	outFile.write(reinterpret_cast<const _byte*>(&nLength),							sizeof(size_t));
	for (auto& pair : m_mapExportMeshInstance)
	{
		//	KEY
		wstring	wstrKey	= Function::ToWString(pair.first);
		nLength = wstrKey.length();
		outFile.write(reinterpret_cast<const _byte*>(&nLength),						sizeof(size_t));
		outFile.write(reinterpret_cast<const _byte*>(wstrKey.c_str()),				sizeof(_wchar) * nLength);
		//	PATH
		wstring	wstrPath = Function::ToWString(pair.second.strPath);
		nLength = wstrPath.length();
		outFile.write(reinterpret_cast<const _byte*>(&nLength),						sizeof(size_t));
		outFile.write(reinterpret_cast<const _byte*>(wstrPath.c_str()),				sizeof(_wchar) * nLength);
		//	MAX INSTANCE
		outFile.write(reinterpret_cast<const _byte*>(&pair.second.iMaxInstance),	sizeof(_int));
	}

#pragma endregion

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_BinaryMeshInstanceList", "Failed to Write File");
	}

	outFile.close();

	return S_OK;
}

HRESULT CScene_Tool::Export_BinaryModel(const wstring& _wstrPath)
{
	shared_ptr<CModel> pOut = m_pairSelectedModel.second;
	if (nullptr == pOut)
	{
		return S_FALSE;
	}

	if (FAILED(pOut->Export(_wstrPath)))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_BinaryModel", "Failed to Export");
	}

	return S_OK;
}

HRESULT CScene_Tool::Export_BinaryParticleMesh(const wstring& _wstrPath)
{
	shared_ptr<CVFX_ParticleMesh> pOut = m_pairSelectedParticleMesh.second;
	if (nullptr == pOut)
	{
		return S_FALSE;
	}

	if (FAILED(pOut->Export(_wstrPath)))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_BinaryParticleMesh", "Failed to Export");
	}

	return S_OK;
}

HRESULT CScene_Tool::Export_BinaryPoints(const wstring& _wstrPath)
{
	std::ofstream outFile(_wstrPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_BinaryPoints", "Failed to Open File");
	}

#pragma region Export

	size_t nLength = m_vecPoint.size();
	outFile.write(reinterpret_cast<const _byte*>(&nLength), sizeof(size_t));
	for (auto vPoint : m_vecPoint)
	{
		outFile.write(reinterpret_cast<const _byte*>(&vPoint), sizeof(_float3));
	}

#pragma endregion

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_BinaryPoints", "Failed to Write File");
	}

	outFile.close();

	return S_OK;
}

shared_ptr<CScene_Tool> CScene_Tool::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Tool> pInstance = make_private_shared(CScene_Tool, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Tool::Create", "Failed to Initialize");
	}

	return pInstance;
}

#endif
