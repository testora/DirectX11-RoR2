#pragma once
#pragma warning(push)
#pragma warning(disable:4275)

namespace Engine
{
	typedef DirectX::XMINT2		_int2;
	typedef DirectX::XMINT3		_int3;
	typedef DirectX::XMINT4		_int4;

	typedef DirectX::XMUINT2	_uint2;
	typedef DirectX::XMUINT3	_uint3;
	typedef DirectX::XMUINT4	_uint4;

	typedef struct ENGINE_DLL	float2 : public DirectX::XMFLOAT2
	{
		float2() : XMFLOAT2{} {}
		float2(_float _x, _float _y) : XMFLOAT2(_x, _y) {}

		float2(XMFLOAT2 _rhs) : XMFLOAT2(_rhs) {}
		float2(XMVECTOR _vector) { XMStoreFloat2(this, _vector); }

		_float length() const noexcept;
		float2 normalize() const noexcept;

		// assignment operators
		float2& operator = (const XMVECTOR&) noexcept;

		float2& operator += (const XMVECTOR&) noexcept;
		float2& operator -= (const XMVECTOR&) noexcept;
		float2& operator *= (const XMVECTOR&) noexcept;
		float2& operator /= (const XMVECTOR&) noexcept;

		float2& operator += (const float2&) noexcept;
		float2& operator -= (const float2&) noexcept;
		float2& operator *= (const float2&) noexcept;
		float2& operator /= (const float2&) noexcept;

		float2& operator *= (float) noexcept;
		float2& operator /= (float) noexcept;

		// unary operators
		float2 operator + () const noexcept;
		float2 operator - () const noexcept;

		// binary operators
		float2 operator + (const XMVECTOR&) const noexcept;
		float2 operator - (const XMVECTOR&) const noexcept;
		float2 operator * (const XMVECTOR&) const noexcept;
		float2 operator / (const XMVECTOR&) const noexcept;

		float2 operator + (const float2&) const noexcept;
		float2 operator - (const float2&) const noexcept;
		float2 operator * (const float2&) const noexcept;
		float2 operator / (const float2&) const noexcept;

		float2 operator * (float) const noexcept;
		float2 operator / (float) const noexcept;

		_bool operator == (const XMVECTOR&) const noexcept;
		_bool operator != (const XMVECTOR&) const noexcept;
		_bool operator == (const float2&) const noexcept;
		_bool operator != (const float2&) const noexcept;

		operator XMVECTOR() const noexcept;
	} _float2;

	typedef struct ENGINE_DLL	float3 : public DirectX::XMFLOAT3
	{
		float3() : XMFLOAT3{} {}
		float3(_float _x, _float _y, _float _z) : XMFLOAT3(_x, _y, _z) {}

		float3(XMFLOAT3 _rhs) : XMFLOAT3(_rhs) {}
		float3(XMVECTOR _vector) { XMStoreFloat3(this, _vector); }

		_float length() const noexcept;
		float3 normalize() const noexcept;

		// assignment operators
		float3& operator = (const XMVECTOR&) noexcept;

		float3& operator += (const XMVECTOR&) noexcept;
		float3& operator -= (const XMVECTOR&) noexcept;
		float3& operator *= (const XMVECTOR&) noexcept;
		float3& operator /= (const XMVECTOR&) noexcept;

		float3& operator += (const float3&) noexcept;
		float3& operator -= (const float3&) noexcept;
		float3& operator *= (const float3&) noexcept;
		float3& operator /= (const float3&) noexcept;

		float3& operator *= (float) noexcept;
		float3& operator /= (float) noexcept;

		// unary operators
		float3 operator + () const noexcept;
		float3 operator - () const noexcept;

		// binary operators
		float3 operator + (const XMVECTOR&) const noexcept;
		float3 operator - (const XMVECTOR&) const noexcept;
		float3 operator * (const XMVECTOR&) const noexcept;
		float3 operator / (const XMVECTOR&) const noexcept;

		float3 operator + (const float3&) const noexcept;
		float3 operator - (const float3&) const noexcept;
		float3 operator * (const float3&) const noexcept;
		float3 operator / (const float3&) const noexcept;

		float3 operator * (float) const noexcept;
		float3 operator / (float) const noexcept;

		_bool operator == (const XMVECTOR&) const noexcept;
		_bool operator != (const XMVECTOR&) const noexcept;
		_bool operator == (const float3&) const noexcept;
		_bool operator != (const float3&) const noexcept;

		operator XMVECTOR() const noexcept;
	} _float3;

