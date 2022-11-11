#pragma once

#include "entity_manager.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <fstream>
#include <iostream>

struct PlayerConfig 
{
  int shapeRadius;
  int collisionRadius;
  float speed;
  int fillColorR;
  int fillColorG;
  int fillColorB;
  int outlineColorR;
  int outlineColorG;
  int outlineColorB;
  int outlineThickness;
  int shapeVertices;
};

struct EnemyConfig 
{
  int shapeRadius;
  int collisionRadius;
  float minSpeed;
  float maxSpeed;
  int outlineColorR;
  int outlineColorG;
  int outlineColorB;
  int outlineThickness;
  int minShapeVertices;
  int maxShapeVertices;
  int childLifespan;
  int spawnInterval;
};

struct BulletConfig 
{
  int shapeRadius;
  int collisionRadius;
  float speed;
  int fillColorR;
  int fillColorG;
  int fillColorB;
  int outlineColorR;
  int outlineColorG;
  int outlineColorB;
  int outlineThickness;
  int shapeVertices;
  int lifespan;
};

class Game
{

  sf::RenderWindow m_window;
  EntityManager m_entities;
  sf::Font m_font;
  sf::Text m_text;
  PlayerConfig m_playerConfig;
  EnemyConfig m_enemyConfig;
  BulletConfig m_bulletConfig;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  bool m_paused = false;
  bool m_running = true;

  std::shared_ptr<Entity> m_player;

  void init(const std::string & config_path);
  void createWindowFromConfig(std::ifstream & config_file);
  void createFontAndTextFromConfig(std::ifstream & config_file);
  void setPaused(bool paused);

  void sMovement();
  void sUserInput();
  void sLifespan();
  void sRender();
  void sEnemySpawner();
  void sCollision();

  void spawnPlayer();
  void spawnEnemy();
  void spawnChildEnemies(std::shared_ptr<Entity> enemy);
  void spawnBullet(std::shared_ptr<Entity> shooter, const Vec2 & mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> shooter);

public:
  Game(const std::string & config_path);

  void run();
};