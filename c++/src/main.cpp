#include <iostream>
#include "entity_manager.h"

int main()
{
  EntityManager manager;

  std::cout << manager.getEntities().size() << std::endl;

  auto test_entity0 = manager.addEntity("test");
  auto test_entity1 = manager.addEntity("test");
  auto test_entity2 = manager.addEntity("test");
  auto test_entity3 = manager.addEntity("based");
  std::cout << manager.getEntities().size() << std::endl;
  std::cout << manager.getEntities("test").size() << std::endl;

  manager.update();
  std::cout << manager.getEntities().size() << std::endl;
  std::cout << manager.getEntities("test").size() << std::endl;
  std::cout << manager.getEntities("based").size() << std::endl;

  test_entity0->destroy();
  test_entity2->destroy();
  test_entity3->destroy();
  manager.update();
  std::cout << manager.getEntities().size() << std::endl;
  std::cout << manager.getEntities("test").size() << std::endl;
  std::cout << manager.getEntities("based").size() << std::endl;

  for(auto& e : manager.getEntities())
  {
    std::cout << e->id() << " " << e->tag() << std::endl;
  }

  return 0;
}