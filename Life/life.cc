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
  field->field = new (std::nothrow) State[height * width]{};

  return field->field != nullptr;
}

void fillField(Field *field)
{
  if (field == nullptr)
    return;

  std::random_device rd;
  std::uniform_int_distribution<std::uint8_t> dist{0, 2};

  for (std::uint32_t y = 0; y < field->height; ++y)
    for (std::uint32_t x = 0; x < field->width; ++x)
      setCell(field, x, y, static_cast<State>(dist(rd)));
}

void setCell(Field *field, std::uint32_t x, std::uint32_t y, State state)
{
  if (field == nullptr)
    return;

  field->field[y * field->width + x] = state;
}

State getCell(Field *field, std::int64_t x, std::int64_t y)
{
  if (field == nullptr)
    return State::DEAD;

  x = (x + static_cast<std::int64_t>(field->width)) % field->width;
  y = (y + static_cast<std::int64_t>(field->height)) % field->height;

  return field->field[y * field->width + x];
}

void drawField(Field *field)
{
  if (field == nullptr)
    return;

  for (std::uint32_t y = 0; y < field->height; ++y)
    for (std::uint32_t x = 0; x < field->width; ++x)
      putPixel(x, y, getCell(field, x, y));
}

std::uint8_t getNeighbours(Field *field, std::uint32_t x, std::uint32_t y, State *newborn)
{
  if (field == nullptr || newborn == nullptr)
    return 0;

  auto ix = static_cast<std::int64_t>(x);
  auto iy = static_cast<std::int64_t>(y);

  std::uint8_t num_neigh = 0;
  int newborn_cnt = 0;

  for (std::int64_t i = -1; i <= 1; ++i)
    for (std::int64_t j = -1; j <= 1; ++j)
    {
      if (i == 0 && j == 0)
        continue;

      auto state = getCell(field, ix + i, iy + j);
      if (state == State::DEAD || ++num_neigh > 3)
        continue;

      newborn_cnt += state == State::ALIVE1 ? 1 : -1;
    }

  *newborn = newborn_cnt > 0 ? State::ALIVE1 : State::ALIVE2;

  return num_neigh;
}

void newGeneration(Field *cur, Field *next)
{
  if (next == nullptr || cur == nullptr)
    return;

  for (std::uint32_t y = 0; y < cur->height; ++y)
    for (std::uint32_t x = 0; x < cur->width; ++x)
    {
      State newborn = State::DEAD, new_state = State::DEAD, cur_state = getCell(cur, x, y);
      auto num_neigh = getNeighbours(cur, x, y, &newborn);

      if (cur_state != State::DEAD)
      {
        if (num_neigh == 2 || num_neigh == 3)
          new_state = cur_state;
      }
      else if (num_neigh == 3)
        new_state = newborn;

      setCell(next, x, y, new_state);
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

void putPixel(std::uint32_t x, std::uint32_t y, State state)
{
  struct Color
  {
    std::uint8_t r, g, b;
  } col = {0, 0, 0};

  switch (state)
  {
  case State::ALIVE1:
    col = {0, 0, 255};
    break;
  case State::ALIVE2:
    col = {255, 0, 0};
  default:
    break;
  }

  graph::putPixel(x, y, col.r, col.g, col.b);
}
} // namespace life
