#pragma once

//事件分发
#include "../GameTool/GameHeadConfig.h"
#include "EventFunctor.h"
#include "EntityComponent.h"
class EventDispatch
{
public:
	typedef std::vector<EventFunctor>		EventFunctorList;
	typedef map<uint, EventFunctorList>	EventFunctorMap;
protected:
	EventFunctorMap m_EventFunctors;	// 事件和函数对象的映射表
public:
	EventDispatch() {}

	void Register(uint eventId, EntityComponent* component, EventDelegate eventDelegate);

	void UnRegister(uint eventId);
	void UnRegisterComponent(EntityComponent* component);

	bool DispatchEvent(GameEvent* evt);
};