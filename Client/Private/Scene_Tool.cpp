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

#if ACTIVATE_TOOL

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

	m_pRenderer				= dynamic_pointer_cast<CRenderer>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Clone_Component: RENDERER");
	}

	m_pShader_NonAnimMesh	= dynamic_pointer_cast<CShader>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXMESH));
	m_pShader_AnimMesh		= dynamic_pointer_cast<CShader>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM));

	if (nullptr == m_pShader_NonAnimMesh
	||	nullptr == m_pShader_AnimMesh)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Clone_Component: SHADER");
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
	m_pRenderer->Add_RenderObject(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CScene_Tool::Render()
{
	return S_OK;
}

static _int iSelectedKeyFrame(0);

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

			const _char* szFilters = "Models (*.fbx, *.mdl){.fbx,.mdl},FBX (*.fbx){.fbx},Binary (*.mdl){.mdl},All files{.*}";
			m_imEmbed_Open.OpenDialog(DIALOG_OPEN_FBX, "Open Model", szFilters, "Bin/Resources/", 1, nullptr,
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
	
							_uint iCompareAnimation = 15;
	
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
				MSG_BOX("Scene_Tool::Info_Model", "Failed to Push ShaderResourceView");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion
	ImGui::End();
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
