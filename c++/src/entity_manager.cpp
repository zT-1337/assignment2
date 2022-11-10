#include "entity_manager.h"

EntityManager::EntityManager() {}

void EntityManager::update()
{
  for(auto& e : m_entitiesToAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }

  m_entitiesToAdd.clear();

  removeDeadEntities(m_entities);
  for(auto& [tag, entities] : m_entityMap)
  {
    removeDeadEntities(entities);
  }

}

void EntityManager::removeDeadEntities(EntityVec & entities)
{
  EntityVec::iterator i = entities.begin();
  while(i != entities.end())
  {
    if((*i)->isActive())
    {
      ++i;
      continue;
    }

    entities.erase(i);
  }
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
  auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
  m_entitiesToAdd.push_back(e);

  return e;
}

const EntityVec& EntityManager::getEntities()
{
  return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string & tag)
{
  return m_entityMap[tag];
}