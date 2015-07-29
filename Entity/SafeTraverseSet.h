#pragma once
//安全遍历的set
#include "../GameTool/GameHeadConfig.h"
template < class T >
class SafeTraverseSet
{
public:
	typedef set<T> SetClass;
protected:
	SetClass	m_Set;
	SetClass	m_TmpSet;
	bool		m_Changed;

public:
	SafeTraverseSet()
	{
		m_Changed = false;
	}

	void Add(T t)
	{
		m_Set.insert(t);
		m_Changed = true;
	}

	void Remove(T t)
	{
		m_Set.erase(t);
		m_Changed = true;
	}

	void BeforeTraverse()
	{
		if (m_Changed)
		{
			m_TmpSet = m_Set;
			m_Changed = false;
		}
	}

	SetClass& GetSet()
	{
		return m_Set;
	}

	SetClass& GetTraverseSet()
	{
		return m_TmpSet;
	}

	void Clear()
	{
		m_Set.clear();
		m_TmpSet.clear();
		m_Changed = false;
	}
};