#include "life.h"


void Swap( FieldPtr *pF1, FieldPtr *pF2 )
{
  FieldPtr tmp = *pF1;
  *pF1 = *pF2;
  *pF2 = tmp;
}

void SetCell( FieldPtr pF, size_t x, size_t y, bool is_alive )
{
  //assert(x < FRAME_W && y < FRAME_H);
  pF[y][x] = is_alive;
}

Byte GetCell( FieldPtr pF, size_t x, size_t y )
{
  x = (x + LIFE_W) % LIFE_W;
  y = (y + LIFE_H) % LIFE_H;
  return pF[y][x];
}

void FieldInit( FieldPtr pF )
{
  for (size_t y = 0; y < LIFE_H; ++y)
    for (size_t x = 0; x < LIFE_W; ++x)
      SetCell(pF, x, y, true);
}


size_t GetNeighbours( FieldPtr pF, size_t x, size_t y )
{
  return GetCell(pF, x - 1, y) + GetCell(pF, x - 1, y + 1) +
         GetCell(pF, x, y + 1) + GetCell(pF, x + 1, y + 1) +
         GetCell(pF, x + 1, y) + GetCell(pF, x + 1, y - 1) +
         GetCell(pF, x, y - 1) + GetCell(pF, x - 1, y - 1);
}

void FieldDraw( FieldPtr pF )
{
  for (size_t y = 0; y < LIFE_H; ++y)
    for (size_t x = 0; x < LIFE_W; ++x)
      if (GetCell(pF, x, y))
        PutPixel(x, y, 0, 0, 128);
      else
        PutPixel(x, y, 255, 0, 0);
}

void NewGeneration( FieldPtr pF1, FieldPtr pF2 )
{
  for (size_t y = 0; y < LIFE_H; ++y)
    for (size_t x = 0; x < LIFE_W; ++x)
    {
      size_t num_neigh = GetNeighbours(pF1, x, y);
      bool is_alive = false;

      if (GetCell(pF1, x, y))
        is_alive = num_neigh == 2 || num_neigh == 3;
      else
        is_alive = num_neigh == 3;

      SetCell(pF2, x, y, is_alive);
    }
}
