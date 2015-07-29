#pragma once
#include "../GameTool/GameHeadConfig.h"
#include "Entity.h"
#include "SafeTraverseSet.h"
#include "EntityConfig.h"
//实体对象管理
// 对象创建接口原型
typedef Entity* (*EntityCreator)(uint entityId, const string& entityName, EntityCreateOpt* createOpt);
// 对象删除接口
typedef void(*EntityDeleter)(Entity* entity);

// 对象的创建和释放接口
struct SEntityAlloc
{
	EntityCreator	m_EntityCreator;	// 对象创建接口
	EntityDeleter	m_EntityDeleter;	// 对象删除接口,置空表示使用delete

	SEntityAlloc()
	{
		m_EntityCreator = NULL;
		m_EntityDeleter = NULL;
	}
};

// 对象管理 
class EntityMgr
{
public:
	typedef UNORDERED_MAP<uint, Entity*>		EntityMap;
	typedef map<uint8, SEntityAlloc>				EntityAllocMap;
	typedef set<Entity*>						EntitySet;
protected:
	EntityAllocMap				m_EntityAllocs;		// 对象分配
	EntityMap					m_Entitys;			// 对象表
	SafeTraverseSet<Entity*>	m_DeleteEntitys;	// 准备删除的对象
	map<uint8, uint>			m_SerialIds;
public:
	EntityMgr();
	virtual ~EntityMgr();

	SINGLETON_MODE(EntityMgr);

	// 生成对象唯一id,id中包含了对象类型
	// ((uint64)EntityType<<56) | (((uint64)serverId)<<40)) | (++SerialId)
	// EntityType:[0,255] serverId:[1,65535] SerialId:[1,2^40]
	uint GetNextEntityId(uint8 entityType);
	uint8 GetEntityType(uint entityId);

	// 从entityId中获得序列号
	static uint GetSerialIdFromEntityId(uint entityId);

	// 初始化序列号
	void InitSerialId(uint8 entityType, uint serialId);
	uint GetSerialId(uint8 entityType);

	// 查找对象
	Entity* GetEntityById(uint entityId);
	Entity* GetEntityByIdStr(const char* strEntityId);

	// 查找对象
	template<class T>
	T* GetEntity(uint entityId)
	{
		return dynamic_cast<T*>(GetEntityById(entityId));
	}

	// 查找对象
	template<class T>
	T* GetEntity(uint entityId, uint8 entityType)
	{
		Entity* object = GetEntityById(entityId);
		if (object && object->GetEntityType() == entityType)
			return (T*)object;
		return NULL;
	}

	// 创建对象
	Entity* CreateEntity(uint8 entityType, const string& objectName, EntityCreateOpt* createOpt);

	// 移除对象
	void RemoveEntity(Entity* entity);
	void RemoveEntity(uint entityId);

	// 注册对象创建接口
	void RegisterEntityAlloc(uint8 entityType, SEntityAlloc entityAlloc);
	SEntityAlloc* GetEntityAlloc(uint8 entityType);

	// 更新:删除需要删除的实体对象
	void Update(uint delta);

	// 释放
	void Shutdown();

	// 实体数量
	uint GetEntityCount() { return (uint)m_Entitys.size(); }
	// 获取每类实体的数量信息 key:EntityType value:count
	void GetEntityInfo(std::map<uint, uint>& info);
	// 实体列表
	const EntityMgr::EntityMap& GetEntityMap() const { return m_Entitys; }
};