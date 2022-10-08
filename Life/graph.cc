#include "graph.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <sstream>

namespace graph
{
static sf::RenderWindow MainWindow{};
static sf::Clock Clock{};
} // namespace graph

extern "C"
{
  bool genRandomBool(void)
  {
    std::random_device rd;
    std::mt19937 gen{rd()};

    std::uniform_int_distribution<uint8_t> dist{0, 1};

    return dist(gen);
  }

  void createWindow(uint32_t width, uint32_t height)
  {
    graph::MainWindow.create(sf::VideoMode(width, height), "");
    graph::MainWindow.setPosition({100, 100});
    graph::MainWindow.setVerticalSyncEnabled(true);
  }

  bool isWindowOpen(void)
  {
    return graph::MainWindow.isOpen();
  }

  void handleEvents(void)
  {
    sf::Event event;
    while (graph::MainWindow.pollEvent(event))
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Escape))
        graph::MainWindow.close();
  }

  void clearWindow(uint8_t r, uint8_t g, uint8_t b)
  {
    graph::MainWindow.clear({r, g, b});
    graph::Clock.restart();
  }

  void flushWindow(void)
  {
    graph::MainWindow.display();
    auto elapsed = graph::Clock.getElapsedTime();

    std::ostringstream ss;

    ss << "FPS: " << 1 / elapsed.asSeconds();
    graph::MainWindow.setTitle(ss.str());
  }

  void putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
  {
    auto coord = sf::Vector2f(x, y);
    sf::RectangleShape pixel;

    pixel.setSize({1.0, 1.0});
    pixel.setFillColor({r, g, b});
    pixel.setPosition(coord);

    graph::MainWindow.draw(pixel);
  }
}
