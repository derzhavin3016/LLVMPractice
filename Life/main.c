#include "graph.h"

#define FIELD_WIDTH 400ull
#define FIELD_HEIGHT 400ull

#define ACCESS_FIELD(field, x, y) (field)[(y) * (FIELD_WIDTH) + (x)]

bool *pActiveField = 0;
bool *pNextField = 0;

bool getCell(int64_t x, int64_t y)
{
  x = (x + FIELD_WIDTH) % FIELD_WIDTH;
  y = (y + FIELD_HEIGHT) % FIELD_HEIGHT;

  return ACCESS_FIELD(pActiveField, x, y);
}

void fillField(void)
{
  for (uint32_t y = 0; y < FIELD_HEIGHT; ++y)
    for (uint32_t x = 0; x < FIELD_WIDTH; ++x)
      ACCESS_FIELD(pActiveField, x, y) = genRandomBool();
}

uint8_t getNeighbours(uint32_t x, uint32_t y)
{
  int64_t ix = (int64_t)x;
  int64_t iy = (int64_t)y;

  uint8_t num_neigh = 0;

  num_neigh += getCell(ix - 1, iy - 1);
  num_neigh += getCell(ix, iy - 1);
  num_neigh += getCell(ix + 1, iy - 1);
  num_neigh += getCell(ix + 1, iy);
  num_neigh += getCell(ix + 1, iy + 1);
  num_neigh += getCell(ix, iy + 1);
  num_neigh += getCell(ix - 1, iy + 1);
  num_neigh += getCell(ix - 1, iy);

  return num_neigh;
}

void newGeneration(void)
{
  for (uint32_t y = 0; y < FIELD_HEIGHT; ++y)
    for (uint32_t x = 0; x < FIELD_WIDTH; ++x)
    {
      uint8_t num_neigh = getNeighbours(x, y);

      bool is_alive = false;

      if (getCell(x, y))
        is_alive = num_neigh == 2 || num_neigh == 3;
      else
        is_alive = num_neigh == 3;

      ACCESS_FIELD(pNextField, x, y) = is_alive;
    }
}

void drawField(void)
{
  for (uint32_t y = 0; y < FIELD_HEIGHT; ++y)
    for (uint32_t x = 0; x < FIELD_WIDTH; ++x)
      if (getCell(x, y))
        putPixel(x, y, 0, 255, 0);
      else
        putPixel(x, y, 0, 0, 0);
}

void swapActiveField(void)
{
  bool *tmp = pActiveField;
  pActiveField = pNextField;
  pNextField = tmp;
}

int main()
{
  bool field1[FIELD_WIDTH * FIELD_HEIGHT] = {0},
                            field2[FIELD_WIDTH * FIELD_HEIGHT] = {0};

  pActiveField = field1;
  pNextField = field2;

  createWindow(FIELD_WIDTH, FIELD_HEIGHT);

  fillField();

  while (isWindowOpen())
  {
    handleEvents();

    clearWindow(0, 0, 0);

    newGeneration();
    drawField();
    swapActiveField();

    flushWindow();
  }
}
