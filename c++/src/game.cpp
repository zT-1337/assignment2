#include "game.h"

Game::Game(const std::string & config_path)
{
  init(config_path);
}

void Game::init(const std::string & config_path)
{
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
          m_running = false;
          m_window.close();
          break;
        default:
          break;
      }
    }
  }
}

void Game::sMovement()
{
  ssMovePlayer();
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
    sRender();
    m_currentFrame++;
  }
}