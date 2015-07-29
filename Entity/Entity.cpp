#include "Entity.h"
#include "EntityConfig.h"

Entity::Entity(uint8 entityType, uint entityId, const string& name)
{
	m_EntityType = entityType;
	m_EntityId = entityId;
	m_EntityName = name;
	m_Components = NULL;
	/*m_EventDispatch = NULL;
	m_Scheduler = NULL;*/
}
Entity::~Entity()
{
	if (m_Components != NULL)
	{
		for (ComponentMap::iterator it = m_Components->begin(); it != m_Components->end(); ++it)
		{
			delete it->second;
		}
		m_Components->clear();
		Game_SAFE_DELETE(m_Components);
	}
	/*Game_SAFE_DELETE(m_EventDispatch);
	Game_SAFE_DELETE(m_Scheduler);*/
}

Entity* Entity::Create(uint entityId, const string& entityName, EntityCreateOpt* createOpt)
{
	return new Entity(EEntityType_Entity, entityId, entityName);
}

const char* Entity::GetPropertyDescName() const
{
	if (m_PropertySet.GetDesc())
		return m_PropertySet.GetDesc()->GetName().c_str();
	return "";
}

EntityComponent* Entity::GetComponentById(uint componentId)
{
	if (m_Components == NULL)
		return NULL;
	ComponentMap::iterator it = m_Components->find(componentId);
	if (it == m_Components->end())
		return NULL;
	if (it->second->m_IsDeleted)
		return NULL;
	return it->second;
}

// 添加一个组件
void Entity::AddComponent(uint componentId, EntityComponent* component)
{
	if (m_Components == NULL)
	{
		m_Components = new ComponentMap();
	}
	EntityComponent* existComponent = NULL;
	ComponentMap::iterator it = m_Components->find(componentId);
	if (it != m_Components->end())
		existComponent = it->second;
	if (existComponent != NULL)
	{
		if (existComponent == component)
		{
			existComponent->m_IsDeleted = false;
			return;
		}
		//Assert(false);
		// delete existComponent;
	}
	(*m_Components)[componentId] = component;
}

// 移除一个组件
void Entity::RemoveComponent(uint componentId)
{
	if (m_Components == NULL)
		return;
	ComponentMap::iterator it = m_Components->find(componentId);
	if (it != m_Components->end())
	{
		it->second->m_IsDeleted = true;
	}
}
// 删除一个组件
void Entity::DeleteComponent(uint componentId)
{
	if (m_Components == NULL)
		return;
	ComponentMap::iterator it = m_Components->find(componentId);
	if (it != m_Components->end())
	{
		delete it->second;
		m_Components->erase(it);
	}
}

bool Entity::LoadPropertyNode(XmlElement* node, bool isLood)
{
	if (!node)
		return false;
	// 如果还没初始化
	if (m_PropertySet.GetDesc() == NULL)
	{
		const char* type = node->Attribute("Type");
		if (type)
		{
			if (!m_PropertySet.Init(type))
				return false;
		}
	}
	const char* name = node->Attribute("Name");
	if (name)
		m_EntityName = name;
	for (XmlElement* propertyNode = node->FirstChildElement("Property");
		propertyNode;
		propertyNode = propertyNode->NextSiblingElement("Property"))
	{
		const char* propertyName = propertyNode->Attribute("Name");
		if (!propertyName)
		{
			//Assert(false);
			return false;
		}
		Var propertyValue;
		if (!propertyValue.InitFromString(propertyNode->Attribute("Type"), propertyNode->Attribute("Value")))
		{
			//Assert(false);
			return false;
		}

		if (isLood || propertyNode->Attribute("Sync") == "")
		{
			SetValueByName(propertyName, propertyValue);
		}
	}
	XmlElement* extraNode = node->FirstChildElement("ExtraPropertys");
	if (extraNode)
	{
		for (XmlElement* propertyNode = extraNode->FirstChildElement("Property");
			propertyNode;
			propertyNode = propertyNode->NextSiblingElement("Property"))
		{
			const char* propertyName = propertyNode->Attribute("Name");
			if (!propertyName)
			{
				//Assert(false);
				return false;
			}
			Var propertyValue;
			if (!propertyValue.InitFromString(propertyNode->Attribute("Type"), propertyNode->Attribute("Value")))
			{
				//Assert(false);
				return false;
			}
			SetExtraValue(propertyName, propertyValue);
		}
	}
	return true;
}

