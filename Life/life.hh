#ifndef __LIFE_H__
#define __LIFE_H__

#include <random>

#include "graph.hh"

namespace life
{
enum class State
{
  DEAD = 0,
  ALIVE1 = 1,
  ALIVE2 = 2
};

struct Field
{
  std::uint32_t height;
  std::uint32_t width;
  State *field = nullptr;
};

void swapFields(Field *f1, Field *f2);

bool initField(Field *field, std::uint32_t width, std::uint32_t height);
void fillField(Field *field);
void destroyField(Field *field);

void setCell(Field *field, std::uint32_t x, std::uint32_t y, State state);
State getCell(Field *field, std::int64_t x, std::int64_t y);

std::uint8_t getNeighbours(Field *field, std::uint32_t x, std::uint32_t y, State *newborn);

void newGeneration(Field *cur, Field *next);

void putPixel(std::uint32_t x, std::uint32_t y, State state);

void drawField(Field *field);
} // namespace life

#endif // __LIFE_H__
