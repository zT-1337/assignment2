#include <iostream>
#include "entity_manager.h"

int main()
{
  EntityManager manager;

  std::cout << manager.getEntities().size() << std::endl;

  auto test_entity = manager.addEntity("test");
  std::cout << manager.getEntities().size() << std::endl;
  std::cout << manager.getEntities("test").size() << std::endl;

  manager.update();
  std::cout << manager.getEntities().size() << std::endl;
  std::cout << manager.getEntities("test").size() << std::endl;

  test_entity->destroy();
  manager.update();
  std::cout << manager.getEntities().size() << std::endl;
  std::cout << manager.getEntities("test").size() << std::endl;

  return 0;
}