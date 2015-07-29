#include "EntityComponent.h"

EntityComponent::EntityComponent(Entity*entity)
	: m_Entity(entity)
{
	m_IsDeleted = false;
}