#pragma once
#include "GameEvent.h"
class Entity;
//实体对象的组件
class EntityComponent
{
	friend class Entity;
protected:
	Entity*	m_Entity;		// 关联的实体对象
	bool	m_IsDeleted;	// 删除标记
public:
	EntityComponent(Entity*	entity);
	virtual ~EntityComponent() {}

	Entity* GetEntity() { return m_Entity; }
	bool IsDeleted() const { return m_IsDeleted; }
	//实例对象模板
	template<class T>
	T* GetEntity()
	{
		return dynamic_cast<T*>(m_Entity);
	}
};

// 注册事件回调
#define REGISTER_EVENT( eventId, eventDelegate )\
	GetEntity()->GetEventDispatch()->Register( eventId, this, (EventDelegate)(&eventDelegate) )
