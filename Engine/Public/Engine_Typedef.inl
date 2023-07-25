namespace Engine
{
#pragma region _float2
#pragma region Assignment Operators

	__forceinline float2&
	float2::operator += (const float2& _v)
	{
		_vector v1 = XMLoadFloat2(this);
		_vector v2 = XMLoadFloat2(&_v);

		_vector result = v1 + v2;

		XMStoreFloat2(this, result);

		return *this;
	}

	__forceinline float2&
	float2::operator -= (const float2& _v)
	{
		_vector v1 = XMLoadFloat2(this);
		_vector v2 = XMLoadFloat2(&_v);

		_vector result = v1 - v2;

		XMStoreFloat2(this, result);

		return *this;
	}

	__forceinline float2&
	float2::operator *= (float _f)
	{
		_vector v = XMLoadFloat2(this);

		_vector result = XMVectorScale(v, _f);

		XMStoreFloat2(this, result);

		return *this;
	}

	__forceinline float2&
	float2::operator /= (float _f)
	{
		assert(_f != 0.f);

		_vector v = XMLoadFloat2(this);

		_vector result = v / _f;

		XMStoreFloat2(this, result);

		return *this;
	}

#pragma endregion
#pragma region Unary Operators

	__forceinline float2
	float2::operator + () const
	{
		return *this;
	}

	__forceinline float2
	float2::operator - () const
	{
		float2 out;

		_vector v = XMLoadFloat2(this);
		_vector result = XMVectorScale(v, -1.f);

		XMStoreFloat2(&out, result);

		return out;
	}

#pragma endregion
#pragma region Binary Operators

	__forceinline float2
	float2::operator + (const float2& _v) const
	{
		float2 out;

		_vector v1 = XMLoadFloat2(this);
		_vector v2 = XMLoadFloat2(&_v);

		XMStoreFloat2(&out, v1 + v2);

		return out;
	}

	__forceinline float2
	float2::operator - (const float2& _v) const
	{
		float2 out;

		_vector v1 = XMLoadFloat2(this);
		_vector v2 = XMLoadFloat2(&_v);

		XMStoreFloat2(&out, v1 - v2);

		return out;
	}

	__forceinline float2
	float2::operator * (float _f) const
	{
		float2 out;

		_vector v = XMLoadFloat2(this);
		_vector result = XMVectorScale(v, _f);

		XMStoreFloat2(&out, result);

		return out;
	}

	__forceinline float2
	float2::operator / (float _f) const
	{
		assert(_f != 0.f);

		float2 out;

		_vector v = XMLoadFloat2(this);

		_vector result = XMVectorScale(v, 1.f / _f);

		XMStoreFloat2(&out, result);

		return out;
	}

	__forceinline float2
	operator * (float _f, const float2& _v)
	{
		return _v * _f;
	}

	__forceinline _bool
	float2::operator == (const float2& _v) const
	{
		_vector v1 = XMLoadFloat2(this);
		_vector v2 = XMLoadFloat2(&_v);

		return XMVector2Equal(v1, v2);
	}

	__forceinline _bool
	float2::operator != (const float2& _v) const
	{
		_vector v1 = XMLoadFloat2(this);
		_vector v2 = XMLoadFloat2(&_v);

		return !XMVector2Equal(v1, v2);
	}

#pragma endregion
#pragma endregion
#pragma region _float3
#pragma region Assignment Operators

	__forceinline float3&
	float3::operator += (const float3& _v)
	{
		_vector v1 = XMLoadFloat3(this);
		_vector v2 = XMLoadFloat3(&_v);

		_vector result = v1 + v2;

		XMStoreFloat3(this, result);

		return *this;
	}

	__forceinline float3&
	float3::operator -= (const float3& _v)
	{
		_vector v1 = XMLoadFloat3(this);
		_vector v2 = XMLoadFloat3(&_v);

		_vector result = v1 - v2;

		XMStoreFloat3(this, result);

		return *this;
	}

	__forceinline float3&
	float3::operator *= (float _f)
	{
		_vector v = XMLoadFloat3(this);

		_vector result = XMVectorScale(v, _f);

		XMStoreFloat3(this, result);

		return *this;
	}

	__forceinline float3&
	float3::operator /= (float _f)
	{
		assert(_f != 0.f);

		_vector v = XMLoadFloat3(this);

		_vector result = v / _f;

		XMStoreFloat3(this, result);

		return *this;
	}

#pragma endregion
#pragma region Unary Operators

	__forceinline float3
	float3::operator + () const
	{
		return *this;
	}

	__forceinline float3
	float3::operator - () const
	{
		float3 out;

		_vector v = XMLoadFloat3(this);
		_vector result = XMVectorScale(v, -1.f);

		XMStoreFloat3(&out, result);

		return out;
	}

#pragma endregion
#pragma region Binary Operators

	__forceinline float3
	float3::operator + (const float3& _v) const
	{
		float3 out;

		_vector v1 = XMLoadFloat3(this);
		_vector v2 = XMLoadFloat3(&_v);

		XMStoreFloat3(&out, v1 + v2);

		return out;
	}

	__forceinline float3
	float3::operator - (const float3& _v) const
	{
		float3 out;

		_vector v1 = XMLoadFloat3(this);
		_vector v2 = XMLoadFloat3(&_v);

		XMStoreFloat3(&out, v1 - v2);

		return out;
	}

	__forceinline float3
	float3::operator * (float _f) const
	{
		float3 out;

		_vector v = XMLoadFloat3(this);
		_vector result = XMVectorScale(v, _f);

		XMStoreFloat3(&out, result);

		return out;
	}

	__forceinline float3
	float3::operator / (float _f) const
	{
		assert(_f != 0.f);

		float3 out;

		_vector v = XMLoadFloat3(this);

		_vector result = XMVectorScale(v, 1.f / _f);

		XMStoreFloat3(&out, result);

		return out;
	}

	__forceinline float3
	operator * (float _f, const float3& _v)
	{
		return _v * _f;
	}

	__forceinline _bool
	float3::operator == (const float3& _v) const
	{
		_vector v1 = XMLoadFloat3(this);
		_vector v2 = XMLoadFloat3(&_v);

		return XMVector3Equal(v1, v2);
	}

	__forceinline _bool
	float3::operator != (const float3& _v) const
	{
		_vector v1 = XMLoadFloat3(this);
		_vector v2 = XMLoadFloat3(&_v);

		return !XMVector3Equal(v1, v2);
	}

#pragma endregion
#pragma endregion
#pragma region _float4
#pragma region Assignment Operators

	__forceinline float4&
	float4::operator += (const float4& _v)
	{
		_vector v1 = XMLoadFloat4(this);
		_vector v2 = XMLoadFloat4(&_v);

		_vector result = v1 + v2;

		XMStoreFloat4(this, result);

		return *this;
	}

	__forceinline float4&
	float4::operator -= (const float4& _v)
	{
		_vector v1 = XMLoadFloat4(this);
		_vector v2 = XMLoadFloat4(&_v);

		_vector result = v1 - v2;

		XMStoreFloat4(this, result);

		return *this;
	}

	__forceinline float4&
	float4::operator *= (float _f)
	{
		_vector v = XMLoadFloat4(this);

		_vector result = XMVectorScale(v, _f);

		XMStoreFloat4(this, result);

		return *this;
	}

	__forceinline float4&
	float4::operator /= (float _f)
	{
		assert(_f != 0.f);

		_vector v = XMLoadFloat4(this);

		_vector result = v / _f;

		XMStoreFloat4(this, result);

		return *this;
	}

#pragma endregion
#pragma region Unary Operators

	__forceinline float4
	float4::operator + () const
	{
		return *this;
	}

	__forceinline float4
	float4::operator - () const
	{
		float4 out;

		_vector v = XMLoadFloat4(this);
		_vector result = XMVectorScale(v, -1.f);

		XMStoreFloat4(&out, result);

		return out;
	}

#pragma endregion
#pragma region Binary Operators

	__forceinline float4
	float4::operator + (const float4& _v) const
	{
		float4 out;

		_vector v1 = XMLoadFloat4(this);
		_vector v2 = XMLoadFloat4(&_v);

		XMStoreFloat4(&out, v1 + v2);

		return out;
	}

	__forceinline float4
	float4::operator - (const float4& _v) const
	{
		float4 out;

		_vector v1 = XMLoadFloat4(this);
		_vector v2 = XMLoadFloat4(&_v);

		XMStoreFloat4(&out, v1 - v2);

		return out;
	}

	__forceinline float4
	float4::operator * (float _f) const
	{
		float4 out;

		_vector v = XMLoadFloat4(this);
		_vector result = XMVectorScale(v, _f);

		XMStoreFloat4(&out, result);

		return out;
	}

	__forceinline float4
	float4::operator / (float _f) const
	{
		assert(_f != 0.f);

		float4 out;

		_vector v = XMLoadFloat4(this);

		_vector result = XMVectorScale(v, 1.f / _f);

		XMStoreFloat4(&out, result);

		return out;
	}

	__forceinline float4
	operator * (float _f, const float4& _v)
	{
		return _v * _f;
	}

	__forceinline _bool
	float4::operator == (const float4& _v) const
	{
		_vector v1 = XMLoadFloat4(this);
		_vector v2 = XMLoadFloat4(&_v);

		return XMVector4Equal(v1, v2);
	}

	__forceinline _bool
	float4::operator != (const float4& _v) const
	{
		_vector v1 = XMLoadFloat4(this);
		_vector v2 = XMLoadFloat4(&_v);

		return !XMVector4Equal(v1, v2);
	}

#pragma endregion
#pragma endregion
#pragma region _float4x4
#pragma region Assignment Operators

	__forceinline float4x4&
	float4x4::operator += (const float4x4& _m)
	{
		_matrix m1 = XMLoadFloat4x4(this);
		_matrix m2 = XMLoadFloat4x4(&_m);

		_matrix result = m1 + m2;

		XMStoreFloat4x4(this, result);

		return *this;
	}

	__forceinline float4x4&
	float4x4::operator -= (const float4x4& _m)
	{
		_matrix m1 = XMLoadFloat4x4(this);
		_matrix m2 = XMLoadFloat4x4(&_m);

		_matrix result = m1 - m2;

		XMStoreFloat4x4(this, result);

		return *this;
	}

	__forceinline float4x4&
	float4x4::operator *= (const float4x4& _m)
	{
		_matrix m1 = XMLoadFloat4x4(this);
		_matrix m2 = XMLoadFloat4x4(&_m);

		_matrix result = m1 * m2;

		XMStoreFloat4x4(this, result);

		return *this;
	}

	__forceinline float4x4&
	float4x4::operator *= (float _f)
	{
		_matrix m = XMLoadFloat4x4(this);

		_matrix result = m * _f;

		XMStoreFloat4x4(this, result);

		return *this;
	}

	__forceinline float4x4&
	float4x4::operator /= (float _f)
	{
		assert(_f != 0.f);

		_matrix m = XMLoadFloat4x4(this);

		_matrix result = m / _f;

		XMStoreFloat4x4(this, result);

		return *this;
	}

#pragma endregion
#pragma region Unary Operators

	__forceinline float4x4
	float4x4::operator + () const
	{
		return *this;
	}

	__forceinline float4x4
	float4x4::operator - () const
	{
		float4x4 out;

		_matrix m = XMLoadFloat4x4(this);
		_matrix result = m * -1.f;

		XMStoreFloat4x4(&out, result);

		return out;
	}

#pragma endregion
#pragma region Binary Operators
	
	__forceinline float4x4
	float4x4::operator + (const float4x4& _m) const
	{
		float4x4 out;

		_matrix m1 = XMLoadFloat4x4(this);
		_matrix m2 = XMLoadFloat4x4(&_m);

		_matrix result = m1 + m2;

		XMStoreFloat4x4(&out, result);

		return out;
	}
	__forceinline float4x4
	float4x4::operator - (const float4x4& _m) const
	{
		float4x4 out;

		_matrix m1 = XMLoadFloat4x4(this);
		_matrix m2 = XMLoadFloat4x4(&_m);

		_matrix result = m1 - m2;

		XMStoreFloat4x4(&out, result);

		return out;
	}
	__forceinline float4x4
	float4x4::operator * (const float4x4& _m) const
	{
		float4x4 out;

		_matrix m1 = XMLoadFloat4x4(this);
		_matrix m2 = XMLoadFloat4x4(&_m);

		_matrix result = XMMatrixMultiply(m1, m2);

		XMStoreFloat4x4(&out, result);
		
		return out;
	}
	
	__forceinline float4x4
	float4x4::operator * (float _f) const
	{
		float4x4 out;

		_matrix m = XMLoadFloat4x4(this);

		_matrix result = m * _f;

		XMStoreFloat4x4(&out, result);
		
		return out;
	}
	__forceinline float4x4
	float4x4::operator / (float _f) const
	{
		assert(_f != 0.f);

		float4x4 out;

		_matrix m = XMLoadFloat4x4(this);

		_matrix result = m / _f;
		
		XMStoreFloat4x4(&out, result);
		
		return out;
	}

	__forceinline _bool
	float4x4::operator == (const float4x4& _m) const
	{
		return !memcmp(this, &_m, sizeof(float4x4));
	}
	__forceinline _bool
	float4x4::operator != (const float4x4& _m) const
	{
		return memcmp(this, &_m, sizeof(float4x4));
	}

#pragma endregion
#pragma endregion
}