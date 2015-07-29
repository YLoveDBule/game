#include "EventDispatch.h"

void EventDispatch::Register(uint eventId, EntityComponent* component, EventDelegate eventDelegate)
{
	m_EventFunctors[eventId].push_back(EventFunctor(component, eventDelegate));
}

bool EventDispatch::DispatchEvent(GameEvent* evt)
{
	EventFunctorMap::iterator it = m_EventFunctors.find(evt->GetEventId());
	if (it == m_EventFunctors.end())
		return true;
	for (EventFunctorList::iterator itList = it->second.begin();
		itList != it->second.end(); ++itList)
	{
		EventFunctor& eventFunctor = *itList;
		if (!eventFunctor.GetComponent()->IsDeleted())
		{
			if (!eventFunctor.Invoke(evt))
				return false;
		}
	}
	return true;
}

void EventDispatch::UnRegister(uint eventId)
{
	EventFunctorMap::iterator it = m_EventFunctors.find(eventId);
	if (it != m_EventFunctors.end())
	{
		m_EventFunctors.erase(it);
	}
}

void EventDispatch::UnRegisterComponent(EntityComponent* component)
{
	if (!component)
		return;
	for (EventFunctorMap::iterator it = m_EventFunctors.begin(); it != m_EventFunctors.end(); ++it)
	{
		for (EventFunctorList::iterator itList = it->second.begin();
			itList != it->second.end();)
		{
			if ((*itList).GetComponent() == component)
			{
				itList = it->second.erase(itList);
				continue;
			}
			++itList;
		}
	}
}