#include "EnginePCH.h"
#include "ImGui_Manager.h"

CImGui_Manager::~CImGui_Manager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT CImGui_Manager::Initialize(_In_ HWND _hWnd, ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	_bool bSuccess = true;

	bSuccess &= IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	bSuccess &= ImGui_ImplWin32_Init(_hWnd);
	bSuccess &= ImGui_ImplDX11_Init(_pDevice.Get(), _pContext.Get());
	ImGui::StyleColorsDark();

	return bSuccess ? S_OK : E_FAIL;
}

void CImGui_Manager::Tick()
{
	if (m_bEnable)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

#pragma region Text

		ImGui::Begin("Test");
		ImGui::End();

#pragma endregion
	}
}

void CImGui_Manager::Render()
{
	if (m_bEnable)
	{
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void CImGui_Manager::Enable()
{
	m_bEnable = true;
}

void CImGui_Manager::Disable()
{
	m_bEnable = false;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CImGui_Manager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_bEnable ? ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) : 0;
}
