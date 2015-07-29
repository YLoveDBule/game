#include "EntityConfigData.h"
EntityConfigData::EntityConfigData(const char* propertyDescName)
{
	m_Entity = EntityMgr::GetInstance().CreateEntity(EEntityType_Entity, "", NULL);
	////Assert( propertyDescName );
	if (propertyDescName)
	{
		m_Entity->GetPropertySet().Init(propertyDescName);
	}
}

EntityConfigData::~EntityConfigData()
{
	if (m_Entity)
	{
		EntityMgr::GetInstance().RemoveEntity(m_Entity->GetEntityId());
		m_Entity = NULL;
	}
}

bool EntityConfigData::Load(XmlElement* entityNode)
{
	if (!entityNode)
		return false;
	if (!m_Entity->LoadPropertyNode(entityNode))
		return false;

	return true;
}