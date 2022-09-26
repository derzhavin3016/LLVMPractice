/*  */

#include "life.h"

Byte
  Field1[LIFE_H][LIFE_W],
  Field2[LIFE_H][LIFE_W];

void Swap( Byte (*F1)[LIFE_W], Byte (*F2)[LIFE_W] )
{
  for (int y = 0; y < LIFE_H; y++)
    for (int x = 0; x < LIFE_W; x++)
      F1[y][x] = F2[y][x];
}

void SetCell( Byte (*F)[LIFE_W], int x, int y, int is_alive )
{
  F[y][x] = is_alive;
}

int GetCell( Byte (*F)[LIFE_W], int x, int y )
{
  x = (x + LIFE_W) % LIFE_W;
  y = (y + LIFE_H) % LIFE_H;
  return F[y][x];
}

void FieldInit( Byte (*F)[LIFE_W] )
{
  for (int y = 0; y < LIFE_H; y++)
    for (int x = 0; x < LIFE_W; x++)
      SetCell(F, x, y, rand() % 2);
}


int GetNeighbours( Byte (*F)[LIFE_W], int x, int y )
{
  return GetCell(F, x - 1, y) + GetCell(F, x - 1, y + 1) +
    GetCell(F, x, y + 1) + GetCell(F, x + 1, y + 1) +
    GetCell(F, x + 1, y) + GetCell(F, x + 1, y - 1) +
    GetCell(F, x, y - 1) + GetCell(F, x - 1, y - 1);
}

void FieldDraw( Byte (*F)[LIFE_W] )
{
  for (int y = 0; y < LIFE_H; y++)
    for (int x = 0; x < LIFE_W; x++)
      if (GetCell(F, x, y))
        PutPixel(x, y, 0, 0.8, 0);
      else
        PutPixel(x, y, 1, 0, 0);
}

void NewGeneration( Byte (*F1)[LIFE_W], Byte (*F2)[LIFE_W] )
{
  for (int y = 0; y < LIFE_H; y++)
    for (int x = 0; x < LIFE_W; x++)
    {
      int num_neigh = GetNeighbours(F1, x, y);
      int is_alive = 0;

      if (GetCell(F1, x, y))
        if (num_neigh < 2 || num_neigh > 3)
          is_alive = 0;
        else
          is_alive = 1;
      else
        if (num_neigh == 3)
          is_alive = 1;
        else
          is_alive = 0;

      SetCell(F2, x, y, is_alive);
    }
}
