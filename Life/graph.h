#ifndef __LIFE_GRAPH_H__
#define __LIFE_GRAPH_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
  int64_t genRandomBool(void);

  void createWindow(int64_t width, int64_t height);

  int64_t isWindowOpen(void);

  void handleEvents(void);

  void clearWindow(int64_t r, int64_t g, int64_t b);

  void putPixel(int64_t x, int64_t y, int64_t r, int64_t g, int64_t b);

  void flushWindow(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIFE_GRAPH_H__ */
