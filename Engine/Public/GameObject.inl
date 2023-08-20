template <typename T>
shared_ptr<T> CGameObject::Get_Component(COMPONENT _eComponent)
{
	if (Function::InRange(_eComponent, static_cast<COMPONENT>(0), COMPONENT::MAX))
	{
		return dynamic_pointer_cast<T>(m_umapComponent[_eComponent]);
	}

	return nullptr;
}

template <typename T>
shared_ptr<T> CGameObject::Get_Behavior(BEHAVIOR _eBehavior)
{
	if (Function::InRange(_eBehavior, static_cast<BEHAVIOR>(0), BEHAVIOR::MAX))
	{
		return dynamic_pointer_cast<T>(m_umapBehavior[_eBehavior]);
	}

	return nullptr;
}
