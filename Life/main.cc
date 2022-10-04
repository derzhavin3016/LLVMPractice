#include "life.hh"

int main()
{
  std::uint32_t height = 400;
  std::uint32_t width = 400;

  graph::createWindow(width, height);

  life::Field field1, field2;
  if (!life::initField(&field1, width, height))
    return 1;
  if (!life::initField(&field2, width, height))
  {
    life::destroyField(&field1);
    return 1;
  }

  life::fillField(&field1);

  while (graph::isWindowOpen())
  {
    graph::handleEvents();

    graph::clearWindow(0, 0, 0);

    life::newGeneration(&field1, &field2);
    life::drawField(&field2);
    life::swapFields(&field1, &field2);

    graph::flushWindow();
  }

  life::destroyField(&field1);
  life::destroyField(&field2);
}
