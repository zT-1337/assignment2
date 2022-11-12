#include "game.h"

Game::Game(const std::string & config_path)
{
  init(config_path);
}

void Game::init(const std::string & config_path)
{
  std::srand(std::time(NULL));

  std::ifstream config_file(config_path);
  if(!config_file)
  {
    std::cerr << "Unable to open config file" << std::endl;
    config_file.close();
    exit(-1);
  }

  createWindowFromFile(config_file);
  createFontAndTextFromFile(config_file);
  createPlayerConfigFromFile(config_file);
  createEnemyConfigFromFile(config_file);
  createBulletConfigFromFile(config_file);
  config_file.close();

  calcPointsOfInterest();
  spawnPlayer();
}

void Game::createWindowFromFile(std::ifstream & config_file)
{
  std::string object_type;
  config_file >> object_type;

  if(object_type != "Window")
  {
    std::cerr << "Expected Window object type, but got " << object_type << std::endl;
    config_file.close();
    exit(-1);
  }

  int width, height, frame_limit;
  bool full_screen_mode; 
  config_file >> width >> height >> frame_limit >> full_screen_mode;

  sf::Uint32 style;
  if(full_screen_mode)
  {
    style = sf::Style::Fullscreen;
  } else 
  {
    style = sf::Style::Default;
  }

  m_window.create(sf::VideoMode(width, height), "SFML Works", style); 
  m_window.setFramerateLimit(frame_limit);
}

void Game::createFontAndTextFromFile(std::ifstream & config_file)
{
  std::string object_type;
  config_file >> object_type;

  if(object_type != "Font")
  {
    std::cerr << "Expected Font object type, but got " << object_type << std::endl;
    config_file.close();
    exit(-1);
  }

  std::string font_path;
  int font_size, font_color_r, font_color_g, font_color_b;
  config_file >> font_path >> font_size >> font_color_r >> font_color_g >> font_color_b;

  if(!m_font.loadFromFile(font_path))
  {
    std::cerr << "Expected font file not found" << std::endl;
    config_file.close();
    exit(-1);
  }

  m_text.setFont(m_font);
  m_text.setFillColor(sf::Color(font_color_r, font_color_g, font_color_b));
  m_text.setString("Score: 0");
  m_text.setPosition(0, 0);
}

void Game::createPlayerConfigFromFile(std::ifstream & config_file)
{
  std::string object_type;
  config_file >> object_type;

  if(object_type != "Player")
  {
    std::cerr << "Expected Player object type, but got " << object_type << std::endl;
    config_file.close();
    exit(-1);
  }

  config_file >> m_playerConfig.shapeRadius >> m_playerConfig.collisionRadius >> m_playerConfig.speed
              >> m_playerConfig.fillColorR >> m_playerConfig.fillColorG >> m_playerConfig.fillColorB
              >> m_playerConfig.outlineColorR >> m_playerConfig.outlineColorG >> m_playerConfig.outlineColorB
              >> m_playerConfig.outlineThickness >> m_playerConfig.shapeVertices;
}

void Game::createEnemyConfigFromFile(std::ifstream & config_file)
{
  std::string object_type;
  config_file >> object_type;

  if(object_type != "Enemy")
  {
    std::cerr << "Expected Enemy object type, but got " << object_type << std::endl;
    config_file.close();
    exit(-1);
  }

  config_file >> m_enemyConfig.shapeRadius >> m_enemyConfig.collisionRadius >> m_enemyConfig.minSpeed >> m_enemyConfig.maxSpeed
              >> m_enemyConfig.outlineColorR >> m_enemyConfig.outlineColorG >> m_enemyConfig.outlineColorB >> m_enemyConfig.outlineThickness
              >> m_enemyConfig.minShapeVertices >> m_enemyConfig.maxShapeVertices >> m_enemyConfig.childLifespan >> m_enemyConfig.spawnInterval;
}

void Game::createBulletConfigFromFile(std::ifstream & config_file)
{
  std::string object_type;
  config_file >> object_type;

  if(object_type != "Bullet")
  {
    std::cerr << "Expected Bullet object type, but got " << object_type << std::endl;
    config_file.close();
    exit(-1);
  }

  config_file >> m_bulletConfig.shapeRadius >> m_bulletConfig.collisionRadius >> m_bulletConfig.speed
              >> m_bulletConfig.fillColorR >> m_bulletConfig.fillColorG >> m_bulletConfig.fillColorB
              >> m_bulletConfig.outlineColorR >> m_bulletConfig.outlineColorG >> m_bulletConfig.outlineColorB
              >> m_bulletConfig.outlineThickness >> m_bulletConfig.shapeVertices >> m_bulletConfig.lifespan;
}

