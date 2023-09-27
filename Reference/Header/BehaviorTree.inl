template <typename T>
shared_ptr<T> CBehaviorTree::Get_BlackBoard(const wstring& _wstrKey)
{
	if (m_umapBlackBoard.end() != m_umapBlackBoard.find(_wstrKey))
	{
		return dynamic_pointer_cast<T>(m_umapBlackBoard[_wstrKey]);
	}

	return nullptr;
}
