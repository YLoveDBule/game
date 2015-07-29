#pragma once 
#include "../GameTool/GameHeadConfig.h"
class TickCounter
{
public:
	uint	m_RunningTick;	// 运行时间
	uint	m_ExpiredTick;	// 过期时刻,为0时,表示永不过期

public:
	TickCounter()
	{
		m_RunningTick = m_ExpiredTick = 0;
	}

	// 重置
	void Reset(uint expiredTick)
	{
		m_RunningTick = 0;
		m_ExpiredTick = expiredTick;
	}

	// 更新时间
	void Update(uint delta)
	{
		m_RunningTick += delta;
	}

	// 是否过期
	bool IsExpired()
	{
		if (m_ExpiredTick == 0)
			return false;
		return (m_RunningTick >= m_ExpiredTick);
	}

	// 是否过期,如果过期曾重置时间
	bool IsExpiredAndResetTick()
	{
		if (m_ExpiredTick == 0)
			return false;
		bool isExpire = false;
		if (m_RunningTick >= m_ExpiredTick)
		{
			isExpire = true;
			m_RunningTick -= m_ExpiredTick;
		}
		return isExpire;
	}

	// 是否过期,如果过期曾重置时间为0
	bool IsExpiredAndResetZero()
	{
		if (m_ExpiredTick == 0)
			return false;
		bool isExpire = false;
		if (m_RunningTick >= m_ExpiredTick)
		{
			isExpire = true;
			m_RunningTick = 0;
		}
		return isExpire;
	}
};