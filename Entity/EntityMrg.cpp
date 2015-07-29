#include "EntityMrg.h"
#include "EntityConfig.h"
#include "GameLog.h"
#include "TypeConvert.h"
EntityMgr::EntityMgr()
{
	SEntityAlloc entityAlloc;
	entityAlloc.m_EntityCreator = Entity::Create;
	RegisterEntityAlloc(EEntityType_Entity, entityAlloc);
}
EntityMgr::~EntityMgr()
{

}

uint EntityMgr::GetNextEntityId(uint8 entityType)
{
	m_SerialIds[entityType]++;

	return (((uint32)entityType << 24) | m_SerialIds[entityType]);
}

uint8 EntityMgr::GetEntityType(uint entityId)
{
	return (uint8)(entityId >> 24);
}

uint EntityMgr::GetSerialIdFromEntityId(uint entityId)
{
	return ((entityId << 8) >> 8);
}

void EntityMgr::InitSerialId(uint8 entityType, uint serialId)
{
	m_SerialIds[entityType] = serialId;
}

uint EntityMgr::GetSerialId(uint8 entityType)
{
	map<uint8, uint>::iterator it = m_SerialIds.find(entityType);
	if (it == m_SerialIds.end())
		return 0;
	return it->second;
}

Entity* EntityMgr::GetEntityById(uint entityId)
{
	EntityMap::iterator it = m_Entitys.find(entityId);
	if (it == m_Entitys.end())
		return NULL;
	return it->second;
}

Entity* EntityMgr::GetEntityByIdStr(const char* strEntityId)
{
	if (!strEntityId)
		return NULL;
	return GetEntityById(TypeConvert::ToUint(strEntityId));
}

void EntityMgr::RemoveEntity(Entity* entity)
{
	if (entity == NULL)
		return;
	m_Entitys.erase(entity->GetEntityId());
	m_DeleteEntitys.Add(entity);
}

void EntityMgr::RemoveEntity(uint entityId)
{
	RemoveEntity(GetEntityById(entityId));
}

void EntityMgr::RegisterEntityAlloc(uint8 entityType, SEntityAlloc entityAlloc)
{
	// 允许覆盖
	m_EntityAllocs[entityType] = entityAlloc;
}

SEntityAlloc* EntityMgr::GetEntityAlloc(uint8 entityType)
{
	EntityAllocMap::iterator it = m_EntityAllocs.find(entityType);
	if (it == m_EntityAllocs.end())
		return NULL;
	return &(it->second);
}

Entity* EntityMgr::CreateEntity(uint8 entityType, const string& entityName, EntityCreateOpt* createOpt)
{
	SEntityAlloc* entityAlloc = GetEntityAlloc(entityType);
	if (entityAlloc == NULL || entityAlloc->m_EntityCreator == NULL)
	{
		//Assert(false);
		return NULL;
	}
	uint entityId = 0;
	if (createOpt != NULL && createOpt->m_EntityId > 0)
	{
		entityId = createOpt->m_EntityId;
	}
	else
	{
		entityId = GetNextEntityId(entityType);
	}
	Entity* existEntity = GetEntityById(entityId);
	if (existEntity != NULL)
	{
		GameLog("EntityMgr::CreateEntity exists entity");
		/*//Assert( false );
		uint existAppId = ((existEntity->GetEntityId()>>40)&0xFFFF);
		uint appId = ((entityId>>40)&0xFFFF);
		FmLog( "EntityMgr::CreateEntity exists entity:entityType=%d(%d) entityName=%s(%s) entityId=%llu(%llu) appId=%d(%d)",
		entityType, existEntity->GetEntityType(), entityName.c_str(), existEntity->GetEntityName().c_str(), entityId, existEntity->GetEntityId(),
		appId, existAppId );*/
		return NULL;
	}
	Entity* newEntity = entityAlloc->m_EntityCreator(entityId, entityName, createOpt);
	if (newEntity)
	{
		// 加入管理器
		m_Entitys[newEntity->GetEntityId()] = newEntity;
	}
	return newEntity;
}

void EntityMgr::Shutdown()
{
	EntityMap deleteList = m_Entitys;
	for (EntityMap::iterator it = deleteList.begin(); it != deleteList.end(); ++it)
	{
		RemoveEntity(it->first);
	}
	m_Entitys.clear();
	Update(0);
}

void EntityMgr::Update(uint delta)
{
	m_DeleteEntitys.BeforeTraverse();
	for (EntitySet::iterator it = m_DeleteEntitys.GetTraverseSet().begin(); it != m_DeleteEntitys.GetTraverseSet().end(); ++it)
	{
		Entity* entity = *it;
		m_DeleteEntitys.Remove(entity);
		SEntityAlloc* entityAlloc = GetEntityAlloc(entity->GetEntityType());
		if (entityAlloc && entityAlloc->m_EntityDeleter)
			entityAlloc->m_EntityDeleter(entity);
		else
			delete entity;
	}
}

void EntityMgr::GetEntityInfo(std::map<uint, uint>& info)
{
	for (EntityMap::iterator it = m_Entitys.begin(); it != m_Entitys.end(); ++it)
	{
		Entity* entity = it->second;
		if (entity)
		{
			info[entity->GetEntityType()]++;
		}
	}
}