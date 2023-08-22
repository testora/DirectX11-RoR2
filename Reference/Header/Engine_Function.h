#pragma once

namespace Function
{
	inline _float						Lerp(_float fStart, _float fEnd, _float fRatio);
	inline _float						Clamp(_float fMin, _float fMax, _float fValue);
	inline _float						ProportionalRatio(_float fMin, _float fMax, _float fValue);

	inline _bool						NearZero(_float fValue);
	inline _bool						NearZero3(XMVECTOR fVector3);
	inline _bool						NearZero4(XMVECTOR fVector4);

#pragma region Template

	template<typename T>
	void Swap(T& lhs, T& rhs)
	{
		T temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Min(T value)
	{
		return value;
	}
	template <typename T, typename... Args>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Min(T first, Args&&... args)
	{
		return min(first, Min(args...));
	}
	
	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Max(T value)
	{
		return value;
	}
	template <typename T, typename... Args>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Max(T first, Args&&... args)
	{
		return max(first, Max(args...));
	}

	template<typename T>
	typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, _bool>::type
	InRange(T value, T low, T high, const string& range = "[)")
	{
		_uint iValue	= static_cast<_uint>(value);
		_uint iLow		= static_cast<_uint>(low);
		_uint iHigh		= static_cast<_uint>(high);

		if (range == "()") {
			return iValue > iLow && iValue < iHigh;
		}
		else if (range == "(]") {
			return iValue > iLow && iValue <= iHigh;
		}
		else if (range == "[)") {
			return iValue >= iLow && iValue < iHigh;
		}
		else if (range == "[]") {
			return iValue >= iLow && iValue <= iHigh;
		}
		else
		{
			throw std::invalid_argument("Function::InRange: Invalid Range Option");
		}
	}

	template<typename T>
	vector<T*> ConvertToRawPtrVector(const vector<ComPtr<T>>& comPtrVector)
	{
		std::vector<T*> rawPtrVector;
		rawPtrVector.reserve(comPtrVector.size());

		for (const auto& comPtr : comPtrVector) {
			rawPtrVector.emplace_back(comPtr.Get());
		}

		return rawPtrVector;
	}

	template<typename T>
	unique_ptr<T[], std::default_delete<T[]>> CreateDynamicArray(size_t arrSize, _bool zeroMemory = true)
	{
		T* rawDynamicArray = new T[arrSize];
		if (zeroMemory) ZeroMemory(rawDynamicArray, sizeof(T) * arrSize);
		return move(unique_ptr<T[], std::default_delete<T[]>>(rawDynamicArray));
	}

#pragma endregion
}

namespace DirectX
{
	inline XMVECTOR QuaternionBetweenAxis(FXMVECTOR, FXMVECTOR);
}

#include "Engine_Function.inl"

template <typename T>
void Safe_Delete(T& _p)
{
	if (nullptr != _p)
	{
		delete _p;
		_p = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& _p)
{
	if (nullptr != _p)
	{
		delete[] _p;
		_p = nullptr;
	}
}
