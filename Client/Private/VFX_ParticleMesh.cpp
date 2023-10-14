#include "ClientPCH.h"
#include "VFX_ParticleMesh.h"
#include "GameInstance.h"

CVFX_ParticleMesh::CVFX_ParticleMesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffect(_pDevice, _pContext)
{
}

CVFX_ParticleMesh::CVFX_ParticleMesh(const CVFX_ParticleMesh& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CVFX_ParticleMesh::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESH, g_aNull);

	return S_OK;
}

HRESULT CVFX_ParticleMesh::Initialize(any _wstrPath)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Initialize", "Failed to __super::Initialize");
	}

	if (_wstrPath.has_value())
	{
		if (FAILED(Read(any_cast<wstring>(_wstrPath))))
		{
			MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Initialize", "Failed to Read");
		}

		if (!m_wstrVIInstMeshTag.empty())
		{
			Add_Component(COMPONENT::VIBUFFER_INSTANCE_MESH, CGameInstance::Get_Instance()->Clone_Component(
				CGameInstance::Get_Instance()->Current_Scene(), m_wstrVIInstMeshTag));
		}
		if (!m_wstrTexDiffusePath.empty())
		{
			m_pTexDiffuse = CTexture::Create(m_pDevice, m_pContext, m_wstrTexDiffusePath);
		}
		if (!m_wstrTexNormalPath.empty())
		{
			m_pTexNormal = CTexture::Create(m_pDevice, m_pContext, m_wstrTexNormalPath);
		}
	}

	m_pTransform->Set_Scale(_float3(3.f, 3.f, 3.f));

	return S_OK;
}

void CVFX_ParticleMesh::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (!m_bTimeLock)
	{
		m_fTimeAcc += _fTimeDelta;
	}
}

void CVFX_ParticleMesh::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CVFX_ParticleMesh::Render()
{
	if (m_pTexDiffuse)
	{
		if (FAILED(m_pTexDiffuse->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Render", "Failed to Bind_ShaderResourceViews");
		}
	}
	if (m_pTexNormal)
	{
		if (FAILED(m_pTexNormal->Bind_ShaderResourceViews(m_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL)))
		{
			MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Render", "Failed to Bind_ShaderResourceViews");
		}
	}

	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CVFX_ParticleMesh::Fetch(any _vPosition3)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Fetch", "Failed to __super::Fetch");
	}

	if (_vPosition3.has_value())
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, any_cast<_float3>(_vPosition3));
	}
	else
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, _float3(0.f, 0.f, 0.f));
	}

	m_fTimeAcc = 0.f;

	return S_OK;
}

_bool CVFX_ParticleMesh::Return()
{
	switch (m_eType)
	{
	case TYPE::BOUNCE:
	{
		if (m_fTotalBounceTime < m_fTimeAcc)
		{
			return true;
		}
	}
	break;
	}

	return false;
}

void CVFX_ParticleMesh::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	VTXPOSNORTANTEXINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSNORTANTEXINSTTRANSCOLORARG*>(_pData);

	for (_uint i = 0; i < m_iActivateInstances; ++i)
	{
		memcpy(&pData[i], &m_vecBounceDesc[i].mInitialTransformation, sizeof(_float4x4));
	}
}

void CVFX_ParticleMesh::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXPOSNORTANTEXINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSNORTANTEXINSTTRANSCOLORARG*>(_pData);

	switch (m_eType)
	{
	case TYPE::BOUNCE:
	{
		for (_uint i = 0; i < m_iActivateInstances; ++i)
		{
			if (m_fTimeAcc < m_vecBounceDesc[i].fBeginDuration)
			{
				_float4x4 m = Function::Lerp(m_vecBounceDesc[i].mInitialTransformation, m_vecBounceDesc[i].mPeakTransformation,
					m_fTimeAcc / m_vecBounceDesc[i].fBeginDuration, m_vecBounceDesc[i].fBeginWeight);
				memcpy(&pData[i], &m, sizeof(_float4x4));
			}
			else if(m_fTimeAcc < m_vecBounceDesc[i].fBeginDuration + m_vecBounceDesc[i].fPeakDuration)
			{
				memcpy(&pData[i], &m_vecBounceDesc[i].mPeakTransformation, sizeof(_float4x4));
			}
			else if (m_fTimeAcc < m_vecBounceDesc[i].fBeginDuration + m_vecBounceDesc[i].fPeakDuration + m_vecBounceDesc[i].fEndDuration)
			{
				_float4x4 m = Function::Lerp(m_vecBounceDesc[i].mPeakTransformation, m_vecBounceDesc[i].mFinalTransformation,
					(m_fTimeAcc - m_vecBounceDesc[i].fBeginDuration - m_vecBounceDesc[i].fPeakDuration) / (m_vecBounceDesc[i].fEndDuration), m_vecBounceDesc[i].fEndWeight);
				memcpy(&pData[i], &m, sizeof(_float4x4));
			}
			else
			{
				memcpy(&pData[i], &m_vecBounceDesc[i].mFinalTransformation, sizeof(_float4x4));
			}
		}
	}
	break;
	}
}

