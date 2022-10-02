#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <GL/glut.h>

#define LIFE_H 100ul
#define LIFE_W 100ul
#define FRAME_H LIFE_H
#define FRAME_W LIFE_W

#define WND_H 800ul
#define WND_W 800ul

typedef unsigned char Byte;

typedef Byte Field_t[LIFE_H][LIFE_W];
typedef Byte (*FieldPtr)[LIFE_W];
typedef Byte Frame_t[FRAME_H][FRAME_W][3];

extern Field_t Field1, Field2;
extern Frame_t Frame;

void PutPixel( size_t x, size_t y, Byte r, Byte g, Byte b);

void SetCell( FieldPtr pF, size_t x, size_t y, bool is_alive );

Byte GetCell( FieldPtr pF, size_t x, size_t y );

void FieldInit( FieldPtr pF );

size_t GetNeighbours( FieldPtr pF, size_t x, size_t y );

void FieldDraw( FieldPtr pF );

void NewGeneration( FieldPtr pF1, FieldPtr pF2 );

void Swap( FieldPtr *pF1, FieldPtr *pF2 );
