#include "life.hh"

namespace life
{
void swapFields(Field *f1, Field *f2)
{
  if (f1 == nullptr || f2 == nullptr)
    return;

  std::swap(f1->height, f2->height);
  std::swap(f1->width, f2->width);
  std::swap(f1->field, f2->field);
}

bool initField(Field *field, std::uint32_t width, std::uint32_t height)
{
  if (field == nullptr)
    return false;

  field->width = width;
  field->height = height;
  field->field = new (std::nothrow) bool[height * width]{};

  return field->field != nullptr;
}

void fillField(Field *field)
{
  if (field == nullptr)
    return;

  std::random_device rd;
  std::uniform_int_distribution<std::uint32_t> dist{0, 1};

  for (std::uint32_t y = 0; y < field->height; ++y)
    for (std::uint32_t x = 0; x < field->width; ++x)
      setCell(field, x, y, dist(rd));
}

void setCell(Field *field, std::uint32_t x, std::uint32_t y, bool is_alive)
{
  if (field == nullptr)
    return;

  field->field[y * field->width + x] = is_alive;
}

bool getCell(Field *field, std::int64_t x, std::int64_t y)
{
  if (field == nullptr)
    return false;

  x = (x + field->width) % field->width;
  y = (y + field->height) % field->height;

  return field->field[y * field->width + x];
}

void drawField(Field *field)
{
  if (field == nullptr)
    return;

  for (std::uint32_t y = 0; y < field->height; ++y)
    for (std::uint32_t x = 0; x < field->width; ++x)
      if (getCell(field, x, y))
        graph::putPixel(x, y, 0, 255, 0);
      else
        graph::putPixel(x, y, 0, 0, 0);
}

std::uint8_t getNeighbours(Field *field, std::uint32_t x, std::uint32_t y)
{
  if (field == nullptr)
    return 0;

  auto ix = static_cast<std::int64_t>(x);
  auto iy = static_cast<std::int64_t>(y);

  std::uint8_t num_neigh = getCell(field, ix - 1, iy - 1);

  num_neigh += getCell(field, ix, iy - 1);
  num_neigh += getCell(field, ix + 1, iy - 1);
  num_neigh += getCell(field, ix + 1, iy);
  num_neigh += getCell(field, ix + 1, iy + 1);
  num_neigh += getCell(field, ix, iy + 1);
  num_neigh += getCell(field, ix - 1, iy + 1);
  num_neigh += getCell(field, ix - 1, iy);

  return num_neigh;
}

void newGeneration(Field *cur, Field *next)
{
  if (next == nullptr || cur == nullptr)
    return;

  for (std::uint32_t y = 0; y < cur->height; ++y)
    for (std::uint32_t x = 0; x < cur->width; ++x)
    {
      auto num_neigh = getNeighbours(cur, x, y);

      bool is_alive = false;

      if (getCell(cur, x, y))
        is_alive = num_neigh == 2 || num_neigh == 3;
      else
        is_alive = num_neigh == 3;

      setCell(next, x, y, is_alive);
    }
}

void destroyField(Field *field)
{
  if (field == nullptr)
    return;

  field->height = field->width = 0;
  delete[] field->field;

  field->field = nullptr;
}
} // namespace life