void Game::calcPointsOfInterest()
{
  auto window_dimensions = m_window.getSize();
  m_worldCenter.x = window_dimensions.x / 2;
  m_worldCenter.y = window_dimensions.y / 2;

  m_topLeftPlayerBound.x = m_playerConfig.collisionRadius;
  m_topLeftPlayerBound.y = m_playerConfig.collisionRadius;

  m_bottomRightPlayerBound.x = window_dimensions.x - m_playerConfig.collisionRadius;
  m_bottomRightPlayerBound.y = window_dimensions.y - m_playerConfig.collisionRadius;

  m_topLeftEnemyBound.x = m_enemyConfig.collisionRadius;
  m_topLeftEnemyBound.y = m_enemyConfig.collisionRadius;

  m_bottomRightEnemyBound.x = window_dimensions.x - m_enemyConfig.collisionRadius;
  m_bottomRightEnemyBound.y = window_dimensions.y - m_enemyConfig.collisionRadius;
}

void Game::spawnPlayer()
{
  auto player = m_entities.addEntity("player");
  m_player = player;

  player->cTransform = std::make_shared<CTransform>(
    Vec2(m_worldCenter.x, m_worldCenter.y),
    Vec2(),
    0
  );

  player->cShape = std::make_shared<CShape>(
    m_playerConfig.shapeRadius,
    m_playerConfig.shapeVertices,
    sf::Color(m_playerConfig.fillColorR, m_playerConfig.fillColorG, m_playerConfig.fillColorB),
    sf::Color(m_playerConfig.outlineColorR, m_playerConfig.outlineColorG, m_playerConfig.outlineColorB),
    m_playerConfig.outlineThickness
  );

  player->cCollision = std::make_shared<CCollision>(m_playerConfig.collisionRadius);

  player->cInput = std::make_shared<CInput>();
}

void Game::spawnEnemy()
{
  auto enemy = m_entities.addEntity("enemy");

  Vec2 enemy_velocity(rand(), rand());
  enemy_velocity.normalize();
  enemy_velocity *= randomIntInRange(m_enemyConfig.minSpeed, m_enemyConfig.maxSpeed);

  enemy->cTransform = std::make_shared<CTransform>(
    Vec2(randomIntInRange(m_topLeftEnemyBound.x, m_bottomRightEnemyBound.x), randomIntInRange(m_topLeftEnemyBound.y, m_bottomRightEnemyBound.y)),
    enemy_velocity,
    0
  );

  enemy->cShape = std::make_shared<CShape>(
    m_enemyConfig.shapeRadius,
    randomIntInRange(m_enemyConfig.minShapeVertices, m_enemyConfig.maxShapeVertices),
    sf::Color(randomIntInRange(0, 255), randomIntInRange(0, 255), randomIntInRange(0, 255)),
    sf::Color(m_enemyConfig.outlineColorR, m_enemyConfig.outlineColorG, m_enemyConfig.outlineColorB),
    m_enemyConfig.outlineThickness
  );

  enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.collisionRadius);
}

void Game::spawnBullet(std::shared_ptr<Entity> shooter, const Vec2 & mousePos)
{
  auto bullet = m_entities.addEntity("bullet");

  Vec2 bullet_velocity = mousePos - shooter->cTransform->pos;
  bullet_velocity.normalize();
  bullet_velocity *= m_bulletConfig.speed;

  bullet->cTransform = std::make_shared<CTransform>(
    Vec2(shooter->cTransform->pos.x, shooter->cTransform->pos.y),
    bullet_velocity,
    0
  );

  bullet->cShape = std::make_shared<CShape>(
    m_bulletConfig.shapeRadius,
    m_bulletConfig.shapeVertices,
    sf::Color(m_bulletConfig.fillColorR, m_bulletConfig.fillColorG, m_bulletConfig.fillColorB),
    sf::Color(m_bulletConfig.outlineColorR, m_bulletConfig.outlineColorG, m_bulletConfig.outlineColorB),
    m_bulletConfig.outlineThickness
  );

  bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.collisionRadius);
  bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.lifespan);
}

int Game::randomIntInRange(int min, int max)
{
  int r = rand() % (1 + max - min);
  return r + min;
}

