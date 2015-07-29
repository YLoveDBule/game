#pragma once 
#include "Var.h"
#include "XmlWrapper.h"
#include "EntityMrg.h"
class  EntityConfigData
{
protected:
	Entity*		m_Entity;			// 实体,用来存放属性集
public:
	EntityConfigData(const char* propertyDescName);
	virtual ~EntityConfigData();

	// 获取实体
	Entity* GetEntity() const { return m_Entity; }

	// 加载数据
	virtual bool Load(XmlElement* entityNode);
};