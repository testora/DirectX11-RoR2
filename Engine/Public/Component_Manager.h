#pragma once
#include "Engine_Define.h"

#include "Renderer.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"

BEGIN(Engine)

class CComponent_Manager final : public CSingleton<CComponent_Manager>
{
private:
	explicit CComponent_Manager() DEFAULT;
	virtual ~CComponent_Manager() DEFAULT;

public:
	HRESULT													Reserve_Manager(const SCENE);

public:
	HRESULT													Add_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<CComponent> pPrototype);

	shared_ptr<CComponent>									Clone_Component(const SCENE, const wstring& strPrototypeTag, any = any());

private:
	shared_ptr<CComponent>									Find_Prototype(const SCENE, const wstring& strPrototypeTag);

private:
	typedef unordered_map<wstring, shared_ptr<CComponent>>	ComponentPrototype;
	unique_ptr<ComponentPrototype[]>						m_arrComponentPrototypes;

	friend CSingleton<CComponent_Manager>;
};

END
