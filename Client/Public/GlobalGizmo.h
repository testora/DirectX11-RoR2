#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBufferInstance_Rect;
END

BEGIN(Client)

class CGlobalGizmo final : public CGameObject
{
private:
	explicit CGlobalGizmo(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGlobalGizmo(const CGlobalGizmo&);
	virtual ~CGlobalGizmo() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Initialize_Gizmo(_int2 vGridSize);

public:
	static shared_ptr<CGlobalGizmo>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
