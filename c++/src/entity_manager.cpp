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