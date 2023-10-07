#include "Animator.h"
template<typename T>
inline _bool CAnimator::Is_Playing(T _eAnimation) const
{
	return Is_Playing(IDX(_eAnimation));
}

template<typename T>
inline _bool CAnimator::Is_Finished(T _eAnimation) const
{
	return Is_Finished(IDX(_eAnimation));
}

template<typename T>
inline _float CAnimator::Get_Duration(T eAnimation) const
{
	return Get_Duration(IDX(eAnimation));
}

template<typename T>
inline void CAnimator::Set_Default(T _eAnimation)
{
	Set_DefaultAnimation(IDX(_eAnimation));
}

template <typename T>
void CAnimator::Play_Animation(T _eAnimation, _float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop)
{
	Set_Animation(IDX(_eAnimation), _fPlaySpeed, _bReverse, _fInterpolationDuration, _bLoop);
}

template<typename T>
inline void CAnimator::Fix_Animation(T _eAnimation, _float _fRatio)
{
	Fix_Animation(IDX(_eAnimation), _fRatio);
}

template<typename T>
inline void CAnimator::Blend_Animation(T eAnimation, _float fRatio)
{
	Blend_Animation(IDX(eAnimation), fRatio);
}
