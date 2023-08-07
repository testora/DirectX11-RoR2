#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CPicker final : public CSingleton<CPicker>
{
private:
	explicit CPicker();
	virtual ~CPicker() DEFAULT;

public:
	HRESULT		Initialize(HWND, ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	void		Tick();



	friend CSingleton<CPicker>;
};

END
