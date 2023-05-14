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
  int32_t genRandomBool(void)
  {
    std::random_device rd;
    std::mt19937 gen{rd()};

    std::uniform_int_distribution<int32_t> dist{0, 1};

    return dist(gen);
  }

  void createWindow(int32_t width, int32_t height)
  {
    graph::MainWindow.create(sf::VideoMode(width, height), "");
    graph::MainWindow.setPosition({100, 100});
    graph::MainWindow.setVerticalSyncEnabled(true);
  }

  int32_t isWindowOpen(void)
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

  void clearWindow(int32_t r, int32_t g, int32_t b)
  {
    graph::MainWindow.clear({static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                             static_cast<uint8_t>(b)});
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

  void putPixel(int32_t x, int32_t y, int32_t r, int32_t g, int32_t b)
  {
    auto coord = sf::Vector2f(x, y);
    sf::RectangleShape pixel;

    pixel.setSize({1.0, 1.0});
    pixel.setFillColor({static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                        static_cast<uint8_t>(b)});
    pixel.setPosition(coord);

    graph::MainWindow.draw(pixel);
  }
}
