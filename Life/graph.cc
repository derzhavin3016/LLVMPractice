#include "graph.hh"

namespace graph
{
static sf::RenderWindow MainWindow{};
static sf::Clock Clock{};

void createWindow(std::uint32_t width, std::uint32_t height)
{
  MainWindow.create(sf::VideoMode(width, height), "");
  MainWindow.setPosition({100, 100});
  MainWindow.setVerticalSyncEnabled(true);
  MainWindow.setFramerateLimit(0);
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
  Clock.restart();
}

void flushWindow()
{
  MainWindow.display();
  auto elapsed = Clock.getElapsedTime();

  std::ostringstream ss;

  ss << "FPS: " << 1 / elapsed.asSeconds();
  MainWindow.setTitle(ss.str());
}

void putPixel(std::uint32_t x, std::uint32_t y, std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
  auto coord = sf::Vector2f(x, y);
  sf::RectangleShape pixel;

  pixel.setSize({1.0, 1.0});
  pixel.setFillColor({r, g, b});
  pixel.setPosition(coord);

  MainWindow.draw(pixel);
}
} // namespace graph
