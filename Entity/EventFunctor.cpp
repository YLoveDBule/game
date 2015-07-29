#include "EventFunctor.h"

EventFunctor::EventFunctor(EntityComponent* component, EventDelegate eventDelegate)
	: m_Component(component), m_EventDelegate(eventDelegate)
{
}

bool EventFunctor::Invoke(GameEvent* evt)
{
	if (m_Component && m_EventDelegate)
	{
		return (m_Component->*m_EventDelegate)(evt);
	}
	return false;
}