#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <sstream>

namespace graph
{
void createWindow(std::uint32_t width, std::uint32_t height);

bool isWindowOpen();

void handleEvents();

void clearWindow(std::uint8_t r, std::uint8_t g, std::uint8_t b);

void putPixel(std::uint32_t x, std::uint32_t y, std::uint8_t r, std::uint8_t g, std::uint8_t b);

void flushWindow();
} // namespace graph

#endif /* __GRAPH_HH__ */
