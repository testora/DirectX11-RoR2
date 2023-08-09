#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer, public std::enable_shared_from_this<CMesh>
{
private:
	explicit CMesh(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CMesh() DEFAULT;

public:
	virtual HRESULT					Initialize(const aiMesh*, _matrixf mPivot);
	virtual HRESULT					Initialize(any pAIMesh) override;

	_uint							m_iMaterialIndex	= 0;

public:
	static shared_ptr<CMesh>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const aiMesh*, _matrixf mPivot);
	virtual shared_ptr<CComponent>	Clone(any = any()) override;

	friend class CModel;
};

END