void Game::sUserInput()
{
  sf::Event event;
  while(m_window.pollEvent(event))
  {
    if(event.type == sf::Event::KeyPressed)
    {
      switch(event.key.code)
      {
        case sf::Keyboard::W:
          m_player->cInput->up = true;
          break;
        case sf::Keyboard::S:
          m_player->cInput->down = true;
          break;
        case sf::Keyboard::A:
          m_player->cInput->left = true;
          break;
        case sf::Keyboard::D:
          m_player->cInput->right = true;
          break;
        default:
          break;
      }
    }

    if(event.type == sf::Event::KeyReleased)
    {
      switch(event.key.code)
      {
        case sf::Keyboard::W:
          m_player->cInput->up = false;
          break;
        case sf::Keyboard::S:
          m_player->cInput->down = false;
          break;
        case sf::Keyboard::A:
          m_player->cInput->left = false;
          break;
        case sf::Keyboard::D:
          m_player->cInput->right = false;
          break;
        case sf::Keyboard::Escape:
          m_window.close();
          exit(0);
          break;
        default:
          break;
      }
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {
      switch (event.mouseButton.button)
      {
        case sf::Mouse::Left:
        {
          auto mouse_pos = sf::Mouse::getPosition(m_window);
          spawnBullet(m_player, Vec2(mouse_pos.x, mouse_pos.y));
          break;
        }
        default:
          break;
      }
    }
  }
}

void Game::sMovement()
{
  ssMovePlayer();
  ssMoveBullets();
  ssMoveEnemies();
}

void Game::ssMovePlayer()
{
  m_player->cTransform->velocity.origin();

  if(m_player->cInput->up)
  {
    m_player->cTransform->velocity.y -= 1;
  }
  if(m_player->cInput->down)
  {
    m_player->cTransform->velocity.y += 1;
  }
  if(m_player->cInput->left)
  {
    m_player->cTransform->velocity.x -= 1;
  }
  if(m_player->cInput->right)
  {
    m_player->cTransform->velocity.x += 1;
  }

  m_player->cTransform->velocity.normalize();
  m_player->cTransform->velocity *= m_playerConfig.speed;
  m_player->cTransform->pos.clampedAddition(m_player->cTransform->velocity, m_topLeftPlayerBound, m_bottomRightPlayerBound);
}

void Game::ssMoveBullets()
{
  for(auto& e : m_entities.getEntities("bullet"))
  {
    e->cTransform->pos += e->cTransform->velocity;
  }
}

void Game::ssMoveEnemies()
{
  for(auto& e : m_entities.getEntities("enemy"))
  {
    e->cTransform->pos += e->cTransform->velocity;

    if(e->cTransform->pos.x < m_topLeftEnemyBound.x || e->cTransform->pos.x > m_bottomRightEnemyBound.x)
    {
      e->cTransform->velocity.x *= -1;
    }
    if(e->cTransform->pos.y < m_topLeftEnemyBound.y || e->cTransform->pos.y > m_bottomRightEnemyBound.y)
    {
      e->cTransform->velocity.y *= -1;
    }
  }
}

void Game::sLifespan()
{
  for(auto& e : m_entities.getEntities())
  {
    if(!e->cLifespan)
    {
      continue;
    }

    e->cLifespan->remaining -= 1;
    if(e->cLifespan->remaining < 1)
    {
      e->destroy();
    }
  }
}

void Game::sEnemySpawner()
{
  if(m_lastEnemySpawnTime + m_enemyConfig.spawnInterval == m_currentFrame)
  {
    spawnEnemy();
    m_lastEnemySpawnTime = m_currentFrame;
  }
}

void Game::sCollision()
{
  for(auto& enemy : m_entities.getEntities("enemy"))
  {
    if(isColliding(enemy, m_player))
    {
      m_player->destroy();
      enemy->destroy();
      spawnPlayer();
    }

    for(auto& bullet : m_entities.getEntities("bullet"))
    {
      if(isColliding(enemy, bullet))
      {
        bullet->destroy();
        enemy->destroy();
        //Spawn child enemies if enemy has no lifespan component
      }
    }
  }
}

bool Game::isColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
  Vec2 distance_vec = a->cTransform->pos - b->cTransform->pos;
  int radius_sum = a->cCollision->radius + b->cCollision->radius;

  return radius_sum * radius_sum > distance_vec.x*distance_vec.x + distance_vec.y*distance_vec.y; 

  return true;
}

void Game::sRender()
{
  m_window.clear();

  for(auto& e : m_entities.getEntities())
  {
    if(!e->cShape)
    {
      continue;
    }

    if(e->cTransform)
    {
      e->cTransform->angle += 2.0f;
      e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
      e->cShape->circle.setRotation(e->cTransform->angle);
    }

    if(e->cLifespan)
    {
      int opacity = 255 * ((float)e->cLifespan->remaining / e->cLifespan->total);
      sf::Color fill_color = e->cShape->circle.getFillColor();
      sf::Color outline_color = e->cShape->circle.getOutlineColor();

      e->cShape->circle.setFillColor(sf::Color(fill_color.r, fill_color.g, fill_color.b, opacity));
      e->cShape->circle.setOutlineColor(sf::Color(outline_color.r, outline_color.g, outline_color.b, opacity));
    }

    m_window.draw(e->cShape->circle);
  }

  m_window.draw(m_text);
  m_window.display();
}

void Game::run()
{
  while(m_running)
  {
    m_entities.update();
    sUserInput();
    sMovement();
    sLifespan();
    sEnemySpawner();
    sCollision();
    sRender();
    m_currentFrame++;
  }
}