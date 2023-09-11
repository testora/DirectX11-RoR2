template<typename T>
T CAnimator::Current_Animation() const
{
	return static_cast<T>(Get_Animation());
}

template <typename T>
void CAnimator::Play_Animation(T _eAnimation, _float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop)
{
	Set_Animation(IDX(_eAnimation), _fPlaySpeed, _bReverse, _fInterpolationDuration, _bLoop);
}