bool Entity::InitPropertySet(Entity* dataEntity)
{
	if (dataEntity == NULL)
		return false;
	PropertyDesc* propertyDesc = dataEntity->GetPropertySet().GetDesc();
	if (propertyDesc == NULL)
		return false;
	for (uint i = 0; i < dataEntity->GetPropertySet().GetPropertyCount(); ++i)
	{
		if (GetPropertySet().GetDesc()->Has(propertyDesc->GetDesc(i)->m_Name))
		{
			GetPropertySet().SetValueByName(propertyDesc->GetDesc(i)->m_Name.c_str(), dataEntity->GetValueByName(propertyDesc->GetDesc(i)->m_Name));
		}
	}
	GetPropertySet().ResetChangedFlag();
	return true;
}

bool Entity::WritePropertyInit(Stream& stream, EPropertyFlag flag)
{
	PropertyDesc* propertyDesc = GetPropertySet().GetDesc();
	if (propertyDesc)
	{
		const vector<uint>* propertyIndexs = NULL;
		if (flag == EPropertyFlag_SyncSelf)
			propertyIndexs = &(propertyDesc->GetSyncSelfIndexs());
		else if (flag == EPropertyFlag_SyncScene)
			propertyIndexs = &(propertyDesc->GetSyncSceneIndexs());
		else
		{
			//Assert(false);
			return false;
		}
		// 暂时设定为不超过255个
		stream << (uint8)(flag);
		stream << (uint8)(propertyIndexs->size());
		if (propertyIndexs->size() > 0)
		{
			for (size_t i = 0; i < propertyIndexs->size(); ++i)
			{
				// todo: 只序列化Var的值,不序列化Var的类型
				stream << GetPropertySet().GetValue(propertyIndexs->at(i));
			}
		}
		return true;
	}
	return false;
}

bool Entity::ReadPropertyInit(Stream& stream)
{
	uint8 flag = 0;
	uint8 propertyCount = 0;
	stream >> flag >> propertyCount;
	PropertyDesc* propertyDesc = GetPropertySet().GetDesc();
	if (propertyDesc == NULL)
	{
		//Assert(false);
		return false;
	}
	const vector<uint>* propertyIndexs = NULL;
	if (flag == EPropertyFlag_SyncSelf)
		propertyIndexs = &(propertyDesc->GetSyncSelfIndexs());
	else if (flag == EPropertyFlag_SyncScene)
		propertyIndexs = &(propertyDesc->GetSyncSceneIndexs());
	else
	{
		//Assert(false);
		return false;
	}
	if (propertyCount > propertyIndexs->size())
	{
		//Assert(false);
		return false;
	}
	for (uint8 i = 0; i < propertyCount; ++i)
	{
		Var propertyValue;
		stream >> propertyValue;
		if (!stream.IsOK())
		{
			//Assert(false);
			return false;
		}
		uint propertyIndex = propertyIndexs->at(i);
		GetPropertySet().SetValue(propertyIndex, propertyValue);
	}
	return true;
}

//void Entity::Update(uint delta)
//{
//	if (m_Scheduler)
//		m_Scheduler->Update(delta);
//	// 删除组件
//	if (m_Components)
//	{
//		for (ComponentMap::iterator it = m_Components->begin(); it != m_Components->end();)
//		{
//			Component* component = it->second;
//			if (component->m_IsDeleted)
//			{
//				// 移除关联的事件
//				if (m_EventDispatch)
//					m_EventDispatch->UnRegisterComponent(component);
//				delete component;
//				m_Components->erase(it++);
//				continue;
//			}
//			else
//			{
//				++it;
//			}
//		}
//	}
//}

EventDispatch* Entity::GetEventDispatch()
{
	if (m_EventDispatch == NULL)
	{
		m_EventDispatch = new EventDispatch();
	}
	return m_EventDispatch;
}
//
//Scheduler* Entity::GetScheduler()
//{
//	if (m_Scheduler == NULL)
//	{
//		m_Scheduler = new Scheduler();
//		m_Scheduler->m_Entity = this;
//	}
//	return m_Scheduler;
//}