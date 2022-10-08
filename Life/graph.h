#ifndef __LIFE_GRAPH_H__
#define __LIFE_GRAPH_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
  bool genRandomBool(void);

  void createWindow(uint32_t width, uint32_t height);

  bool isWindowOpen(void);

  void handleEvents(void);

  void clearWindow(uint8_t r, uint8_t g, uint8_t b);

  void putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

  void flushWindow(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIFE_GRAPH_H__ */
