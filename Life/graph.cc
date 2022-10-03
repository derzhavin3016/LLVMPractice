#include "graph.hh"

namespace graph
{
static sf::RenderWindow MainWindow{};

void createWindow(std::uint32_t width, std::uint32_t height)
{
  MainWindow.create(sf::VideoMode(width, height), "Game of Life");
  MainWindow.setPosition({100, 100});
}

bool isWindowOpen()
{
  return MainWindow.isOpen();
}

void handleEvents()
{
  sf::Event event;
  while (MainWindow.pollEvent(event))
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
      MainWindow.close();
}

void clearWindow(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
  MainWindow.clear({r, g, b});
}

void flushWindow()
{
  MainWindow.display();
}

void putPixel(std::uint32_t x, std::uint32_t y, std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
  auto coord = sf::Vector2f(x, y);

  sf::Vertex point(coord, {r, g, b});
  MainWindow.draw(&point, 1, sf::Points);
}
} // namespace graph
