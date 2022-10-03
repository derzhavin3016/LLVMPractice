#ifndef __LIFE_H__
#define __LIFE_H__

#include <random>

#include "graph.hh"

namespace life
{
struct Field
{
  std::uint32_t height;
  std::uint32_t width;
  bool *field = nullptr;
};

void swapFields(Field *f1, Field *f2);

bool initField(Field *field, std::uint32_t width, std::uint32_t height);
void fillField(Field *field);
void destroyField(Field *field);

void setCell(Field *field, std::uint32_t x, std::uint32_t y, bool is_alive);
bool getCell(Field *field, std::int64_t x, std::int64_t y);

std::uint8_t getNeighbours(Field *field, std::uint32_t x, std::uint32_t y);

void newGeneration(Field *cur, Field *next);

void drawField(Field *field);
}

#endif // __LIFE_H__