	typedef struct ENGINE_DLL	float4 : public DirectX::XMFLOAT4
	{
		float4() : XMFLOAT4{} {}
		float4(_float _x, _float _y, _float _z, _float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		float4(XMFLOAT3 _float3, _float _w) : XMFLOAT4(_float3.x, _float3.y, _float3.z, _w) {}

		float4(XMFLOAT4 _rhs) : XMFLOAT4(_rhs) {}
		float4(XMVECTOR _vector) { XMStoreFloat4(this, _vector); }

		_float length() const noexcept;
		float4 normalize() const noexcept;

		// assignment operators
		float4& operator = (const XMVECTOR&) noexcept;

		float4& operator += (const XMVECTOR&) noexcept;
		float4& operator -= (const XMVECTOR&) noexcept;
		float4& operator *= (const XMVECTOR&) noexcept;
		float4& operator /= (const XMVECTOR&) noexcept;

		float4& operator += (const float4&) noexcept;
		float4& operator -= (const float4&) noexcept;
		float4& operator *= (const float4&) noexcept;
		float4& operator /= (const float4&) noexcept;

		float4& operator *= (float) noexcept;
		float4& operator /= (float) noexcept;

		// unary operators
		float4 operator + () const noexcept;
		float4 operator - () const noexcept;

		// binary operators
		float4 operator + (const XMVECTOR&) const noexcept;
		float4 operator - (const XMVECTOR&) const noexcept;
		float4 operator * (const XMVECTOR&) const noexcept;
		float4 operator / (const XMVECTOR&) const noexcept;

		float4 operator + (const float4&) const noexcept;
		float4 operator - (const float4&) const noexcept;
		float4 operator * (const float4&) const noexcept;
		float4 operator / (const float4&) const noexcept;

		float4 operator * (float) const noexcept;
		float4 operator / (float) const noexcept;

		_bool operator == (const XMVECTOR&) const noexcept;
		_bool operator != (const XMVECTOR&) const noexcept;
		_bool operator == (const float4&) const noexcept;
		_bool operator != (const float4&) const noexcept;

		operator XMVECTOR() const noexcept;
	} _float4, _quaternion, _color;

	typedef struct ENGINE_DLL	float4x4 : public DirectX::XMFLOAT4X4
	{
		float4x4() : XMFLOAT4X4{} {}
		float4x4(float4 _a, float4 _b, float4 _c, float4 _d) : XMFLOAT4X4(
			_a.x, _a.y, _a.z, _a.w,
			_b.x, _b.y, _b.z, _b.w,
			_c.x, _c.y, _c.z, _c.w,
			_d.x, _d.y, _d.z, _d.w
		) {}
		float4x4(XMFLOAT4X4 _float4x4) : XMFLOAT4X4(_float4x4) {}
		float4x4(XMMATRIX _matrix) { XMStoreFloat4x4(this, _matrix); }

		float4x4 inverse() const noexcept;
		float4 row(_uint _index) const noexcept;

		// assignment operators
		float4x4& operator = (const XMMATRIX&) noexcept;

		float4x4& operator += (const XMMATRIX&) noexcept;
		float4x4& operator -= (const XMMATRIX&) noexcept;
		float4x4& operator *= (const XMMATRIX&) noexcept;

		float4x4& operator += (const float4x4&) noexcept;
		float4x4& operator -= (const float4x4&) noexcept;
		float4x4& operator *= (const float4x4&) noexcept;

		float4x4& operator *= (float) noexcept;
		float4x4& operator /= (float) noexcept;

		// unary operators
		float4x4 operator + () const noexcept;
		float4x4 operator - () const noexcept;

		// binary operators
		float4x4 operator + (const XMMATRIX&) const noexcept;
		float4x4 operator - (const XMMATRIX&) const noexcept;
		float4x4 operator * (const XMMATRIX&) const noexcept;

		float4x4 operator + (const float4x4&) const noexcept;
		float4x4 operator - (const float4x4&) const noexcept;
		float4x4 operator * (const float4x4&) const noexcept;

		float4x4 operator * (float) const noexcept;
		float4x4 operator / (float) const noexcept;

		_bool operator == (const XMMATRIX&) const noexcept;
		_bool operator != (const XMMATRIX&) const noexcept;
		_bool operator == (const float4x4&) const noexcept;
		_bool operator != (const float4x4&) const noexcept;

		operator XMMATRIX() const noexcept;
	} _float4x4;

	typedef DirectX::XMVECTOR	_vector;
	typedef DirectX::FXMVECTOR	_vectorf;
	typedef DirectX::GXMVECTOR	_vectorg;
	typedef DirectX::HXMVECTOR	_vectorh;
	typedef DirectX::CXMVECTOR	_vectorc;
								
	typedef DirectX::XMMATRIX	_matrix;
	typedef DirectX::FXMMATRIX	_matrixf;
	typedef DirectX::CXMMATRIX	_matrixc;

	typedef array<float3, 3>	POLYGON;
}

#include "Engine_Typedef.inl"

#pragma warning(pop)
