#pragma once
//函数对象

#include "GameEvent.h"
#include "EntityComponent.h"
typedef bool (EntityComponent::*EventDelegate)(GameEvent*);
class EventFunctor
{
protected:
	EntityComponent*		m_Component;		// 组件
	EventDelegate	m_EventDelegate;	// 组件的成员函数
public:
	EventFunctor(EntityComponent* component, EventDelegate eventDelegate);

	EntityComponent* GetComponent() { return m_Component; }

	bool Invoke(GameEvent* evt);
};