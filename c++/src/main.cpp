#include <iostream>
#include "vec2.h"

int main()
{
  Vec2 a;
  Vec2 b(50.0f, 40.0f);
  Vec2 c(50.0f, 40.0f);

  std::cout << "init" << std::endl;
  std::cout << a.x << " " << a.y << std::endl;
  std::cout << b.x << " " << b.y << std::endl;

  std::cout << "equals" << std::endl;
  std::cout << (a == b) << std::endl;
  std::cout << (a != b) << std::endl;

  std::cout << (c == b) << std::endl;
  std::cout << (c != b) << std::endl;

  std::cout << "addition" << std::endl;
  Vec2 d = a + b + c;
  std::cout << d.x << " " << d.y << std::endl;

  std::cout << "subtraction" << std::endl;
  Vec2 e = d - b;
  std::cout << e.x << " " << e.y << std::endl;

  std::cout << "multiplication with single value" << std::endl;
  Vec2 f = b * 2;
  std::cout << f.x << " " << f.y << std::endl;

  std::cout << "division with single value" << std::endl;
  Vec2 g = b / 2;
  std::cout << g.x << " " << g.y << std::endl;

  std::cout << "addition withouth new instance" << std::endl;
  b += c;
  std::cout << b.x << " " << b.y << std::endl;

  std::cout << "subtraction without new instance" << std::endl;
  b -= c;
  std::cout << b.x << " " << b.y << std::endl;

  std::cout << "multiplication with single value withouth new instance" << std::endl;
  b *= 2;
  std::cout << b.x << " " << b.y << std::endl;

  std::cout << "division with single value withouth new instance" << std::endl;
  b /= 2;
  std::cout << b.x << " " << b.y << std::endl;

  return 0;
}