#ifndef __LIFE_GRAPH_H__
#define __LIFE_GRAPH_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
  int32_t genRandomBool(void);

  void createWindow(int32_t width, int32_t height);

  int32_t isWindowOpen(void);

  void handleEvents(void);

  void clearWindow(int32_t r, int32_t g, int32_t b);

  void putPixel(int32_t x, int32_t y, int32_t r, int32_t g, int32_t b);

  void flushWindow(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIFE_GRAPH_H__ */
