#pragma once
#include "../GameTool/GameHeadConfig.h"
//游戏事件
class GameEvent
{
protected:
	uint	m_EventId;	// 事件id
	uint	m_EntityId;	// 对象id
public:
	GameEvent(uint eventId, uint entityId)
		: m_EventId(eventId), m_EntityId(entityId)
	{
	}
	uint GetEventId() { return m_EventId; }
	uint GetEntityId() { return m_EntityId; }
};