#if ACTIVATE_TOOL
shared_ptr<CTexture> CVFX_ParticleMesh::Get_Texture(aiTextureType _eType) const
{
	switch (_eType)
	{
	case aiTextureType_DIFFUSE:
		return m_pTexDiffuse;
	case aiTextureType_NORMALS:
		return m_pTexNormal;
	default:
		MSG_RETURN(nullptr, "CVFX_ParticleMesh::Get_Texture", "Invalid Texture Type");
	}

	return nullptr;
}
wstring CVFX_ParticleMesh::Get_TexturePath(aiTextureType _eType) const
{
	switch (_eType)
	{
	case aiTextureType_DIFFUSE:
		return m_wstrTexDiffusePath;
	case aiTextureType_NORMALS:
		return m_wstrTexDiffusePath;
	default:
		MSG_RETURN(wstring(); , "CVFX_ParticleMesh::Get_Texture", "Invalid Texture Type");
	}

	return wstring();
}
HRESULT CVFX_ParticleMesh::Set_TexturePath(aiTextureType _eType, const wstring& _wstrPath)
{
	switch (_eType)
	{
	case aiTextureType_DIFFUSE:
		m_wstrTexDiffusePath = _wstrPath;
		m_pTexDiffuse = CTexture::Create(m_pDevice, m_pContext, m_wstrTexDiffusePath);
		break;
	case aiTextureType_NORMALS:
		m_wstrTexNormalPath = _wstrPath;
		m_pTexNormal = CTexture::Create(m_pDevice, m_pContext, m_wstrTexNormalPath);
		break;
	default:
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Set_TexturePath", "Invalid Texture Type");
	}

	return S_OK;
}
#endif

void CVFX_ParticleMesh::Set_ActivateInstances(_uint _iNumInstance)
{
	__super::Set_ActivateInstances(_iNumInstance);

	switch (m_eType)
	{
	case TYPE::BOUNCE:
	{
		m_vecBounceDesc.resize(_iNumInstance);
	}
	break;
	}
}

HRESULT CVFX_ParticleMesh::Add_Component(const COMPONENT _eComponent, shared_ptr<CComponent> _pComponent)
{
	if (FAILED(__super::Add_Component(_eComponent, _pComponent)))
	{
		MSG_RETURN(E_FAIL, "CFVX_ParticleMesh::Add_Component", "Failed to __super::Add_Component");
	}

	m_vecBounceDesc.resize(m_iActivateInstances);

	Update();

	return S_OK;
}

void CVFX_ParticleMesh::Update()
{
	switch (m_eType)
	{
	case TYPE::BOUNCE:
	{
		_float fTotalTime = 0.f;
		for (auto& desc : m_vecBounceDesc)
		{
			fTotalTime = desc.fBeginDuration + desc.fPeakDuration + desc.fEndDuration;
			if (m_fTotalBounceTime < fTotalTime)
			{
				m_fTotalBounceTime = fTotalTime;
			}
		}
	}
	break;
	}
}

