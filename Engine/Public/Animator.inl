template<typename T>
T CAnimator::Current_Animation() const
{
	return static_cast<T>(Get_Animation());
}

template <typename T>
void CAnimator::Play_Animation(T _eAnimation, _float _fInterpolationDuration, _bool _bLoop)
{
	Set_Animation(IDX(_eAnimation), _fInterpolationDuration, _bLoop);
}

template <typename T>
void CAnimator::Play_Animation(T _eAnimation, T _eNextAnimation, _float _fInterpolationDuration, _float _fReservedInterpolationDuration, _bool _bReservedLoop)
{
	Set_Animation(IDX(_eAnimation), IDX(_eNextAnimation), _fInterpolationDuration, _fReservedInterpolationDuration, _bReservedLoop);
}
