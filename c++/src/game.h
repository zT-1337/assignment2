#pragma once

#include "entity_manager.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <fstream>
#include <iostream>
#include <cmath>

#define PI 3.14159265

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
  sf::Text m_killstreak_text;
  PlayerConfig m_playerConfig;
  EnemyConfig m_enemyConfig;
  BulletConfig m_bulletConfig;
  int m_score = 0;
  int m_req_killstreak = 1;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  int m_lastParticleSpawnTime = 0;
  bool m_paused = false;
  bool m_running = true;

  Vec2 m_worldCenter;
  Vec2 m_topLeftPlayerBound;
  Vec2 m_bottomRightPlayerBound;
  Vec2 m_topLeftEnemyBound;
  Vec2 m_bottomRightEnemyBound;

  std::shared_ptr<Entity> m_player;

  void init(const std::string & config_path);
  void createWindowFromFile(std::ifstream & config_file);
  void createFontAndTextFromFile(std::ifstream & config_file);
  void createPlayerConfigFromFile(std::ifstream & config_file);
  void createEnemyConfigFromFile(std::ifstream & config_file);
  void createBulletConfigFromFile(std::ifstream & config_file);
  void calcPointsOfInterest();
  void setPaused(bool paused);
  void setScore(int score);
  void setKillstreak(int killstreak);

  void sMovement();
  void ssMovePlayer();
  void ssMoveKillstreak();
  void ssMoveBulletsAndParticles();
  void ssMoveEnemies();
  void sUserInput();
  void sLifespan();
  void sRender();
  void sEnemySpawner();
  void sCollision();
  void sSuperArmor();

  bool isColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

  void spawnPlayer();
  void spawnKillstreak();
  void spawnArmorParticles();
  void spawnEnemy();
  void spawnChildEnemies(std::shared_ptr<Entity> enemy);
  void spawnBullet(std::shared_ptr<Entity> shooter, const Vec2 & mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> shooter);

  int randomIntInRange(int min, int max);

public:
  Game(const std::string & config_path);

  void run();
};