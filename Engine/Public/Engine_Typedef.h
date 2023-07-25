#pragma once
#pragma warning(push)
#pragma warning(disable:4275)

namespace Engine
{
	typedef XMVECTOR			_vector;
	typedef FXMVECTOR			_vectorf;
	typedef GXMVECTOR			_vectorg;
	typedef HXMVECTOR			_vectorh;
	typedef CXMVECTOR			_vectorc;

	typedef XMMATRIX			_matrix;
	typedef FXMMATRIX			_matrixf;

	typedef struct ENGINE_DLL	float2 : public XMFLOAT2
	{
		float2() : XMFLOAT2{} {}
		float2(_float _x, _float _y) : XMFLOAT2(_x, _y) {}

		// assignment operators
		float2& operator += (const float2&);
		float2& operator -= (const float2&);
		float2& operator *= (float);
		float2& operator /= (float);

		// unary operators
		float2 operator + () const;
		float2 operator - () const;

		// binary operators
		float2 operator + (const float2&) const;
		float2 operator - (const float2&) const;
		float2 operator * (float) const;
		float2 operator / (float) const;

		_bool operator == (const float2&) const;
		_bool operator != (const float2&) const;
	} _float2;
	typedef struct ENGINE_DLL	float3 : public XMFLOAT3
	{
		float3() : XMFLOAT3{} {}
		float3(_float _x, _float _y, _float _z) : XMFLOAT3(_x, _y, _z) {}

		// assignment operators
		float3& operator += (const float3&);
		float3& operator -= (const float3&);
		float3& operator *= (float);
		float3& operator /= (float);

		// unary operators
		float3 operator + () const;
		float3 operator - () const;

		// binary operators
		float3 operator + (const float3&) const;
		float3 operator - (const float3&) const;
		float3 operator * (float) const;
		float3 operator / (float) const;

		_bool operator == (const float3&) const;
		_bool operator != (const float3&) const;
	} _float3;
	typedef struct ENGINE_DLL	float4 : public XMFLOAT4
	{
		float4() : XMFLOAT4{} {}
		float4(_float _x, _float _y, _float _z, _float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		float4(float3 _xyz, _float _w) : XMFLOAT4(_xyz.x, _xyz.y, _xyz.z, _w) {}

		// assignment operators
		float4& operator += (const float4&);
		float4& operator -= (const float4&);
		float4& operator *= (float);
		float4& operator /= (float);

		// unary operators
		float4 operator + () const;
		float4 operator - () const;

		// binary operators
		float4 operator + (const float4&) const;
		float4 operator - (const float4&) const;
		float4 operator * (float) const;
		float4 operator / (float) const;

		_bool operator == (const float4&) const;
		_bool operator != (const float4&) const;
	} _float4;
	typedef struct ENGINE_DLL	float4x4 : public XMFLOAT4X4
	{
		float4x4() : XMFLOAT4X4{} {}
		float4x4(float4 _a, float4 _b, float4 _c, float4 _d) : XMFLOAT4X4(
			_a.x, _a.y, _a.z, _a.w,
			_b.x, _b.y, _b.z, _b.w,
			_c.x, _c.y, _c.z, _c.w,
			_d.x, _d.y, _d.z, _d.w
		) {}

		// Assignment operators
		float4x4& operator += (const float4x4&);
		float4x4& operator -= (const float4x4&);
		float4x4& operator *= (const float4x4&);
		float4x4& operator *= (float);
		float4x4& operator /= (float);

		// unary operators
		float4x4 operator + () const;
		float4x4 operator - () const;

		// binary operators
		float4x4 operator + (const float4x4&) const;
		float4x4 operator - (const float4x4&) const;
		float4x4 operator * (const float4x4&) const;
		float4x4 operator * (float) const;
		float4x4 operator / (float) const;

		_bool operator == (const float4x4&) const;
		_bool operator != (const float4x4&) const;

	} _float4x4;
}

#include "Engine_Typedef.inl"

#pragma warning(pop)
