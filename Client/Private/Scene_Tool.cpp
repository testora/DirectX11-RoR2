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

#ifdef _DEBUG
#if ACTIVATE_IMGUI

CScene_Tool::CScene_Tool(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::TOOL)
{
}

HRESULT CScene_Tool::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to __super::Initialize");
	}

	CImGui_Manager::Get_Instance()->Enable();

	return S_OK;
}

void CScene_Tool::Tick(_float _fTimeDelta)
{
	System_Model();
	Info_Model();
}

void CScene_Tool::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CScene_Tool::Render()
{
	return S_OK;
}

void CScene_Tool::System_Model()
{
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGui::SetNextWindowSize(ImVec2(540.f, ImGui::GetIO().DisplaySize.y));

	ImGui::Begin("System:Model", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

#pragma region Open Model

	static MODEL eType(MODEL::NONANIM);
	
	if (ImGui::Button("Load"))
	{
		if (!m_imEmbed_Open.IsOpened())
		{
			if (m_imEmbed_Export.IsOpened())
			{
				m_imEmbed_Export.Close();
			}

			m_imEmbed_Open.OpenDialog(DIALOG_OPEN_FBX, "Open FBX", ".fbx", "Bin/Resources/", 1, nullptr,
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

			const _char* szFilters = "Binary (*.mdl){.mdl},Models (*.fbx, *.mdl){*.fbx *.mdl},All files{.*}";
			m_imEmbed_Export.OpenDialog(DIALOG_EXPORT_MODEL, "Export Model", szFilters, "Bin/Resources/", "",
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

	static _uint iTab = 0;
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

	if (m_imEmbed_Open.Display(DIALOG_OPEN_FBX, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imEmbed_Open.IsOk())
		{
			if (FAILED(Load_Model(Function::ToWString(m_imEmbed_Open.GetCurrentPath()), Function::ToWString(m_imEmbed_Open.GetCurrentFileName()), eType, mPivot)))
			{
				MSG_BOX("CScene_Tool::Tick", "Failed to Load_Model");
			}
		}
	}

	if (m_imEmbed_Export.Display(DIALOG_EXPORT_MODEL, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imEmbed_Export.IsOk())
		{
			if (FAILED(Export_BinaryModel(Function::ToWString(m_imEmbed_Export.GetCurrentPath()), Function::ToWString(m_imEmbed_Export.GetCurrentFileName()))))
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
	if (ImGui::Button("-", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		m_mapAnimModels.erase(m_pairSelectedModel.first);
		m_pairSelectedModel.first.clear();
		m_pairSelectedModel.second = nullptr;
	}
	if (ImGui::BeginListBox("Anim", ImVec2(-FLT_MIN, 0.f)))
	{
		for (auto pair : m_mapAnimModels)
		{
			if (ImGui::Selectable(pair.first.c_str(), m_pairSelectedModel == pair))
			{
				m_pairSelectedModel = pair;
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
	if (ImGui::Button("-", ImVec2(fButtonWidth0, fButtonWidth0)))
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
				m_pairSelectedModel = pair;
			}

			if (m_pairSelectedModel == pair)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}

#pragma endregion

	ImGui::End();
}

void CScene_Tool::Info_Model()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 360.f, 0.f));
	ImGui::SetNextWindowSize(ImVec2(360.f, ImGui::GetIO().DisplaySize.y));

	ImGui::Begin("Information:Model", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	static _uint			iSelectedItem(-1);
	static _uint			iItemIdx;
	
	static _uint			iSelectedMaterial(-1);
	static _uint			iMaterialIdx;

	static _uint			iSelectedTexture(-1);
	static _uint			iTextureIdx;

	static _uint			iRangeBone;
	static _uint			iRangeAnimation;
	static _uint			iRangeMesh;
	static _uint			iRangeMaterial;

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
				for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumAnimations(); ++i)
				{
					if (ImGui::Selectable(m_pairSelectedModel.second->Get_Animation(i)->Get_Name(), iSelectedItem == iItemIdx))
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
			iItemIdx += m_pairSelectedModel.second->Get_NumAnimations();
		}

		iRangeAnimation = iItemIdx;
#pragma endregion
#pragma region Mesh List
		if (ImGui::CollapsingHeader("Meshes"))
		{
			if (ImGui::BeginListBox("Mesh: ", ImVec2(-FLT_MIN, 0.f)))
			{
				for (_uint i = 0; i < m_pairSelectedModel.second->Get_NumMeshes(); ++i)
				{
					if (ImGui::Selectable(m_pairSelectedModel.second->Get_Mesh(i)->Get_Name(), iSelectedItem == iItemIdx))
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
	}
#pragma endregion
#pragma region Meshes
	if (Function::InRange(iSelectedItem, iRangeAnimation, iRangeMesh))
	{
		ImGui::NewLine();
		ImGui::SeparatorText("Meshes");
	}
#pragma endregion
#pragma region Materials
	if (Function::InRange(iSelectedItem, iRangeMesh, iRangeMaterial))
	{
		ImGui::NewLine();
		ImGui::SeparatorText("Materials");

		static _int		iCurrentIdx(0);
		const _char*	szTextureTypes[]	= { "Diffuse", "Normal" };
		const _char*	szPreview			= szTextureTypes[iCurrentIdx];
		if (ImGui::TreeNodeEx(string("Material " + std::to_string(iSelectedMaterial)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginCombo("Type", szPreview))
			{
				for (int i = 0; i < IM_ARRAYSIZE(szTextureTypes); ++i)
				{
					const _bool bSelected = iCurrentIdx == i;
					if (ImGui::Selectable(szTextureTypes[i], bSelected))
					{
						iSelectedTexture	= -1;
						iCurrentIdx			= i;
						szPreview			= szTextureTypes[i];
					}

					if (bSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			switch (iCurrentIdx)
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
			_float fButtonSpace1 = ImGui::GetStyle().ItemSpacing.x;
		
			ImGui::SameLine(fWindowWidth1 - 2.f * fButtonWidth1 - fButtonSpace1 - ImGui::GetStyle().ItemInnerSpacing.y);
			if (ImGui::Button("-", ImVec2(fButtonWidth1, fButtonWidth1)))
			{
				if (iSelectedTexture != -1)
				{
					if (SUCCEEDED(m_tSelectedMaterial.pMaterial[IDX(eTexType)]->Remove_ShaderResourceView(iSelectedTexture)))
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
			if (ImGui::Button("+", ImVec2(fButtonWidth1, fButtonWidth1)))
			{
				const _char* szFilters = "All files{.*},WIC files(*.png *.jpg *.jpeg){.png,.jpg,.jpeg},DDS files(*.dds){.dds}";
				ImGuiFileDialog::Instance()->OpenDialog(DIALOG_OPEN_TEXTURE, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
			}

			if (ImGui::BeginListBox(szPreview, ImVec2(-FLT_MIN, 0.f)))
			{
				iTextureIdx = 0;
				for (auto& strTexture : m_tSelectedMaterial.pMaterial[IDX(eTexType)]->Get_TextureFileName())
				{
					if (ImGui::Selectable(Function::ToString(strTexture).c_str(), iSelectedTexture == iTextureIdx))
					{
						iSelectedTexture = iTextureIdx;
					}

					++iTextureIdx;
				}
				ImGui::EndListBox();	
			}

			if (iSelectedTexture != -1)
			{
				ImVec2 imgSize;

				D3D11_TEXTURE2D_DESC tTexture2dDesc{};
				m_tSelectedMaterial.pMaterial[IDX(eTexType)]->Get_Texture2D(iSelectedTexture)->GetDesc(&tTexture2dDesc);

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

				ImGui::Image(m_tSelectedMaterial.pMaterial[IDX(eTexType)]->Get_ShaderResourceView(iSelectedTexture).Get(), imgSize);
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
#pragma region File Dialog

	if (ImGuiFileDialog::Instance()->Display(DIALOG_OPEN_TEXTURE))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(m_tSelectedMaterial.pMaterial[IDX(eTexType)]->Push_ShaderResourceView(Function::ToWString(ImGuiFileDialog::Instance()->GetFilePathName()))))
			{
				MSG_BOX("Scene_Tool::Info_Model", "Failed to Push ShaderResourceView");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion

	ImGui::End();
}

HRESULT CScene_Tool::Load_Model(const wstring& _strFilePath, const wstring& _strFileName, const MODEL _eType, _matrixf _mPivot)
{
	if (!Function::InRange(_eType, static_cast<MODEL>(0), MODEL::MAX))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Invalid MODEL");
	}

	shared_ptr<CModel> pModel = CModel::Create(m_pDevice, m_pContext, _eType, _strFilePath + wstring(TEXT("\\")) + _strFileName, _mPivot);

	if (nullptr == pModel)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Failed to Create");
	}

	switch (_eType)
	{
	case MODEL::NONANIM:
		m_mapNonAnimModels.emplace(Function::ToString(_strFileName), pModel);
		break;

	case MODEL::ANIM:
		m_mapAnimModels.emplace(Function::ToString(_strFileName), pModel);
		break;

	default:
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Invalid MODEL");
		break;
	}

	m_strModelPath = _strFilePath;
	
	return S_OK;
}

HRESULT CScene_Tool::Export_BinaryModel(const wstring& _strFilePath, const wstring& _strFileName)
{
	shared_ptr<CModel> pOut = m_pairSelectedModel.second;
	if (nullptr == pOut)
	{
		return S_FALSE;
	}

	std::ofstream fout(_strFilePath + wstring(TEXT("\\")) + _strFileName, std::ios::binary);
	if (!fout.is_open())
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_ModelBinary", "Failed to Open File");
	}

#pragma region Model
	const MODEL	eType			= pOut->Get_ModelType();
	const _uint	iNumBones		= pOut->Get_NumBones();
	const _uint	iNumAnimations	= pOut->Get_NumAnimations();
	const _uint	iNumMeshes		= pOut->Get_NumMeshes();
	const _uint	iNumMaterials	= pOut->Get_NumMaterials();

	fout.write(reinterpret_cast<const _byte*>(&eType),			sizeof(MODEL));
	fout.write(reinterpret_cast<const _byte*>(&iNumBones),		sizeof(_uint));
	fout.write(reinterpret_cast<const _byte*>(&iNumAnimations),	sizeof(_uint));
	fout.write(reinterpret_cast<const _byte*>(&iNumMeshes),		sizeof(_uint));
	fout.write(reinterpret_cast<const _byte*>(&iNumMaterials),	sizeof(_uint));

	if (fout.fail())
	{
		fout.clear();
		fout.close();
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_ModelBinary", "Failed to Write File");
	}
#pragma endregion
#pragma region Bones
	for (_uint i = 0; i < iNumBones; ++i)
	{
		shared_ptr<CBone>	pBone			= pOut->Get_Bone(i);
		const _uint			iParrentBoneIdx	= pBone->Get_ParentBoneIndex();
		const _float4x4		mTransformation	= pBone->Get_Transformation();

		fout.write(pBone->Get_Name(),									MAX_PATH);
		fout.write(reinterpret_cast<const _byte*>(&iParrentBoneIdx),	sizeof(_uint));
		fout.write(reinterpret_cast<const _byte*>(&mTransformation),	sizeof(_float4x4));
	}

	if (fout.fail())
	{
		fout.clear();
		fout.close();
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_ModelBinary", "Failed to Write File");
	}
#pragma endregion
#pragma region Animations
	for (_uint i = 0; i < iNumAnimations; ++i)
	{
		shared_ptr<CAnimation>		pAnimation		= pOut->Get_Animation(i);
		const _float 				fDuration		= pAnimation->Get_Duration();
		const _float 				fTicksPerSecond	= pAnimation->Get_TicksPerSecond();
		const _uint					iNumChannels	= pAnimation->Get_NumChannels();

		fout.write(pAnimation->Get_Name(),								MAX_PATH);
		fout.write(reinterpret_cast<const _byte*>(&fDuration),			sizeof(_float));
		fout.write(reinterpret_cast<const _byte*>(&fTicksPerSecond),	sizeof(_float));
		fout.write(reinterpret_cast<const _byte*>(&iNumChannels),		sizeof(_uint));
#pragma region Channels
		for (_uint j = 0; j < iNumChannels; ++j)
		{
			shared_ptr<CChannel>	pChannel		= pAnimation->Get_Channel(j);
			const _uint				iBoneIndex		= pChannel->Get_BoneIndex();
			const _uint				iNumKeyFrames	= pChannel->Get_NumKeyFrames();

			fout.write(reinterpret_cast<const _byte*>(&iBoneIndex),		sizeof(_uint));
			fout.write(reinterpret_cast<const _byte*>(&iNumKeyFrames),	sizeof(_uint));
#pragma region KeyFrames
			for (_uint k = 0; k < iNumKeyFrames; ++k)
			{
				const KEYFRAME		tKeyFrame		= pChannel->Get_KeyFrame(k);
				fout.write(reinterpret_cast<const _byte*>(&tKeyFrame),	sizeof(KEYFRAME));
			}
#pragma endregion
		}
#pragma endregion
	}

	if (fout.fail())
	{
		fout.clear();
		fout.close();
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_ModelBinary", "Failed to Write File");
	}
#pragma endregion
#pragma region Meshes
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		shared_ptr<CMesh>					pMesh				= pOut->Get_Mesh(i);
		const _float4x4						mPivot				= pMesh->Get_Pivot();
		const _uint							iMaterialIndex		= pMesh->Get_MaterialIndex();
		const _uint							iNumVertexBuffers	= pMesh->Get_NumVertexBuffers();
		const _uint							iNumVertices		= pMesh->Get_NumVertices();
		const _uint							iNumIndices			= pMesh->Get_NumIndices();

		fout.write(pMesh->Get_Name(),											MAX_PATH);
		fout.write(reinterpret_cast<const _byte*>(&mPivot),						sizeof(_float4x4));
		fout.write(reinterpret_cast<const _byte*>(&iMaterialIndex),				sizeof(_uint));
		fout.write(reinterpret_cast<const _byte*>(&iNumVertexBuffers),			sizeof(_uint));
		fout.write(reinterpret_cast<const _byte*>(&iNumVertices),				sizeof(_uint));
		fout.write(reinterpret_cast<const _byte*>(&iNumIndices),				sizeof(_uint));
		fout.write(reinterpret_cast<const _byte*>(pMesh->Get_Vertices().first),	sizeof(_float3)	* iNumVertices);
		fout.write(reinterpret_cast<const _byte*>(pMesh->Get_Indices().first),	sizeof(_uint)	* iNumIndices);

		switch (eType)
		{
		case MODEL::NONANIM:
		{
			fout.write(reinterpret_cast<const _byte*>(pMesh->Get_Vertices_NonAnim().first), sizeof(VTXMESH)	* iNumVertices);
		}
		break;
		case MODEL::ANIM:
		{
			const _uint	iNumBones	= pMesh->Get_NumBones();

			fout.write(reinterpret_cast<const _byte*>(&iNumBones), sizeof(_uint));
			fout.write(reinterpret_cast<const _byte*>(pMesh->Get_BoneIndices().first),		sizeof(_uint)		* iNumBones);
			fout.write(reinterpret_cast<const _byte*>(pMesh->Get_BoneOffsets().first),		sizeof(_float4x4)	* iNumVertices);
			fout.write(reinterpret_cast<const _byte*>(pMesh->Get_Vertices_Anim().first),	sizeof(VTXMESHANIM)	* iNumVertices);
		}
		break;
		};
	}

	if (fout.fail())
	{
		fout.clear();
		fout.close();
		MSG_RETURN(E_FAIL, "CScene_Tool::Export_ModelBinary", "Failed to Write File");
	}
#pragma endregion

	fout.close();

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
#endif
