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
  config_file.close();

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

void Game::spawnPlayer()
{
  auto player = m_entities.addEntity("player");
  m_player = player;

  auto window_dimensions = m_window.getSize();
  player->cTransform = std::make_shared<CTransform>(
    Vec2(window_dimensions.x/2, window_dimensions.y/2),
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

void Game::sRender()
{
  m_window.clear();

  m_window.draw(m_text);

  for(auto& e : m_entities.getEntities())
  {
    auto entity_shape = e->cShape;
    if(!entity_shape)
    {
      continue;
    }

    auto entity_transform = e->cTransform;
    if(entity_transform)
    {
      entity_transform->angle += 2.0f;
      entity_shape->circle.setPosition(entity_transform->pos.x, entity_transform->pos.y);
      entity_shape->circle.setRotation(entity_transform->angle);
    }

    m_window.draw(entity_shape->circle);
  }

  m_window.display();
}

void Game::run()
{
  while(m_running)
  {
    m_entities.update();
    sRender();
  }
}