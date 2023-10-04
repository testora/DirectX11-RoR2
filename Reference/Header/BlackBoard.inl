template<typename T>
shared_ptr<T> CBlackBoard::Get_System(const wstring& _wstrKey)
{
	if (m_umapSystem.end() != m_umapSystem.find(_wstrKey))
	{
		return dynamic_pointer_cast<T>(m_umapSystem[_wstrKey]);
	}

	return nullptr;
}

template<typename T>
optional<T> CBlackBoard::Get_Anything(const wstring& _wstrKey)
{
	if (m_umapAnything.end() != m_umapAnything.find(_wstrKey))
	{
		return any_cast<T>(m_umapAnything[_wstrKey]);
	}

	return nullopt;
}
