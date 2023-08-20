#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer, public std::enable_shared_from_this<CVIBuffer_Terrain>
{
private:
	explicit CVIBuffer_Terrain(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CVIBuffer_Terrain() DEFAULT;

public: 
	HRESULT									Initialize(any strHeightMapPath);

private:
	_float2									m_vNumVertices{};

public:
	static shared_ptr<CVIBuffer_Terrain>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& strHeightMapPath);
	virtual shared_ptr<CComponent>			Clone(any = any()) override;
public:
};

END