shared_ptr<CVFX_ParticleMesh> CVFX_ParticleMesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CVFX_ParticleMesh> pInstance = make_private_shared(CVFX_ParticleMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CVFX_ParticleMesh::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CVFX_ParticleMesh::Clone(any _wstrPath)
{
	shared_ptr<CVFX_ParticleMesh> pInstance = make_private_shared_copy(CVFX_ParticleMesh, *this);

	if (FAILED(pInstance->Initialize(_wstrPath)))
	{
		MSG_RETURN(nullptr, "CVFX_ParticleMesh::Clone", "Failed to Initialize");
	}

	return pInstance;
}

HRESULT CVFX_ParticleMesh::Read(const wstring& _wstrPath)
{
	std::ifstream inFile(_wstrPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Read", "Failed to Open File");
	}

#pragma region Import

	inFile.read(reinterpret_cast<_byte*>(&m_eType),					sizeof(TYPE));

	size_t nPathLength(0);
	_wchar wszBuffer[MAX_PATH];

	inFile.read(reinterpret_cast<_byte*>(&nPathLength),				sizeof(size_t));
	if (nPathLength)
	{
		ZeroMemory(wszBuffer, sizeof(_wchar) * MAX_PATH);
		inFile.read(reinterpret_cast<_byte*>(wszBuffer),			sizeof(_wchar) * nPathLength);
		m_wstrVIInstMeshTag = wszBuffer;
	}

	inFile.read(reinterpret_cast<_byte*>(&nPathLength),				sizeof(size_t));
	if (nPathLength)
	{
		ZeroMemory(wszBuffer, sizeof(_wchar) * MAX_PATH);
		inFile.read(reinterpret_cast<_byte*>(wszBuffer),			sizeof(_wchar) * nPathLength);
		m_wstrTexDiffusePath = wszBuffer;
	}

	inFile.read(reinterpret_cast<_byte*>(&nPathLength),				sizeof(size_t));
	if (nPathLength)
	{
		ZeroMemory(wszBuffer, sizeof(_wchar) * MAX_PATH);
		inFile.read(reinterpret_cast<_byte*>(wszBuffer),			sizeof(_wchar) * nPathLength);
		m_wstrTexNormalPath = wszBuffer;
	}

	size_t nBounceDescSize(0);
	inFile.read(reinterpret_cast<_byte*>(&nBounceDescSize),			sizeof(size_t));
	m_vecBounceDesc.resize(nBounceDescSize);
	inFile.read(reinterpret_cast<_byte*>(m_vecBounceDesc.data()),	sizeof(BOUNCEDESC) * nBounceDescSize);

#pragma endregion

	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Read", "Failed to Read File");
	}

	inFile.close();

	return S_OK;
}

#if ACTIVATE_TOOL
HRESULT CVFX_ParticleMesh::Export(const wstring& _wstrPath)
{
	std::ofstream outFile(_wstrPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Export", "Failed to Open File");
	}

#pragma region Export

	outFile.write(reinterpret_cast<const _byte*>(&m_eType),							sizeof(TYPE));

	size_t nPathLength(0);
	
	nPathLength = m_wstrVIInstMeshTag.length();
	outFile.write(reinterpret_cast<const _byte*>(&nPathLength),						sizeof(size_t));
	if (nPathLength)
	{
		outFile.write(reinterpret_cast<const _byte*>(m_wstrVIInstMeshTag.c_str()), sizeof(_wchar) * nPathLength);
	}

	nPathLength = m_wstrTexDiffusePath.length();
	outFile.write(reinterpret_cast<const _byte*>(&nPathLength),						sizeof(size_t));
	if (nPathLength)
	{
		outFile.write(reinterpret_cast<const _byte*>(m_wstrTexDiffusePath.c_str()), sizeof(_wchar) * nPathLength);
	}

	nPathLength = m_wstrTexNormalPath.length();
	outFile.write(reinterpret_cast<const _byte*>(&nPathLength),						sizeof(size_t));
	if (nPathLength)
	{
		outFile.write(reinterpret_cast<const _byte*>(m_wstrTexNormalPath.c_str()),	sizeof(_wchar) * nPathLength);
	}

	size_t nBounceDescSize = m_vecBounceDesc.size();
	outFile.write(reinterpret_cast<const _byte*>(&nBounceDescSize),					sizeof(size_t));
	outFile.write(reinterpret_cast<const _byte*>(m_vecBounceDesc.data()),			sizeof(BOUNCEDESC) * nBounceDescSize);

#pragma endregion

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CVFX_ParticleMesh::Export", "Failed to Write File");
	}

	outFile.close();

	return S_OK;
}
#endif
